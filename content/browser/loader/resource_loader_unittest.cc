// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/loader/resource_loader.h"

#include "base/files/file.h"
#include "base/files/file_util.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/run_loop.h"
#include "content/browser/browser_thread_impl.h"
#include "content/browser/loader/redirect_to_file_resource_handler.h"
#include "content/browser/loader/resource_loader_delegate.h"
#include "content/public/browser/resource_request_info.h"
#include "content/public/common/resource_response.h"
#include "content/public/test/mock_resource_context.h"
#include "content/public/test/test_browser_thread_bundle.h"
#include "content/test/test_content_browser_client.h"
#include "ipc/ipc_message.h"
#include "net/base/io_buffer.h"
#include "net/base/mock_file_stream.h"
#include "net/base/request_priority.h"
#include "net/cert/x509_certificate.h"
#include "net/ssl/client_cert_store.h"
#include "net/ssl/ssl_cert_request_info.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job_factory_impl.h"
#include "net/url_request/url_request_test_job.h"
#include "net/url_request/url_request_test_util.h"
#include "storage/browser/blob/shareable_file_reference.h"
#include "testing/gtest/include/gtest/gtest.h"

using storage::ShareableFileReference;

namespace content {
namespace {

// Stub client certificate store that returns a preset list of certificates for
// each request and records the arguments of the most recent request for later
// inspection.
class ClientCertStoreStub : public net::ClientCertStore {
 public:
  // Creates a new ClientCertStoreStub that returns |response| on query. It
  // saves the number of requests and most recently certificate authorities list
  // in |requested_authorities| and |request_count|, respectively. The caller is
  // responsible for ensuring those pointers outlive the ClientCertStoreStub.
  //
  // TODO(ppi): Make the stub independent from the internal representation of
  // SSLCertRequestInfo. For now it seems that we can neither save the
  // scoped_refptr<> (since it is never passed to us) nor copy the entire
  // CertificateRequestInfo (since there is no copy constructor).
  ClientCertStoreStub(const net::CertificateList& response,
                      int* request_count,
                      std::vector<std::string>* requested_authorities)
      : response_(response),
        async_(false),
        requested_authorities_(requested_authorities),
        request_count_(request_count) {
    requested_authorities_->clear();
    *request_count_ = 0;
  }

  ~ClientCertStoreStub() override {}

  // Configures whether the certificates are returned asynchronously or not.
  void set_async(bool async) { async_ = async; }

  // net::ClientCertStore:
  void GetClientCerts(const net::SSLCertRequestInfo& cert_request_info,
                      net::CertificateList* selected_certs,
                      const base::Closure& callback) override {
    *requested_authorities_ = cert_request_info.cert_authorities;
    ++(*request_count_);

    *selected_certs = response_;
    if (async_) {
      base::MessageLoop::current()->PostTask(FROM_HERE, callback);
    } else {
      callback.Run();
    }
  }

 private:
  const net::CertificateList response_;
  bool async_;
  std::vector<std::string>* requested_authorities_;
  int* request_count_;
};

// Arbitrary read buffer size.
const int kReadBufSize = 1024;

// Dummy implementation of ResourceHandler, instance of which is needed to
// initialize ResourceLoader.
class ResourceHandlerStub : public ResourceHandler {
 public:
  explicit ResourceHandlerStub(net::URLRequest* request)
      : ResourceHandler(request),
        read_buffer_(new net::IOBuffer(kReadBufSize)),
        defer_request_on_will_start_(false),
        expect_reads_(true),
        cancel_on_read_completed_(false),
        defer_eof_(false),
        received_on_will_read_(false),
        received_eof_(false),
        received_response_completed_(false),
        total_bytes_downloaded_(0) {
  }

  // If true, defers the resource load in OnWillStart.
  void set_defer_request_on_will_start(bool defer_request_on_will_start) {
    defer_request_on_will_start_ = defer_request_on_will_start;
  }

  // If true, expect OnWillRead / OnReadCompleted pairs for handling
  // data. Otherwise, expect OnDataDownloaded.
  void set_expect_reads(bool expect_reads) { expect_reads_ = expect_reads; }

  // If true, cancel the request in OnReadCompleted by returning false.
  void set_cancel_on_read_completed(bool cancel_on_read_completed) {
    cancel_on_read_completed_ = cancel_on_read_completed;
  }

  // If true, cancel the request in OnReadCompleted by returning false.
  void set_defer_eof(bool defer_eof) { defer_eof_ = defer_eof; }

  const GURL& start_url() const { return start_url_; }
  ResourceResponse* response() const { return response_.get(); }
  bool received_response_completed() const {
    return received_response_completed_;
  }
  const net::URLRequestStatus& status() const { return status_; }
  int total_bytes_downloaded() const { return total_bytes_downloaded_; }

  void Resume() {
    controller()->Resume();
  }

  // ResourceHandler implementation:
  bool OnUploadProgress(uint64 position, uint64 size) override {
    NOTREACHED();
    return true;
  }

  bool OnRequestRedirected(const net::RedirectInfo& redirect_info,
                           ResourceResponse* response,
                           bool* defer) override {
    NOTREACHED();
    return true;
  }

  bool OnResponseStarted(ResourceResponse* response, bool* defer) override {
    EXPECT_FALSE(response_.get());
    response_ = response;
    return true;
  }

  bool OnWillStart(const GURL& url, bool* defer) override {
    EXPECT_TRUE(start_url_.is_empty());
    start_url_ = url;
    *defer = defer_request_on_will_start_;
    return true;
  }

  bool OnBeforeNetworkStart(const GURL& url, bool* defer) override {
    return true;
  }

  bool OnWillRead(scoped_refptr<net::IOBuffer>* buf,
                  int* buf_size,
                  int min_size) override {
    EXPECT_TRUE(expect_reads_);
    EXPECT_FALSE(received_on_will_read_);
    EXPECT_FALSE(received_eof_);
    EXPECT_FALSE(received_response_completed_);

    *buf = read_buffer_;
    *buf_size = kReadBufSize;
    received_on_will_read_ = true;
    return true;
  }

  bool OnReadCompleted(int bytes_read, bool* defer) override {
    EXPECT_TRUE(received_on_will_read_);
    EXPECT_TRUE(expect_reads_);
    EXPECT_FALSE(received_response_completed_);

    if (bytes_read == 0) {
      received_eof_ = true;
      if (defer_eof_) {
        defer_eof_ = false;
        *defer = true;
      }
    }

    // Need another OnWillRead() call before seeing an OnReadCompleted().
    received_on_will_read_ = false;

    return !cancel_on_read_completed_;
  }

  void OnResponseCompleted(const net::URLRequestStatus& status,
                           const std::string& security_info,
                           bool* defer) override {
    EXPECT_FALSE(received_response_completed_);
    if (status.is_success() && expect_reads_)
      EXPECT_TRUE(received_eof_);

    received_response_completed_ = true;
    status_ = status;
  }

  void OnDataDownloaded(int bytes_downloaded) override {
    EXPECT_FALSE(expect_reads_);
    total_bytes_downloaded_ += bytes_downloaded;
  }

 private:
  scoped_refptr<net::IOBuffer> read_buffer_;

  bool defer_request_on_will_start_;
  bool expect_reads_;
  bool cancel_on_read_completed_;
  bool defer_eof_;

  GURL start_url_;
  scoped_refptr<ResourceResponse> response_;
  bool received_on_will_read_;
  bool received_eof_;
  bool received_response_completed_;
  net::URLRequestStatus status_;
  int total_bytes_downloaded_;
};

// Test browser client that captures calls to SelectClientCertificates and
// records the arguments of the most recent call for later inspection.
class SelectCertificateBrowserClient : public TestContentBrowserClient {
 public:
  SelectCertificateBrowserClient() : call_count_(0) {}

  void SelectClientCertificate(
      int render_process_id,
      int render_view_id,
      net::SSLCertRequestInfo* cert_request_info,
      const base::Callback<void(net::X509Certificate*)>& callback) override {
    ++call_count_;
    passed_certs_ = cert_request_info->client_certs;
  }

  int call_count() {
    return call_count_;
  }

  net::CertificateList passed_certs() {
    return passed_certs_;
  }

 private:
  net::CertificateList passed_certs_;
  int call_count_;
};

class ResourceContextStub : public MockResourceContext {
 public:
  explicit ResourceContextStub(net::URLRequestContext* test_request_context)
      : MockResourceContext(test_request_context) {}

  scoped_ptr<net::ClientCertStore> CreateClientCertStore() override {
    return dummy_cert_store_.Pass();
  }

  void SetClientCertStore(scoped_ptr<net::ClientCertStore> store) {
    dummy_cert_store_ = store.Pass();
  }

 private:
  scoped_ptr<net::ClientCertStore> dummy_cert_store_;
};

// Fails to create a temporary file with the given error.
void CreateTemporaryError(
    base::File::Error error,
    const CreateTemporaryFileStreamCallback& callback) {
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(callback, error, base::Passed(scoped_ptr<net::FileStream>()),
                 scoped_refptr<ShareableFileReference>()));
}

}  // namespace

class ResourceLoaderTest : public testing::Test,
                           public ResourceLoaderDelegate {
 protected:
  ResourceLoaderTest()
    : thread_bundle_(content::TestBrowserThreadBundle::IO_MAINLOOP),
      resource_context_(&test_url_request_context_),
      raw_ptr_resource_handler_(NULL),
      raw_ptr_to_request_(NULL) {
    job_factory_.SetProtocolHandler(
        "test", net::URLRequestTestJob::CreateProtocolHandler());
    test_url_request_context_.set_job_factory(&job_factory_);
  }

  GURL test_url() const {
    return net::URLRequestTestJob::test_url_1();
  }

  std::string test_data() const {
    return net::URLRequestTestJob::test_data_1();
  }

  virtual scoped_ptr<ResourceHandler> WrapResourceHandler(
      scoped_ptr<ResourceHandlerStub> leaf_handler,
      net::URLRequest* request) {
    return leaf_handler.Pass();
  }

  void SetUp() override {
    const int kRenderProcessId = 1;
    const int kRenderViewId = 2;

    scoped_ptr<net::URLRequest> request(
        resource_context_.GetRequestContext()->CreateRequest(
            test_url(),
            net::DEFAULT_PRIORITY,
            NULL /* delegate */,
            NULL /* cookie_store */));
    raw_ptr_to_request_ = request.get();
    ResourceRequestInfo::AllocateForTesting(request.get(),
                                            RESOURCE_TYPE_MAIN_FRAME,
                                            &resource_context_,
                                            kRenderProcessId,
                                            kRenderViewId,
                                            MSG_ROUTING_NONE,
                                            true,    // is_main_frame
                                            false,   // parent_is_main_frame
                                            true,    // allow_download
                                            false);  // is_async
    scoped_ptr<ResourceHandlerStub> resource_handler(
        new ResourceHandlerStub(request.get()));
    raw_ptr_resource_handler_ = resource_handler.get();
    loader_.reset(new ResourceLoader(
        request.Pass(),
        WrapResourceHandler(resource_handler.Pass(), raw_ptr_to_request_),
        this));
  }

  // ResourceLoaderDelegate:
  ResourceDispatcherHostLoginDelegate* CreateLoginDelegate(
      ResourceLoader* loader,
      net::AuthChallengeInfo* auth_info) override {
    return NULL;
  }
  bool HandleExternalProtocol(ResourceLoader* loader,
                              const GURL& url) override {
    return false;
  }
  void DidStartRequest(ResourceLoader* loader) override {}
  void DidReceiveRedirect(ResourceLoader* loader,
                          const GURL& new_url) override {}
  void DidReceiveResponse(ResourceLoader* loader) override {}
  void DidFinishLoading(ResourceLoader* loader) override {}

  content::TestBrowserThreadBundle thread_bundle_;

  net::URLRequestJobFactoryImpl job_factory_;
  net::TestURLRequestContext test_url_request_context_;
  ResourceContextStub resource_context_;

  // The ResourceLoader owns the URLRequest and the ResourceHandler.
  ResourceHandlerStub* raw_ptr_resource_handler_;
  net::URLRequest* raw_ptr_to_request_;
  scoped_ptr<ResourceLoader> loader_;
};

// Verifies if a call to net::UrlRequest::Delegate::OnCertificateRequested()
// causes client cert store to be queried for certificates and if the returned
// certificates are correctly passed to the content browser client for
// selection.
TEST_F(ResourceLoaderTest, ClientCertStoreLookup) {
  // Set up the test client cert store.
  int store_request_count;
  std::vector<std::string> store_requested_authorities;
  net::CertificateList dummy_certs(1, scoped_refptr<net::X509Certificate>(
      new net::X509Certificate("test", "test", base::Time(), base::Time())));
  scoped_ptr<ClientCertStoreStub> test_store(new ClientCertStoreStub(
      dummy_certs, &store_request_count, &store_requested_authorities));
  resource_context_.SetClientCertStore(test_store.Pass());

  // Prepare a dummy certificate request.
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info(
      new net::SSLCertRequestInfo());
  std::vector<std::string> dummy_authority(1, "dummy");
  cert_request_info->cert_authorities = dummy_authority;

  // Plug in test content browser client.
  SelectCertificateBrowserClient test_client;
  ContentBrowserClient* old_client = SetBrowserClientForTesting(&test_client);

  // Everything is set up. Trigger the resource loader certificate request event
  // and run the message loop.
  loader_->OnCertificateRequested(raw_ptr_to_request_, cert_request_info.get());
  base::RunLoop().RunUntilIdle();

  // Restore the original content browser client.
  SetBrowserClientForTesting(old_client);

  // Check if the test store was queried against correct |cert_authorities|.
  EXPECT_EQ(1, store_request_count);
  EXPECT_EQ(dummy_authority, store_requested_authorities);

  // Check if the retrieved certificates were passed to the content browser
  // client.
  EXPECT_EQ(1, test_client.call_count());
  EXPECT_EQ(dummy_certs, test_client.passed_certs());
}

// Verifies if a call to net::URLRequest::Delegate::OnCertificateRequested()
// on a platform with a NULL client cert store still calls the content browser
// client for selection.
TEST_F(ResourceLoaderTest, ClientCertStoreNull) {
  // Prepare a dummy certificate request.
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info(
      new net::SSLCertRequestInfo());
  std::vector<std::string> dummy_authority(1, "dummy");
  cert_request_info->cert_authorities = dummy_authority;

  // Plug in test content browser client.
  SelectCertificateBrowserClient test_client;
  ContentBrowserClient* old_client = SetBrowserClientForTesting(&test_client);

  // Everything is set up. Trigger the resource loader certificate request event
  // and run the message loop.
  loader_->OnCertificateRequested(raw_ptr_to_request_, cert_request_info.get());
  base::RunLoop().RunUntilIdle();

  // Restore the original content browser client.
  SetBrowserClientForTesting(old_client);

  // Check if the SelectClientCertificate was called on the content browser
  // client.
  EXPECT_EQ(1, test_client.call_count());
  EXPECT_EQ(net::CertificateList(), test_client.passed_certs());
}

TEST_F(ResourceLoaderTest, ClientCertStoreAsyncCancel) {
  // Set up the test client cert store.
  int store_request_count;
  std::vector<std::string> store_requested_authorities;
  scoped_ptr<ClientCertStoreStub> test_store(
      new ClientCertStoreStub(net::CertificateList(), &store_request_count,
                              &store_requested_authorities));
  test_store->set_async(true);
  EXPECT_EQ(0, store_request_count);
  resource_context_.SetClientCertStore(test_store.Pass());

  // Prepare a dummy certificate request.
  scoped_refptr<net::SSLCertRequestInfo> cert_request_info(
      new net::SSLCertRequestInfo());
  std::vector<std::string> dummy_authority(1, "dummy");
  cert_request_info->cert_authorities = dummy_authority;

  // Everything is set up. Trigger the resource loader certificate request
  // event.
  loader_->OnCertificateRequested(raw_ptr_to_request_, cert_request_info.get());

  // Check if the test store was queried against correct |cert_authorities|.
  EXPECT_EQ(1, store_request_count);
  EXPECT_EQ(dummy_authority, store_requested_authorities);

  // Cancel the request before the store calls the callback.
  loader_.reset();

  // Pump the event loop. There shouldn't be a crash when the callback is run.
  base::RunLoop().RunUntilIdle();
}

TEST_F(ResourceLoaderTest, ResumeCancelledRequest) {
  raw_ptr_resource_handler_->set_defer_request_on_will_start(true);

  loader_->StartRequest();
  loader_->CancelRequest(true);
  static_cast<ResourceController*>(loader_.get())->Resume();
}

// Tests that no invariants are broken if a ResourceHandler cancels during
// OnReadCompleted.
TEST_F(ResourceLoaderTest, CancelOnReadCompleted) {
  raw_ptr_resource_handler_->set_cancel_on_read_completed(true);

  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::CANCELED,
            raw_ptr_resource_handler_->status().status());
}

// Tests that no invariants are broken if a ResourceHandler defers EOF.
TEST_F(ResourceLoaderTest, DeferEOF) {
  raw_ptr_resource_handler_->set_defer_eof(true);

  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_FALSE(raw_ptr_resource_handler_->received_response_completed());

  raw_ptr_resource_handler_->Resume();
  base::RunLoop().RunUntilIdle();

  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::SUCCESS,
            raw_ptr_resource_handler_->status().status());
}

class ResourceLoaderRedirectToFileTest : public ResourceLoaderTest {
 public:
  ResourceLoaderRedirectToFileTest()
      : file_stream_(NULL),
        redirect_to_file_resource_handler_(NULL) {
  }

  base::FilePath temp_path() const { return temp_path_; }
  ShareableFileReference* deletable_file() const {
    return deletable_file_.get();
  }
  net::testing::MockFileStream* file_stream() const { return file_stream_; }
  RedirectToFileResourceHandler* redirect_to_file_resource_handler() const {
    return redirect_to_file_resource_handler_;
  }

  void ReleaseLoader() {
    file_stream_ = NULL;
    deletable_file_ = NULL;
    loader_.reset();
  }

  scoped_ptr<ResourceHandler> WrapResourceHandler(
      scoped_ptr<ResourceHandlerStub> leaf_handler,
      net::URLRequest* request) override {
    leaf_handler->set_expect_reads(false);

    // Make a temporary file.
    CHECK(base::CreateTemporaryFile(&temp_path_));
    int flags = base::File::FLAG_WRITE | base::File::FLAG_TEMPORARY |
                base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_ASYNC;
    base::File file(temp_path_, flags);
    CHECK(file.IsValid());

    // Create mock file streams and a ShareableFileReference.
    scoped_ptr<net::testing::MockFileStream> file_stream(
        new net::testing::MockFileStream(file.Pass(),
                                         base::MessageLoopProxy::current()));
    file_stream_ = file_stream.get();
    deletable_file_ = ShareableFileReference::GetOrCreate(
        temp_path_,
        ShareableFileReference::DELETE_ON_FINAL_RELEASE,
        BrowserThread::GetMessageLoopProxyForThread(
            BrowserThread::FILE).get());

    // Inject them into the handler.
    scoped_ptr<RedirectToFileResourceHandler> handler(
        new RedirectToFileResourceHandler(leaf_handler.Pass(), request));
    redirect_to_file_resource_handler_ = handler.get();
    handler->SetCreateTemporaryFileStreamFunctionForTesting(
        base::Bind(&ResourceLoaderRedirectToFileTest::PostCallback,
                   base::Unretained(this),
                   base::Passed(&file_stream)));
    return handler.Pass();
  }

 private:
  void PostCallback(
      scoped_ptr<net::FileStream> file_stream,
      const CreateTemporaryFileStreamCallback& callback) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(callback, base::File::FILE_OK,
                   base::Passed(&file_stream), deletable_file_));
  }

  base::FilePath temp_path_;
  scoped_refptr<ShareableFileReference> deletable_file_;
  // These are owned by the ResourceLoader.
  net::testing::MockFileStream* file_stream_;
  RedirectToFileResourceHandler* redirect_to_file_resource_handler_;
};

// Tests that a RedirectToFileResourceHandler works and forwards everything
// downstream.
TEST_F(ResourceLoaderRedirectToFileTest, Basic) {
  // Run it to completion.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // Check that the handler forwarded all information to the downstream handler.
  EXPECT_EQ(temp_path(),
            raw_ptr_resource_handler_->response()->head.download_file_path);
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::SUCCESS,
            raw_ptr_resource_handler_->status().status());
  EXPECT_EQ(test_data().size(), static_cast<size_t>(
      raw_ptr_resource_handler_->total_bytes_downloaded()));

  // Check that the data was written to the file.
  std::string contents;
  ASSERT_TRUE(base::ReadFileToString(temp_path(), &contents));
  EXPECT_EQ(test_data(), contents);

  // Release the loader and the saved reference to file. The file should be gone
  // now.
  ReleaseLoader();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(base::PathExists(temp_path()));
}

// Tests that RedirectToFileResourceHandler handles errors in creating the
// temporary file.
TEST_F(ResourceLoaderRedirectToFileTest, CreateTemporaryError) {
  // Swap out the create temporary function.
  redirect_to_file_resource_handler()->
      SetCreateTemporaryFileStreamFunctionForTesting(
          base::Bind(&CreateTemporaryError, base::File::FILE_ERROR_FAILED));

  // Run it to completion.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // To downstream, the request was canceled.
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::CANCELED,
            raw_ptr_resource_handler_->status().status());
  EXPECT_EQ(0, raw_ptr_resource_handler_->total_bytes_downloaded());
}

// Tests that RedirectToFileResourceHandler handles synchronous write errors.
TEST_F(ResourceLoaderRedirectToFileTest, WriteError) {
  file_stream()->set_forced_error(net::ERR_FAILED);

  // Run it to completion.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // To downstream, the request was canceled sometime after it started, but
  // before any data was written.
  EXPECT_EQ(temp_path(),
            raw_ptr_resource_handler_->response()->head.download_file_path);
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::CANCELED,
            raw_ptr_resource_handler_->status().status());
  EXPECT_EQ(0, raw_ptr_resource_handler_->total_bytes_downloaded());

  // Release the loader. The file should be gone now.
  ReleaseLoader();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(base::PathExists(temp_path()));
}

// Tests that RedirectToFileResourceHandler handles asynchronous write errors.
TEST_F(ResourceLoaderRedirectToFileTest, WriteErrorAsync) {
  file_stream()->set_forced_error_async(net::ERR_FAILED);

  // Run it to completion.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // To downstream, the request was canceled sometime after it started, but
  // before any data was written.
  EXPECT_EQ(temp_path(),
            raw_ptr_resource_handler_->response()->head.download_file_path);
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::CANCELED,
            raw_ptr_resource_handler_->status().status());
  EXPECT_EQ(0, raw_ptr_resource_handler_->total_bytes_downloaded());

  // Release the loader. The file should be gone now.
  ReleaseLoader();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(base::PathExists(temp_path()));
}

// Tests that RedirectToFileHandler defers completion if there are outstanding
// writes and accounts for errors which occur in that time.
TEST_F(ResourceLoaderRedirectToFileTest, DeferCompletion) {
  // Program the MockFileStream to error asynchronously, but throttle the
  // callback.
  file_stream()->set_forced_error_async(net::ERR_FAILED);
  file_stream()->ThrottleCallbacks();

  // Run it as far as it will go.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // At this point, the request should have completed.
  EXPECT_EQ(net::URLRequestStatus::SUCCESS,
            raw_ptr_to_request_->status().status());

  // However, the resource loader stack is stuck somewhere after receiving the
  // response.
  EXPECT_EQ(temp_path(),
            raw_ptr_resource_handler_->response()->head.download_file_path);
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_FALSE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(0, raw_ptr_resource_handler_->total_bytes_downloaded());

  // Now, release the floodgates.
  file_stream()->ReleaseCallbacks();
  base::RunLoop().RunUntilIdle();

  // Although the URLRequest was successful, the leaf handler sees a failure
  // because the write never completed.
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::CANCELED,
            raw_ptr_resource_handler_->status().status());

  // Release the loader. The file should be gone now.
  ReleaseLoader();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(base::PathExists(temp_path()));
}

// Tests that a RedirectToFileResourceHandler behaves properly when the
// downstream handler defers OnWillStart.
TEST_F(ResourceLoaderRedirectToFileTest, DownstreamDeferStart) {
  // Defer OnWillStart.
  raw_ptr_resource_handler_->set_defer_request_on_will_start(true);

  // Run as far as we'll go.
  loader_->StartRequest();
  base::RunLoop().RunUntilIdle();

  // The request should have stopped at OnWillStart.
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_FALSE(raw_ptr_resource_handler_->response());
  EXPECT_FALSE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(0, raw_ptr_resource_handler_->total_bytes_downloaded());

  // Now resume the request. Now we complete.
  raw_ptr_resource_handler_->Resume();
  base::RunLoop().RunUntilIdle();

  // Check that the handler forwarded all information to the downstream handler.
  EXPECT_EQ(temp_path(),
            raw_ptr_resource_handler_->response()->head.download_file_path);
  EXPECT_EQ(test_url(), raw_ptr_resource_handler_->start_url());
  EXPECT_TRUE(raw_ptr_resource_handler_->received_response_completed());
  EXPECT_EQ(net::URLRequestStatus::SUCCESS,
            raw_ptr_resource_handler_->status().status());
  EXPECT_EQ(test_data().size(), static_cast<size_t>(
      raw_ptr_resource_handler_->total_bytes_downloaded()));

  // Check that the data was written to the file.
  std::string contents;
  ASSERT_TRUE(base::ReadFileToString(temp_path(), &contents));
  EXPECT_EQ(test_data(), contents);

  // Release the loader. The file should be gone now.
  ReleaseLoader();
  base::RunLoop().RunUntilIdle();
  EXPECT_FALSE(base::PathExists(temp_path()));
}

}  // namespace content

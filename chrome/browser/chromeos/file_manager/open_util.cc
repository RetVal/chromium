// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/file_manager/open_util.h"

#include "base/bind.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/chromeos/drive/file_system_util.h"
#include "chrome/browser/chromeos/file_manager/app_id.h"
#include "chrome/browser/chromeos/file_manager/file_tasks.h"
#include "chrome/browser/chromeos/file_manager/fileapi_util.h"
#include "chrome/browser/chromeos/file_manager/path_util.h"
#include "chrome/browser/chromeos/file_manager/url_util.h"
#include "chrome/browser/extensions/api/file_handlers/mime_util.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/simple_message_box.h"
#include "chrome/grit/generated_resources.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/user_metrics.h"
#include "storage/browser/fileapi/file_system_backend.h"
#include "storage/browser/fileapi/file_system_context.h"
#include "storage/browser/fileapi/file_system_operation_runner.h"
#include "storage/browser/fileapi/file_system_url.h"
#include "ui/base/l10n/l10n_util.h"

using content::BrowserThread;
using storage::FileSystemURL;

namespace file_manager {
namespace util {
namespace {

// Shows a warning message box saying that the file could not be opened.
void ShowWarningMessageBox(Profile* profile,
                           const base::FilePath& file_path,
                           int message_id) {
  Browser* browser = chrome::FindTabbedBrowser(
      profile, false, chrome::HOST_DESKTOP_TYPE_ASH);
  chrome::ShowMessageBox(
      browser ? browser->window()->GetNativeWindow() : NULL,
      l10n_util::GetStringFUTF16(
          IDS_FILE_BROWSER_ERROR_VIEWING_FILE_TITLE,
          base::UTF8ToUTF16(file_path.BaseName().AsUTF8Unsafe())),
      l10n_util::GetStringUTF16(message_id),
      chrome::MESSAGE_BOX_TYPE_WARNING);
}

// Executes the |task| for the file specified by |url|.
void ExecuteFileTaskForUrl(Profile* profile,
                           const file_tasks::TaskDescriptor& task,
                           const GURL& url) {
  storage::FileSystemContext* file_system_context =
      GetFileSystemContextForExtensionId(profile, kFileManagerAppId);

  file_tasks::ExecuteFileTask(
      profile,
      GetFileManagerMainPageUrl(), // Executing the task on behalf of Files.app.
      task,
      std::vector<FileSystemURL>(1, file_system_context->CrackURL(url)),
      file_tasks::FileTaskFinishedCallback());
}

// Opens the file manager for the specified |url|. Used to implement
// internal handlers of special action IDs:
//
// "open" - Open the file manager for the given folder.
// "select" - Open the file manager for the given file. The folder containing
//            the file will be opened with the file selected.
void OpenFileManagerWithInternalActionId(Profile* profile,
                                         const GURL& url,
                                         const std::string& action_id) {
  DCHECK(action_id == "open" || action_id == "select");
  content::RecordAction(base::UserMetricsAction("ShowFileBrowserFullTab"));

  file_tasks::TaskDescriptor task(kFileManagerAppId,
                                  file_tasks::TASK_TYPE_FILE_BROWSER_HANDLER,
                                  action_id);
  ExecuteFileTaskForUrl(profile, task, url);
}

// Opens the file with fetched MIME type and calls the callback.
void OpenFileWithMimeType(Profile* profile,
                          const base::FilePath& path,
                          const GURL& url,
                          const base::Callback<void(bool)>& callback,
                          const std::string& mime_type) {
  extensions::app_file_handler_util::PathAndMimeTypeSet path_mime_set;
  path_mime_set.insert(std::make_pair(path, mime_type));

  std::vector<GURL> file_urls;
  file_urls.push_back(url);

  std::vector<file_tasks::FullTaskDescriptor> tasks;
  file_tasks::FindAllTypesOfTasks(
      profile,
      drive::util::GetDriveAppRegistryByProfile(profile),
      path_mime_set,
      file_urls,
      &tasks);

  // Select a default handler. If a default handler is not available, select
  // a non-generic file handler.
  const file_tasks::FullTaskDescriptor* chosen_task = nullptr;
  for (const auto& task : tasks) {
    if (!task.is_generic_file_handler()) {
      chosen_task = &task;
      if (task.is_default())
        break;
    }
  }

  if (chosen_task != nullptr) {
    ExecuteFileTaskForUrl(profile, chosen_task->task_descriptor(), url);
    callback.Run(true);
  } else {
    callback.Run(false);
  }
}

// Opens the file specified by |url| by finding and executing a file task for
// the file. In case of success, calls |callback| with true. Otherwise the
// returned value is false.
void OpenFile(Profile* profile,
              const base::FilePath& path,
              const GURL& url,
              const base::Callback<void(bool)>& callback) {
  extensions::app_file_handler_util::GetMimeTypeForLocalPath(
      profile,
      path,
      base::Bind(&OpenFileWithMimeType, profile, path, url, callback));
}

// Called when execution of ContinueOpenItem() is completed.
void OnContinueOpenItemCompleted(Profile* profile,
                                 const base::FilePath& file_path,
                                 bool result) {
  if (!result) {
    int message;
    if (file_path.Extension() == FILE_PATH_LITERAL(".dmg"))
      message = IDS_FILE_BROWSER_ERROR_VIEWING_FILE_FOR_DMG;
    else if (file_path.Extension() == FILE_PATH_LITERAL(".exe") ||
             file_path.Extension() == FILE_PATH_LITERAL(".msi"))
      message = IDS_FILE_BROWSER_ERROR_VIEWING_FILE_FOR_EXECUTABLE;
    else
      message = IDS_FILE_BROWSER_ERROR_VIEWING_FILE;
    ShowWarningMessageBox(profile, file_path, message);
  }
}

// Used to implement OpenItem().
void ContinueOpenItem(Profile* profile,
                      const base::FilePath& file_path,
                      const GURL& url,
                      base::File::Error error) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (error == base::File::FILE_OK) {
    // A directory exists at |url|. Open it with the file manager.
    OpenFileManagerWithInternalActionId(profile, url, "open");
  } else {
    // |url| should be a file. Open it.
    OpenFile(profile,
             file_path,
             url,
             base::Bind(&OnContinueOpenItemCompleted, profile, file_path));
  }
}

// Converts the |path| passed from external callers to the filesystem URL
// that the file manager can correctly handle.
//
// When conversion fails, it shows a warning dialog UI and returns false.
bool ConvertPath(Profile* profile, const base::FilePath& path, GURL* url) {
  if (!ConvertAbsoluteFilePathToFileSystemUrl(
          profile, path, kFileManagerAppId, url)) {
    ShowWarningMessageBox(profile, path,
                          IDS_FILE_BROWSER_ERROR_UNRESOLVABLE_FILE);
    return false;
  }
  return true;
}

}  // namespace

void OpenItem(Profile* profile, const base::FilePath& file_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  GURL url;
  if (!ConvertPath(profile, file_path, &url))
    return;

  CheckIfDirectoryExists(
      GetFileSystemContextForExtensionId(profile, kFileManagerAppId),
      url,
      base::Bind(&ContinueOpenItem, profile, file_path, url));
}

void ShowItemInFolder(Profile* profile, const base::FilePath& file_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  GURL url;
  if (!ConvertPath(profile, file_path, &url))
    return;

  // This action changes the selection so we do not reuse existing tabs.
  OpenFileManagerWithInternalActionId(profile, url, "select");
}

}  // namespace util
}  // namespace file_manager

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/android/web_contents_observer_android.h"

#include <string>

#include <jni.h>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "content/browser/android/content_view_core_impl.h"
#include "content/browser/renderer_host/render_widget_host_impl.h"
#include "content/browser/web_contents/web_contents_impl.h"
#include "base/android/scoped_java_ref.h"
#include "jni/WebContentsObserverAndroid_jni.h"

using base::android::AttachCurrentThread;
using base::android::ScopedJavaLocalRef;
using base::android::ConvertUTF8ToJavaString;
using base::android::ConvertUTF16ToJavaString;
using base::android::HasClass;

namespace content {

WebContentsObserverAndroid::WebContentsObserverAndroid(
    JNIEnv* env,
    jobject obj,
    WebContents* web_contents)
    : WebContentsObserver(web_contents),
      weak_java_observer_(env, obj){
}

WebContentsObserverAndroid::~WebContentsObserverAndroid() {
}

jint Init(JNIEnv* env, jobject obj, jint native_web_contents) {
  WebContents* web_contents =
      reinterpret_cast<WebContents*>(native_web_contents);
  WebContentsObserverAndroid* native_observer = new WebContentsObserverAndroid(
      env, obj, web_contents);
  return reinterpret_cast<jint>(native_observer);
}

void WebContentsObserverAndroid::Destroy(JNIEnv* env, jobject obj) {
  delete this;
}

void WebContentsObserverAndroid::WebContentsDestroyed(
    WebContents* web_contents) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = weak_java_observer_.get(env);
  if (obj.is_null()) {
    delete this;
  } else {
    // The java side will destroy |this|
    Java_WebContentsObserverAndroid_detachFromWebContents(env, obj.obj());
  }
}

void WebContentsObserverAndroid::DidStartLoading(
    RenderViewHost* render_view_host) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = weak_java_observer_.get(env);
  if (obj.is_null())
    return;
  ScopedJavaLocalRef<jstring> jstring_url =
      ConvertUTF8ToJavaString(env, web_contents()->GetURL().spec());
  Java_WebContentsObserverAndroid_didStartLoading(
      env, obj.obj(), jstring_url.obj());
}

void WebContentsObserverAndroid::DidStopLoading(
    RenderViewHost* render_view_host) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = weak_java_observer_.get(env);
  if (obj.is_null())
    return;
  ScopedJavaLocalRef<jstring> jstring_url =
      ConvertUTF8ToJavaString(env, web_contents()->GetURL().spec());
  Java_WebContentsObserverAndroid_didStopLoading(
      env, obj.obj(), jstring_url.obj());
}

void WebContentsObserverAndroid::DidFailProvisionalLoad(
    int64 frame_id,
    bool is_main_frame,
    const GURL& validated_url,
    int error_code,
    const string16& error_description,
    RenderViewHost* render_view_host) {
  DidFailLoadInternal(
        true, is_main_frame, error_code, error_description, validated_url);
}

void WebContentsObserverAndroid::DidFailLoad(
    int64 frame_id,
    const GURL& validated_url,
    bool is_main_frame,
    int error_code,
    const string16& error_description,
    RenderViewHost* render_view_host) {
  DidFailLoadInternal(
        false, is_main_frame, error_code, error_description, validated_url);
}

void WebContentsObserverAndroid::DidFailLoadInternal(
    bool is_provisional_load,
    bool is_main_frame,
    int error_code,
    const string16& description,
    const GURL& url) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jobject> obj = weak_java_observer_.get(env);
  if (obj.is_null())
    return;
  ScopedJavaLocalRef<jstring> jstring_error_description =
      ConvertUTF16ToJavaString(env, description);
  ScopedJavaLocalRef<jstring> jstring_url =
      ConvertUTF8ToJavaString(env, url.spec());

  Java_WebContentsObserverAndroid_didFailLoad(
      env, obj.obj(),
      is_provisional_load,
      is_main_frame,
      error_code,
      jstring_error_description.obj(), jstring_url.obj());
}

bool RegisterWebContentsObserverAndroid(JNIEnv* env) {
  if (!HasClass(env, kWebContentsObserverAndroidClassPath)) {
    DLOG(ERROR) << "Unable to find class WebContentsObserverAndroid!";
    return false;
  }
  return RegisterNativesImpl(env);
}
}  // namespace content

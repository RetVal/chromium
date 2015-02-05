// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_DEBUG_TRACED_VALUE_H_
#define CC_DEBUG_TRACED_VALUE_H_

namespace base {
namespace trace_event {
class TracedValue;
}

// TODO(ssid): remove these aliases after the tracing clients are moved to the
// new trace_event namespace. See crbug.com/451032. ETA: March 2015.
namespace debug {
using ::base::trace_event::TracedValue;
}
}  // namespace base

namespace cc {

class TracedValue {
 public:
  static void AppendIDRef(const void* id, base::debug::TracedValue* array);
  static void SetIDRef(const void* id,
                       base::debug::TracedValue* dict,
                       const char* name);
  static void MakeDictIntoImplicitSnapshot(base::debug::TracedValue* dict,
                                           const char* object_name,
                                           const void* id);
  static void MakeDictIntoImplicitSnapshotWithCategory(
      const char* category,
      base::debug::TracedValue* dict,
      const char* object_name,
      const void* id);
  static void MakeDictIntoImplicitSnapshotWithCategory(
      const char* category,
      base::debug::TracedValue* dict,
      const char* object_base_type_name,
      const char* object_name,
      const void* id);
};

}  // namespace cc

#endif  // CC_DEBUG_TRACED_VALUE_H_

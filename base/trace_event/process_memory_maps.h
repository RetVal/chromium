// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TRACE_EVENT_PROCESS_MEMORY_MAPS_H_
#define BASE_TRACE_EVENT_PROCESS_MEMORY_MAPS_H_

#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {
namespace trace_event {

class TracedValue;

// Data model for process-wide memory stats.
class BASE_EXPORT ProcessMemoryMaps {
 public:
  struct VMRegion {
    static const uint32 kProtectionFlagsRead;
    static const uint32 kProtectionFlagsWrite;
    static const uint32 kProtectionFlagsExec;

    uint64 start_address;
    uint64 size_in_bytes;
    uint32 protection_flags;
    std::string mapped_file;
    uint64 mapped_file_offset;
    uint64 byte_stats_resident;
    uint64 byte_stats_anonymous;
  };

  ProcessMemoryMaps();
  ~ProcessMemoryMaps();

  void AddVMRegion(const VMRegion& region) { vm_regions_.push_back(region); }
  const std::vector<VMRegion>& vm_regions() const { return vm_regions_; }

  // Called at trace generation time to populate the TracedValue.
  void AsValueInto(TracedValue* value) const;

 private:
  std::vector<VMRegion> vm_regions_;

  DISALLOW_COPY_AND_ASSIGN(ProcessMemoryMaps);
};

}  // namespace trace_event
}  // namespace base

#endif  // BASE_TRACE_EVENT_PROCESS_MEMORY_MAPS_H_

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chrome_gpu_util.h"

#include "base/command_line.h"
#include "base/metrics/field_trial.h"
#include "base/metrics/histogram.h"
#include "base/version.h"
#if defined(OS_WIN)
#include "base/win/windows_version.h"
#endif
#include "chrome/common/chrome_switches.h"
#include "chrome/common/chrome_version_info.h"
#include "content/public/browser/gpu_data_manager.h"
#include "content/public/common/content_constants.h"
#include "content/public/common/content_switches.h"

using content::GpuDataManager;

namespace gpu_util {

void DisableCompositingFieldTrial() {
  base::FieldTrial* trial =
      base::FieldTrialList::Find(content::kGpuCompositingFieldTrialName);
  if (trial)
    trial->Disable();
}

bool ShouldRunCompositingFieldTrial() {
// Enable the field trial only on desktop OS's.
#if !(defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX))
  return false;
#endif

#if defined(OS_CHROMEOS)
  return false;
#endif

#if defined(OS_WIN)
  // Don't run the trial on Windows XP.
  if (base::win::GetVersion() < base::win::VERSION_VISTA)
    return false;
#endif

  // The performance of accelerated compositing is too low with software
  // rendering.
  if (content::GpuDataManager::GetInstance()->ShouldUseSoftwareRendering())
    return false;

  // Don't activate the field trial if force-compositing-mode has been
  // explicitly disabled from the command line.
  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableForceCompositingMode))
    return false;

  return true;
}

// Note: The compositing field trial may be created at startup time via the
// Finch framework. In that case, all the Groups and probability values are
// set before this function is called and any Field Trial setup calls
// made here are simply ignored.
// Early outs from this function intended to bypass activation of the field
// trial must call DisableCompositingFieldTrial() before returning.
void InitializeCompositingFieldTrial() {
  // Early out in configurations that should not run the compositing
  // field trial.
  if (!ShouldRunCompositingFieldTrial()) {
    DisableCompositingFieldTrial();
    return;
  }

  const base::FieldTrial::Probability kDivisor = 3;
  scoped_refptr<base::FieldTrial> trial(
    base::FieldTrialList::FactoryGetFieldTrial(
        content::kGpuCompositingFieldTrialName, kDivisor,
        "disable", 2012, 12, 31, NULL));

  // Produce the same result on every run of this client.
  trial->UseOneTimeRandomization();

  base::FieldTrial::Probability force_compositing_mode_probability = 0;
  base::FieldTrial::Probability threaded_compositing_probability = 0;

   int force_compositing_group = trial->AppendGroup(
      content::kGpuCompositingFieldTrialForceCompositingEnabledName,
      force_compositing_mode_probability);
  int thread_group = trial->AppendGroup(
      content::kGpuCompositingFieldTrialThreadEnabledName,
      threaded_compositing_probability);

  bool force_compositing = (trial->group() == force_compositing_group);
  bool thread = (trial->group() == thread_group);
  UMA_HISTOGRAM_BOOLEAN("GPU.InForceCompositingModeFieldTrial",
                        force_compositing);
  UMA_HISTOGRAM_BOOLEAN("GPU.InCompositorThreadFieldTrial", thread);
}

}  // namespace gpu_util;


# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from profile_creators import cookie_profile_extender
from profile_creators import history_profile_extender
from telemetry.page import profile_creator

class LargeProfileCreator(profile_creator.ProfileCreator):
  """This class creates a large profile by performing a large number of url
  navigations."""
  def Run(self, options):
    extender = history_profile_extender.HistoryProfileExtender()
    extender.Run(options)

    extender = cookie_profile_extender.CookieProfileExtender()
    extender.Run(options)

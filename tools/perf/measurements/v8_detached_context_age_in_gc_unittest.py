# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from measurements import v8_detached_context_age_in_gc
from telemetry.core import wpr_modes
from telemetry.page import page_test
from telemetry.page import page as page_module
from telemetry.results import page_test_results
from telemetry.unittest_util import options_for_unittests
from telemetry.unittest_util import page_test_test_case
from telemetry.value import skip


class FakePage(object):
  def __init__(self, url):
    self.url = url
    self.is_file = url.startswith('file://')


class FakeTab(object):
  def __init__(self, histograms):
    self.histograms = histograms
    self.current_histogram_index = 0

  def EvaluateJavaScript(self, script):
    if 'V8.DetachedContextAgeInGC' in script:
      self.current_histogram_index += 1
      return self.histograms[self.current_histogram_index - 1]
    return '{}'

  def CollectGarbage(self):
    pass


def _MeasureFakePage(histograms):
    results = page_test_results.PageTestResults()
    page = FakePage('file://blank.html')
    tab = FakeTab(histograms)
    metric = v8_detached_context_age_in_gc.V8DetachedContextAgeInGC()
    results.WillRunPage(page)
    metric.DidNavigateToPage(page, tab)
    metric.ValidateAndMeasurePage(page, tab, results)
    results.DidRunPage(page)
    return results


def _ActualValues(results):
  return dict((v.name, v) for v in results.all_page_specific_values)


class SimplePage(page_module.Page):
    def __init__(self, page_set):
      super(SimplePage, self).__init__(
          'file://host.html', page_set, page_set.base_dir)
    def RunPageInteractions(self, action_runner):
      # Reload the page to detach the previous context.
      action_runner.ReloadPage()


class V8DetachedContextAgeInGCTests(page_test_test_case.PageTestTestCase):

  def setUp(self):
    self._options = options_for_unittests.GetCopy()
    self._options.browser_options.wpr_mode = wpr_modes.WPR_OFF

  def testWithNoData(self):
    histograms = [
        """{"count": 0, "buckets": []}""",
        """{"count": 0, "buckets": []}"""
    ]
    results = _MeasureFakePage(histograms)
    actual = _ActualValues(results)
    self.assertTrue('skip' in actual)
    self.assertFalse('V8_DetachedContextAgeInGC' in actual)

  def testWithData(self):
    histograms = [
        """{"count": 3, "buckets": [{"low": 1, "high": 2, "count": 1},
                                    {"low": 2, "high": 3, "count": 2}]}""",
        """{"count": 4, "buckets": [{"low": 1, "high": 2, "count": 2},
                                    {"low": 2, "high": 3, "count": 2}]}""",
    ]
    results = _MeasureFakePage(histograms)
    actual = _ActualValues(results)['V8_DetachedContextAgeInGC']
    self.assertEqual(2, actual.value)
    self.assertEqual('garbage_collections', actual.units)

  def testWithSimplePage(self):
    page_set = self.CreateEmptyPageSet()
    page = SimplePage(page_set)
    page_set.AddUserStory(page)
    metric = v8_detached_context_age_in_gc.V8DetachedContextAgeInGC()
    results = self.RunMeasurement(metric, page_set, options=self._options)
    self.assertEquals(0, len(results.failures))
    actual = _ActualValues(results)['V8_DetachedContextAgeInGC']
    self.assertLessEqual(0, actual.value)
    self.assertEqual('garbage_collections', actual.units)

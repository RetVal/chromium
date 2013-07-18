# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'common',
      'type': 'static_library',
      'variables': {
        'chrome_common_target': 1,
        'enable_wexit_time_destructors': 1,
      },
      'include_dirs': [
          '..',
          '<(SHARED_INTERMEDIATE_DIR)',  # Needed by chrome_content_client.cc.
        ],
      'direct_dependent_settings': {
        'include_dirs': [
          '..',
        ],
      },
      'dependencies': [
        # TODO(gregoryd): chrome_resources and chrome_strings could be
        #  shared with the 64-bit target, but it does not work due to a gyp
        # issue.
        'common_net',
        'common_version',
        'installer_util',
        'metrics_proto',
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/base/base.gyp:base_i18n',
        '<(DEPTH)/base/base.gyp:base_prefs',
        '<(DEPTH)/base/base.gyp:base_static',
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_resources',
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_strings',
        '<(DEPTH)/chrome/chrome_resources.gyp:theme_resources',
        '<(DEPTH)/chrome/common_constants.gyp:common_constants',
        '<(DEPTH)/components/components.gyp:visitedlink_common',
        '<(DEPTH)/content/content.gyp:content_common',
        '<(DEPTH)/net/net.gyp:net',
        '<(DEPTH)/skia/skia.gyp:skia',
        '<(DEPTH)/third_party/icu/icu.gyp:icui18n',
        '<(DEPTH)/third_party/icu/icu.gyp:icuuc',
        '<(DEPTH)/third_party/libxml/libxml.gyp:libxml',
        '<(DEPTH)/third_party/mt19937ar/mt19937ar.gyp:mt19937ar',
        '<(DEPTH)/third_party/sqlite/sqlite.gyp:sqlite',
        '<(DEPTH)/third_party/zlib/zlib.gyp:zip',
        '<(DEPTH)/ui/ui.gyp:ui_resources',
        '<(DEPTH)/url/url.gyp:url_lib',
        '<(DEPTH)/webkit/common/user_agent/webkit_user_agent.gyp:user_agent',
      ],
      'sources': [
        '../apps/app_shim/app_shim_launch.h',
        '../apps/app_shim/app_shim_messages.h',
        '../extensions/common/constants.cc',
        '../extensions/common/constants.h',
        '../extensions/common/crx_file.cc',
        '../extensions/common/crx_file.h',
        '../extensions/common/draggable_region.cc',
        '../extensions/common/draggable_region.h',
        '../extensions/common/error_utils.cc',
        '../extensions/common/error_utils.h',
        '../extensions/common/event_filter.cc',
        '../extensions/common/event_filter.h',
        '../extensions/common/event_filtering_info.cc',
        '../extensions/common/event_filtering_info.h',
        '../extensions/common/event_matcher.cc',
        '../extensions/common/event_matcher.h',
        '../extensions/common/extension_paths.cc',
        '../extensions/common/extension_paths.h',
        '../extensions/common/extension_resource.cc',
        '../extensions/common/extension_resource.h',
        '../extensions/common/id_util.cc',
        '../extensions/common/id_util.h',
        '../extensions/common/install_warning.cc',
        '../extensions/common/install_warning.h',
        '../extensions/common/matcher/regex_set_matcher.cc',
        '../extensions/common/matcher/regex_set_matcher.h',
        '../extensions/common/matcher/string_pattern.cc',
        '../extensions/common/matcher/string_pattern.h',
        '../extensions/common/matcher/substring_set_matcher.cc',
        '../extensions/common/matcher/substring_set_matcher.h',
        '../extensions/common/matcher/url_matcher.cc',
        '../extensions/common/matcher/url_matcher.h',
        '../extensions/common/matcher/url_matcher_constants.cc',
        '../extensions/common/matcher/url_matcher_constants.h',
        '../extensions/common/matcher/url_matcher_factory.cc',
        '../extensions/common/matcher/url_matcher_factory.h',
        '../extensions/common/matcher/url_matcher_helpers.cc',
        '../extensions/common/matcher/url_matcher_helpers.h',
        '../extensions/common/one_shot_event.cc',
        '../extensions/common/one_shot_event.h',
        '../extensions/common/url_pattern.cc',
        '../extensions/common/url_pattern.h',
        '../extensions/common/url_pattern_set.cc',
        '../extensions/common/url_pattern_set.h',
        '../extensions/common/view_type.cc',
        '../extensions/common/view_type.h',
        'common/all_messages.h',
        'common/attrition_experiments.h',
        'common/auto_start_linux.cc',
        'common/auto_start_linux.h',
        'common/autocomplete_match_type.cc',
        'common/autocomplete_match_type.h',
        'common/automation_constants.cc',
        'common/automation_constants.h',
        'common/automation_id.cc',
        'common/automation_id.h',
        'common/automation_messages.cc',
        'common/automation_messages.h',
        'common/automation_messages_internal.h',
        'common/badge_util.cc',
        'common/badge_util.h',
        'common/cancelable_task_tracker.cc',
        'common/cancelable_task_tracker.h',
        'common/child_process_logging.h',
        'common/child_process_logging_mac.mm',
        'common/child_process_logging_posix.cc',
        'common/child_process_logging_win.cc',
        'common/chrome_content_client.cc',
        'common/chrome_content_client.h',
        'common/chrome_content_client_constants.cc',
        'common/chrome_content_client_ios.mm',
        'common/chrome_result_codes.h',
        'common/chrome_sandbox_type_mac.h',
        'common/chrome_utility_messages.h',
        'common/chrome_version_info.cc',
        'common/chrome_version_info_android.cc',
        'common/chrome_version_info_chromeos.cc',
        'common/chrome_version_info_posix.cc',
        'common/chrome_version_info_mac.mm',
        'common/chrome_version_info_win.cc',
        'common/chrome_version_info.h',
        'common/cloud_print/cloud_print_class_mac.h',
        'common/cloud_print/cloud_print_class_mac.mm',
        'common/cloud_print/cloud_print_constants.cc',
        'common/cloud_print/cloud_print_constants.h',
        'common/cloud_print/cloud_print_helpers.cc',
        'common/cloud_print/cloud_print_helpers.h',
        'common/cloud_print/cloud_print_proxy_info.cc',
        'common/cloud_print/cloud_print_proxy_info.h',
        'common/common_message_generator.cc',
        'common/common_message_generator.h',
        'common/common_param_traits.cc',
        'common/common_param_traits.h',
        'common/common_param_traits_macros.h',
        'common/content_settings.cc',
        'common/content_settings.h',
        'common/content_settings_helper.cc',
        'common/content_settings_helper.h',
        'common/content_settings_pattern.cc',
        'common/content_settings_pattern.h',
        'common/content_settings_pattern_parser.cc',
        'common/content_settings_pattern_parser.h',
        'common/content_settings_types.h',
        'common/crash_keys.cc',
        'common/crash_keys.h',
        'common/custom_handlers/protocol_handler.cc',
        'common/custom_handlers/protocol_handler.h',
        'common/descriptors_android.h',
        'common/dump_without_crashing.cc',
        'common/dump_without_crashing.h',
        'common/extensions/api/commands/commands_handler.cc',
        'common/extensions/api/commands/commands_handler.h',
        'common/extensions/api/extension_action/action_info.cc',
        'common/extensions/api/extension_action/action_info.h',
        'common/extensions/api/extension_action/browser_action_handler.cc',
        'common/extensions/api/extension_action/browser_action_handler.h',
        'common/extensions/api/extension_action/page_action_handler.cc',
        'common/extensions/api/extension_action/page_action_handler.h',
        'common/extensions/api/extension_action/script_badge_handler.cc',
        'common/extensions/api/extension_action/script_badge_handler.h',
        'common/extensions/api/extension_api.cc',
        'common/extensions/api/extension_api.h',
        'common/extensions/api/extension_api_stub.cc',
        'common/extensions/api/file_browser_handlers/file_browser_handler.cc',
        'common/extensions/api/file_browser_handlers/file_browser_handler.h',
        'common/extensions/api/file_handlers/file_handlers_parser.cc',
        'common/extensions/api/file_handlers/file_handlers_parser.h',
        'common/extensions/api/i18n/default_locale_handler.cc',
        'common/extensions/api/i18n/default_locale_handler.h',
        'common/extensions/api/identity/oauth2_manifest_handler.cc',
        'common/extensions/api/identity/oauth2_manifest_handler.h',
        'common/extensions/api/input_ime/input_components_handler.cc',
        'common/extensions/api/input_ime/input_components_handler.h',
        'common/extensions/api/managed_mode_private/managed_mode_handler.cc',
        'common/extensions/api/managed_mode_private/managed_mode_handler.h',
        'common/extensions/api/media_galleries_private/media_galleries_handler.h',
        'common/extensions/api/media_galleries_private/media_galleries_handler.cc',
        'common/extensions/api/omnibox/omnibox_handler.cc',
        'common/extensions/api/omnibox/omnibox_handler.h',
        'common/extensions/api/page_launcher/page_launcher_handler.cc',
        'common/extensions/api/page_launcher/page_launcher_handler.h',
        'common/extensions/api/plugins/plugins_handler.cc',
        'common/extensions/api/plugins/plugins_handler.h',
        'common/extensions/api/speech/tts_engine_manifest_handler.cc',
        'common/extensions/api/speech/tts_engine_manifest_handler.h',
        'common/extensions/api/spellcheck/spellcheck_handler.cc',
        'common/extensions/api/spellcheck/spellcheck_handler.h',
        'common/extensions/api/storage/storage_schema_manifest_handler.cc',
        'common/extensions/api/storage/storage_schema_manifest_handler.h',
        'common/extensions/api/system_indicator/system_indicator_handler.cc',
        'common/extensions/api/system_indicator/system_indicator_handler.h',
        'common/extensions/background_info.cc',
        'common/extensions/background_info.h',
        'common/extensions/chrome_manifest_handlers.cc',
        'common/extensions/chrome_manifest_handlers.h',
        'common/extensions/command.cc',
        'common/extensions/command.h',
        'common/extensions/csp_handler.cc',
        'common/extensions/csp_handler.h',
        'common/extensions/csp_validator.cc',
        'common/extensions/csp_validator.h',
        'common/extensions/dom_action_types.h',
        'common/extensions/extension.cc',
        'common/extensions/extension.h',
        'common/extensions/extension_constants.cc',
        'common/extensions/extension_constants.h',
        'common/extensions/extension_file_util.cc',
        'common/extensions/extension_file_util.h',
        'common/extensions/extension_icon_set.cc',
        'common/extensions/extension_icon_set.h',
        'common/extensions/extension_l10n_util.cc',
        'common/extensions/extension_l10n_util.h',
        'common/extensions/extension_manifest_constants.cc',
        'common/extensions/extension_manifest_constants.h',
        'common/extensions/extension_messages.cc',
        'common/extensions/extension_messages.h',
        'common/extensions/extension_process_policy.cc',
        'common/extensions/extension_process_policy.h',
        'common/extensions/extension_set.cc',
        'common/extensions/extension_set.h',
        'common/extensions/feature_switch.cc',
        'common/extensions/feature_switch.h',
        'common/extensions/features/api_feature.cc',
        'common/extensions/features/api_feature.h',
        'common/extensions/features/base_feature_provider.cc',
        'common/extensions/features/base_feature_provider.h',
        'common/extensions/features/complex_feature.cc',
        'common/extensions/features/complex_feature.h',
        'common/extensions/features/feature.cc',
        'common/extensions/features/feature.h',
        'common/extensions/features/feature_provider.h',
        'common/extensions/features/manifest_feature.cc',
        'common/extensions/features/manifest_feature.h',
        'common/extensions/features/permission_feature.cc',
        'common/extensions/features/permission_feature.h',
        'common/extensions/features/simple_feature.cc',
        'common/extensions/features/simple_feature.h',
        'common/extensions/incognito_handler.cc',
        'common/extensions/incognito_handler.h',
        'common/extensions/manifest.cc',
        'common/extensions/manifest.h',
        'common/extensions/manifest_handler.cc',
        'common/extensions/manifest_handler.h',
        'common/extensions/manifest_handler_helpers.cc',
        'common/extensions/manifest_handler_helpers.h',
        'common/extensions/manifest_handlers/app_isolation_info.cc',
        'common/extensions/manifest_handlers/app_isolation_info.h',
        'common/extensions/manifest_handlers/app_launch_info.cc',
        'common/extensions/manifest_handlers/app_launch_info.h',
        'common/extensions/manifest_handlers/content_scripts_handler.cc',
        'common/extensions/manifest_handlers/content_scripts_handler.h',
        'common/extensions/manifest_handlers/externally_connectable.cc',
        'common/extensions/manifest_handlers/externally_connectable.h',
        'common/extensions/manifest_handlers/icons_handler.cc',
        'common/extensions/manifest_handlers/icons_handler.h',
        'common/extensions/manifest_handlers/kiosk_enabled_info.cc',
        'common/extensions/manifest_handlers/kiosk_enabled_info.h',
        'common/extensions/manifest_handlers/nacl_modules_handler.cc',
        'common/extensions/manifest_handlers/nacl_modules_handler.h',
        'common/extensions/manifest_handlers/offline_enabled_info.cc',
        'common/extensions/manifest_handlers/offline_enabled_info.h',
        'common/extensions/manifest_handlers/requirements_handler.cc',
        'common/extensions/manifest_handlers/requirements_handler.h',
        'common/extensions/manifest_handlers/sandboxed_page_info.cc',
        'common/extensions/manifest_handlers/sandboxed_page_info.h',
        'common/extensions/manifest_handlers/shared_module_info.cc',
        'common/extensions/manifest_handlers/shared_module_info.h',
        'common/extensions/manifest_handlers/theme_handler.cc',
        'common/extensions/manifest_handlers/theme_handler.h',
        'common/extensions/manifest_url_handler.cc',
        'common/extensions/manifest_url_handler.h',
        'common/extensions/message_bundle.cc',
        'common/extensions/message_bundle.h',
        'common/extensions/mime_types_handler.cc',
        'common/extensions/mime_types_handler.h',
        'common/extensions/permissions/api_permission.cc',
        'common/extensions/permissions/api_permission.h',
        'common/extensions/permissions/api_permission_set.cc',
        'common/extensions/permissions/api_permission_set.h',
        'common/extensions/permissions/bluetooth_permission.cc',
        'common/extensions/permissions/bluetooth_permission.h',
        'common/extensions/permissions/bluetooth_permission_data.cc',
        'common/extensions/permissions/bluetooth_permission_data.h',
        'common/extensions/permissions/chrome_api_permissions.cc',
        'common/extensions/permissions/chrome_api_permissions.h',
        'common/extensions/permissions/chrome_scheme_hosts.cc',
        'common/extensions/permissions/chrome_scheme_hosts.h',
        'common/extensions/permissions/media_galleries_permission.cc',
        'common/extensions/permissions/media_galleries_permission.h',
        'common/extensions/permissions/media_galleries_permission_data.cc',
        'common/extensions/permissions/media_galleries_permission_data.h',
        'common/extensions/permissions/permission_message.cc',
        'common/extensions/permissions/permission_message.h',
        'common/extensions/permissions/permission_set.cc',
        'common/extensions/permissions/permission_set.h',
        'common/extensions/permissions/permissions_data.cc',
        'common/extensions/permissions/permissions_data.h',
        'common/extensions/permissions/permissions_info.cc',
        'common/extensions/permissions/permissions_info.h',
        'common/extensions/permissions/set_disjunction_permission.h',
        'common/extensions/permissions/socket_permission.cc',
        'common/extensions/permissions/socket_permission.h',
        'common/extensions/permissions/socket_permission_data.cc',
        'common/extensions/permissions/socket_permission_data.h',
        'common/extensions/permissions/usb_device_permission.cc',
        'common/extensions/permissions/usb_device_permission.h',
        'common/extensions/permissions/usb_device_permission_data.cc',
        'common/extensions/permissions/usb_device_permission_data.h',
        'common/extensions/sync_helper.cc',
        'common/extensions/sync_helper.h',
        'common/extensions/update_manifest.cc',
        'common/extensions/update_manifest.h',
        'common/extensions/user_script.cc',
        'common/extensions/user_script.h',
        'common/extensions/value_counter.cc',
        'common/extensions/value_counter.h',
        'common/extensions/web_accessible_resources_handler.cc',
        'common/extensions/web_accessible_resources_handler.h',
        'common/external_ipc_fuzzer.cc',
        'common/external_ipc_fuzzer.h',
        'common/favicon/favicon_types.cc',
        'common/favicon/favicon_types.h',
        'common/favicon/favicon_url_parser.cc',
        'common/favicon/favicon_url_parser.h',
        'common/icon_with_badge_image_source.cc',
        'common/icon_with_badge_image_source.h',
        'common/importer/firefox_importer_utils.cc',
        'common/importer/firefox_importer_utils.h',
        'common/importer/firefox_importer_utils_linux.cc',
        'common/importer/firefox_importer_utils_mac.mm',
        'common/importer/firefox_importer_utils_win.cc',
        'common/importer/ie_importer_test_registry_overrider_win.cc',
        'common/importer/ie_importer_test_registry_overrider_win.h',
        'common/importer/ie_importer_utils_win.cc',
        'common/importer/ie_importer_utils_win.h',
        'common/importer/imported_bookmark_entry.cc',
        'common/importer/imported_bookmark_entry.h',
        'common/importer/imported_favicon_usage.cc',
        'common/importer/imported_favicon_usage.h',
        'common/importer/importer_bridge.cc',
        'common/importer/importer_bridge.h',
        'common/importer/importer_data_types.cc',
        'common/importer/importer_data_types.h',
        'common/importer/importer_type.h',
        'common/importer/importer_url_row.cc',
        'common/importer/importer_url_row.h',
        'common/importer/profile_import_process_messages.cc',
        'common/importer/profile_import_process_messages.h',
        'common/importer/safari_importer_utils.h',
        'common/importer/safari_importer_utils.mm',
        'common/instant_restricted_id_cache.h',
        'common/instant_types.cc',
        'common/instant_types.h',
        'common/json_schema/json_schema_constants.cc',
        'common/json_schema/json_schema_constants.h',
        'common/json_schema/json_schema_validator.cc',
        'common/json_schema/json_schema_validator.h',
        'common/localized_error.cc',
        'common/localized_error.h',
        'common/logging_chrome.cc',
        'common/logging_chrome.h',
        'common/mac/app_mode_common.h',
        'common/mac/app_mode_common.mm',
        'common/mac/cfbundle_blocker.h',
        'common/mac/cfbundle_blocker.mm',
        'common/mac/launchd.h',
        'common/mac/launchd.mm',
        'common/mac/nscoder_util.h',
        'common/mac/nscoder_util.mm',
        'common/mac/objc_method_swizzle.h',
        'common/mac/objc_method_swizzle.mm',
        'common/mac/objc_zombie.h',
        'common/mac/objc_zombie.mm',
        'common/media/webrtc_logging_messages.h',
        'common/metrics/entropy_provider.cc',
        'common/metrics/entropy_provider.h',
        'common/metrics/metrics_log_base.cc',
        'common/metrics/metrics_log_base.h',
        'common/metrics/metrics_log_manager.cc',
        'common/metrics/metrics_log_manager.h',
        'common/metrics/metrics_service_base.cc',
        'common/metrics/metrics_service_base.h',
        'common/metrics/metrics_util.cc',
        'common/metrics/metrics_util.h',
        'common/metrics/variations/uniformity_field_trials.cc',
        'common/metrics/variations/uniformity_field_trials.h',
        'common/metrics/variations/variations_util.cc',
        'common/metrics/variations/variations_util.h',
        'common/multi_process_lock.h',
        'common/multi_process_lock_linux.cc',
        'common/multi_process_lock_mac.cc',
        'common/multi_process_lock_win.cc',
        'common/nacl_cmd_line.cc',
        'common/nacl_cmd_line.h',
        'common/nacl_host_messages.h',
        'common/nacl_messages.cc',
        'common/nacl_messages.h',
        'common/nacl_types.cc',
        'common/nacl_types.h',
        'common/omaha_query_params/omaha_query_params.cc',
        'common/omaha_query_params/omaha_query_params.h',
        'common/omnibox_focus_state.h',
        'common/one_click_signin_messages.h',
        'common/partial_circular_buffer.cc',
        'common/partial_circular_buffer.h',
        'common/pepper_flash.cc',
        'common/pepper_flash.h',
        'common/pepper_permission_util.cc',
        'common/pepper_permission_util.h',
        'common/pnacl_types.cc',
        'common/pnacl_types.h',
        'common/policy/policy_schema.cc',
        'common/policy/policy_schema.h',
        'common/pref_names_util.cc',
        'common/pref_names_util.h',
        'common/print_messages.cc',
        'common/print_messages.h',
        'common/profiling.cc',
        'common/profiling.h',
        'common/ref_counted_util.h',
        'common/render_messages.cc',
        'common/render_messages.h',
        'common/safe_browsing/download_protection_util.cc',
        'common/safe_browsing/download_protection_util.h',
        'common/safe_browsing/safebrowsing_messages.h',
        'common/safe_browsing/zip_analyzer.cc',
        'common/safe_browsing/zip_analyzer.h',
        'common/search_provider.h',
        'common/search_types.h',
        'common/service_messages.h',
        'common/service_process_util.cc',
        'common/service_process_util.h',
        'common/service_process_util_linux.cc',
        'common/service_process_util_mac.mm',
        'common/service_process_util_posix.cc',
        'common/service_process_util_posix.h',
        'common/service_process_util_win.cc',
        'common/spellcheck_common.cc',
        'common/spellcheck_common.h',
        'common/spellcheck_marker.h',
        'common/spellcheck_messages.h',
        'common/spellcheck_result.h',
        'common/startup_metric_utils.cc',
        'common/startup_metric_utils.h',
        'common/switch_utils.cc',
        'common/switch_utils.h',
        'common/thumbnail_score.cc',
        'common/thumbnail_score.h',
        'common/time_format.cc',
        'common/time_format.h',
        'common/translate/language_detection_details.cc',
        'common/translate/language_detection_details.h',
        'common/translate/language_detection_util.cc',
        'common/translate/language_detection_util.h',
        'common/translate/translate_common_metrics.cc',
        'common/translate/translate_common_metrics.h',
        'common/translate/translate_errors.h',
        'common/translate/translate_util.cc',
        'common/translate/translate_util.h',
        'common/tts_messages.h',
        'common/tts_utterance_request.cc',
        'common/tts_utterance_request.h',
        'common/url_constants.cc',
        'common/url_constants.h',
        'common/validation_message_messages.h',
        'common/web_application_info.cc',
        'common/web_application_info.h',
        'common/worker_thread_ticker.cc',
        'common/worker_thread_ticker.h',
        '../components/nacl/common/nacl_process_type.h',
      ],
      'conditions': [
        ['enable_extensions==1', {
          'sources!': [
            'common/extensions/api/extension_api_stub.cc',
          ],
          'dependencies': [
            '../device/usb/usb.gyp:device_usb',
          ],
        }, {  # enable_extensions == 0
          'sources/': [
            ['exclude', '^common/extensions/api/'],
            ['include', 'common/extensions/api/extension_api_stub.cc'],
            ['include', 'common/extensions/api/extension_action/action_info.cc'],
            ['include', 'common/extensions/api/extension_action/action_info.h'],
            ['include', 'common/extensions/api/extension_action/browser_action_handler.cc'],
            ['include', 'common/extensions/api/extension_action/browser_action_handler.h'],
            ['include', 'common/extensions/api/extension_action/page_action_handler.cc'],
            ['include', 'common/extensions/api/extension_action/page_action_handler.h'],
            ['include', 'common/extensions/api/i18n/default_locale_handler.cc'],
            ['include', 'common/extensions/api/i18n/default_locale_handler.h'],
            ['include', 'common/extensions/api/identity/oauth2_manifest_handler.cc'],
            ['include', 'common/extensions/api/identity/oauth2_manifest_handler.h'],
            ['include', 'common/extensions/api/managed_mode_private/managed_mode_handler.cc'],
            ['include', 'common/extensions/api/managed_mode_private/managed_mode_handler.h'],
            ['include', 'common/extensions/api/plugins/plugins_handler.cc'],
            ['include', 'common/extensions/api/plugins/plugins_handler.h'],
            ['include', 'common/extensions/api/spellcheck/spellcheck_handler.cc'],
            ['include', 'common/extensions/api/spellcheck/spellcheck_handler.h'],
            ['include', 'common/extensions/api/storage/storage_schema_manifest_handler.cc'],
            ['include', 'common/extensions/api/storage/storage_schema_manifest_handler.h'],
          ],
        }],
        ['OS=="win" or OS=="mac"', {
          'sources': [
            'common/itunes_library.cc',
            'common/itunes_library.h',
            'common/itunes_xml_utils.cc',
            'common/itunes_xml_utils.h',
            'common/media_galleries/picasa_types.cc',
            'common/media_galleries/picasa_types.h',
            'common/media_galleries/pmp_constants.h',
          ],
        }],
        ['OS != "ios"', {
          'dependencies': [
            '<(DEPTH)/chrome/app/policy/cloud_policy_codegen.gyp:policy',
            '<(DEPTH)/chrome/common/extensions/api/api.gyp:api',
            '<(DEPTH)/components/components.gyp:autofill_core_common',
            '<(DEPTH)/ipc/ipc.gyp:ipc',
            '<(DEPTH)/printing/printing.gyp:printing',
            '<(DEPTH)/third_party/adobe/flash/flash_player.gyp:flapper_version_h',
            '<(DEPTH)/third_party/re2/re2.gyp:re2',
            '<(DEPTH)/third_party/widevine/cdm/widevine_cdm.gyp:widevine_cdm_version_h',
            '<(DEPTH)/webkit/support/webkit_support.gyp:glue',
          ],
        }, {  # OS == ios
          'sources/': [
            ['exclude', '^common/child_process_'],
            ['exclude', '^common/chrome_content_client\\.cc$'],
            ['exclude', '^common/chrome_version_info_posix\\.cc$'],
            ['exclude', '^common/common_message_generator\\.cc$'],
            ['exclude', '^common/common_param_traits'],
            ['exclude', '^common/custom_handlers/'],
            ['exclude', '^common/extensions/'],
            ['exclude', '^common/external_ipc_fuzzer\\.'],
            ['exclude', '^common/logging_chrome\\.'],
            ['exclude', '^common/multi_process_'],
            ['exclude', '^common/nacl_'],
            ['exclude', '^common/pepper_flash\\.'],
            ['exclude', '^common/profiling\\.'],
            ['exclude', '^common/service_process_util_'],
            ['exclude', '^common/spellcheck_'],
            ['exclude', '^common/validation_message_'],
            ['exclude', '^common/web_apps\\.'],
            # TODO(ios): Include files here as they are made to work; once
            # everything is online, remove everything below here and just
            # use the exclusions above.
            ['exclude', '\\.(cc|mm)$'],
            ['include', '_ios\\.(cc|mm)$'],
            ['include', '(^|/)ios/'],
            ['include', '^common/chrome_version_info\\.cc$'],
            ['include', '^common/translate'],
            ['include', '^common/zip'],
          ],
          'include_dirs': [
            '<(DEPTH)/breakpad/src',
          ],
        }],
        ['OS=="android"', {
          'sources/': [
            ['exclude', '^common/chrome_version_info_posix.cc'],
            ['exclude', '^common/service_'],
          ],
          'dependencies!': [
            '<(DEPTH)/chrome/app/policy/cloud_policy_codegen.gyp:policy',
          ],
        }],
        ['OS=="win"', {
          'include_dirs': [
            '<(DEPTH)/third_party/wtl/include',
          ],
          'sources!': [
            'common/crash_keys.cc',
            'common/crash_keys.h',
          ],
        }],
        ['enable_mdns == 1', {
            'sources': [
              'common/local_discovery/service_discovery_client.cc',
              'common/local_discovery/service_discovery_client.h',
            ]
        }],
        ['toolkit_uses_gtk == 1', {
          'dependencies': [
            '../build/linux/system.gyp:gtk',
          ],
          'export_dependent_settings': [
            '../third_party/sqlite/sqlite.gyp:sqlite',
          ],
          'link_settings': {
            'libraries': [
              '-lX11',
              '-lXrender',
              '-lXss',
              '-lXext',
            ],
          },
        }],
        ['chromeos==1', {
          'sources!': [
            'common/chrome_version_info_linux.cc',
          ],
        }],
        ['OS=="mac"', {
          'dependencies': [
            '../third_party/mach_override/mach_override.gyp:mach_override',
          ],
          'include_dirs': [
            '<(DEPTH)/breakpad/src',
            '../third_party/GTM',
          ],
          'sources!': [
            'common/child_process_logging_posix.cc',
            'common/chrome_version_info_posix.cc',
          ],
        }],
        ['remoting==1', {
          'dependencies': [
            '../remoting/remoting.gyp:remoting_client_plugin',
          ],
        }],
        ['enable_automation==0', {
          'sources/': [
            ['exclude', '^common/automation_']
          ]
        }],
        ['use_system_nspr==1', {
          'dependencies': [
            '<(DEPTH)/base/third_party/nspr/nspr.gyp:nspr',
          ],
        }],
        ['enable_webrtc==0', {
          'sources!': [
            'common/media/webrtc_logging_messages.h',
          ]
        }],
        ['enable_language_detection==1', {
          'dependencies': [
            '../third_party/cld/cld.gyp:cld',
          ],
        }],
      ],
      'target_conditions': [
        ['OS == "ios"', {
          'sources/': [
            # Pull in specific Mac files for iOS (which have been filtered out
            # by file name rules).
            ['include', '^common/chrome_version_info_mac\\.mm$'],
            ['include', '^common/mac/nscoder_util\\.'],
          ],
        }],
      ],
      'export_dependent_settings': [
        '../base/base.gyp:base',
        'metrics_proto',
      ],
    },
    {
      'target_name': 'common_version',
      'type': 'none',
      'conditions': [
        ['os_posix == 1 and OS != "mac" and OS != "ios"', {
          'direct_dependent_settings': {
            'include_dirs': [
              '<(SHARED_INTERMEDIATE_DIR)',
            ],
          },
          # Because posix_version generates a header, we must set the
          # hard_dependency flag.
          'hard_dependency': 1,
          'actions': [
            {
              'action_name': 'posix_version',
              'variables': {
                'lastchange_path':
                  '<(DEPTH)/build/util/LASTCHANGE',
                'version_py_path': 'tools/build/version.py',
                'version_path': 'VERSION',
                'template_input_path': 'common/chrome_version_info_posix.h.version',
              },
              'conditions': [
                [ 'branding == "Chrome"', {
                  'variables': {
                     'branding_path':
                       'app/theme/google_chrome/BRANDING',
                  },
                }, { # else branding!="Chrome"
                  'variables': {
                     'branding_path':
                       'app/theme/chromium/BRANDING',
                  },
                }],
              ],
              'inputs': [
                '<(template_input_path)',
                '<(version_path)',
                '<(branding_path)',
                '<(lastchange_path)',
              ],
              'outputs': [
                '<(SHARED_INTERMEDIATE_DIR)/chrome/common/chrome_version_info_posix.h',
              ],
              'action': [
                'python',
                '<(version_py_path)',
                '-f', '<(version_path)',
                '-f', '<(branding_path)',
                '-f', '<(lastchange_path)',
                '<(template_input_path)',
                '<@(_outputs)',
              ],
              'message': 'Generating version information',
            },
          ],
        }],
      ],
    },
    {
      'target_name': 'common_net',
      'type': 'static_library',
      'sources': [
        'common/net/net_error_tracker.cc',
        'common/net/net_error_tracker.h',
        'common/net/net_resource_provider.cc',
        'common/net/net_resource_provider.h',
        'common/net/predictor_common.h',
        'common/net/url_util.cc',
        'common/net/url_util.h',
        'common/net/x509_certificate_model.cc',
        'common/net/x509_certificate_model_nss.cc',
        'common/net/x509_certificate_model_openssl.cc',
        'common/net/x509_certificate_model.h',
      ],
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_resources',
        '<(DEPTH)/chrome/chrome_resources.gyp:chrome_strings',
        '<(DEPTH)/crypto/crypto.gyp:crypto',
        '<(DEPTH)/net/net.gyp:net_resources',
        '<(DEPTH)/net/net.gyp:net',
        '<(DEPTH)/third_party/icu/icu.gyp:icui18n',
        '<(DEPTH)/third_party/icu/icu.gyp:icuuc',
      ],
      'conditions': [
        ['OS != "ios"', {
          'dependencies': [
            '<(DEPTH)/gpu/gpu.gyp:gpu_ipc',
          ],
        }, {  # OS == ios
          'sources!': [
            'common/net/net_resource_provider.cc',
            'common/net/x509_certificate_model.cc',
          ],
        }],
        ['os_posix == 1 and OS != "mac" and OS != "ios" and OS != "android"', {
            'dependencies': [
              '../build/linux/system.gyp:ssl',
            ],
          },
        ],
        ['os_posix != 1 or OS == "mac" or OS == "ios"', {
            'sources!': [
              'common/net/x509_certificate_model_nss.cc',
              'common/net/x509_certificate_model_openssl.cc',
            ],
          },
        ],
        ['OS == "android"', {
            'dependencies': [
              '../third_party/openssl/openssl.gyp:openssl',
            ],
          },
        ],
        ['use_openssl==1', {
            'sources!': [
              'common/net/x509_certificate_model_nss.cc',
            ],
          },
          {  # else !use_openssl: remove the unneeded files
            'sources!': [
              'common/net/x509_certificate_model_openssl.cc',
            ],
          },
        ],
        ['OS=="win"', {
            # TODO(jschuh): crbug.com/167187 fix size_t to int truncations.
            'msvs_disabled_warnings': [4267, ],
          },
        ],
      ],
    },
    {
      # Protobuf compiler / generator for the safebrowsing client
      # model proto and the client-side detection (csd) request
      # protocol buffer.
      'target_name': 'safe_browsing_proto',
      'type': 'static_library',
      'sources': [
        'common/safe_browsing/client_model.proto',
        'common/safe_browsing/csd.proto'
      ],
      'variables': {
        'proto_in_dir': 'common/safe_browsing',
        'proto_out_dir': 'chrome/common/safe_browsing',
      },
      'includes': [ '../build/protoc.gypi' ],
    },
    {
      # Protobuf compiler / generator for UMA (User Metrics Analysis).
      'target_name': 'metrics_proto',
      'type': 'static_library',
      'sources': [
        'common/metrics/proto/chrome_experiments.proto',
        'common/metrics/proto/chrome_user_metrics_extension.proto',
        'common/metrics/proto/histogram_event.proto',
        'common/metrics/proto/omnibox_event.proto',
        'common/metrics/proto/perf_data.proto',
        'common/metrics/proto/permuted_entropy_cache.proto',
        'common/metrics/proto/profiler_event.proto',
        'common/metrics/proto/system_profile.proto',
        'common/metrics/proto/user_action_event.proto',
      ],
      'variables': {
        'proto_in_dir': 'common/metrics/proto',
        'proto_out_dir': 'chrome/common/metrics/proto',
      },
      'includes': [ '../build/protoc.gypi' ],
    },
  ],
}

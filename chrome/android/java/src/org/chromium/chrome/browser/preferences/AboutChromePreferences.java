// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.chrome.browser.preferences;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.text.format.DateUtils;
import android.view.ContextThemeWrapper;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.ChromeVersionInfo;
import org.chromium.chrome.browser.preferences.PrefServiceBridge.AboutVersionStrings;
import org.chromium.chrome.browser.preferences.PrefServiceBridge.ProfilePathCallback;
import org.chromium.chrome.browser.util.FeatureUtilities;

import java.util.Calendar;

/**
 * Settings fragment that displays information about Chrome.
 */
public class AboutChromePreferences extends PreferenceFragment implements
        ProfilePathCallback {

    private static final String PREF_APPLICATION_VERSION = "application_version";
    private static final String PREF_OS_VERSION = "os_version";
    private static final String PREF_WEBKIT_VERSION = "webkit_version";
    private static final String PREF_JAVASCRIPT_VERSION = "javascript_version";
    private static final String PREF_EXECUTABLE_PATH = "executable_path";
    private static final String PREF_PROFILE_PATH = "profile_path";
    private static final String PREF_LEGAL_INFORMATION = "legal_information";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getActivity().setTitle(R.string.prefs_about_chrome);
        addPreferencesFromResource(R.xml.about_chrome_preferences);

        if (!FeatureUtilities.isApplicationUpdateSupported()) {
            ChromeBasePreference deprecationWarning = new ChromeBasePreference(
                    new ContextThemeWrapper(getActivity(),
                            R.style.DeprecationWarningPreferenceTheme));
            deprecationWarning.setOrder(-1);
            deprecationWarning.setTitle(R.string.deprecation_warning);
            deprecationWarning.setIcon(R.drawable.deprecation_warning);
            getPreferenceScreen().addPreference(deprecationWarning);
        }

        PrefServiceBridge prefServiceBridge = PrefServiceBridge.getInstance();
        AboutVersionStrings versionStrings = prefServiceBridge.getAboutVersionStrings();
        Preference p = findPreference(PREF_APPLICATION_VERSION);
        p.setSummary(getApplicationVersion(versionStrings.getApplicationVersion()));
        p = findPreference(PREF_OS_VERSION);
        p.setSummary(versionStrings.getOSVersion());
        p = findPreference(PREF_WEBKIT_VERSION);
        p.setSummary(versionStrings.getWebkitVersion());
        p = findPreference(PREF_JAVASCRIPT_VERSION);
        p.setSummary(versionStrings.getJavascriptVersion());
        p = findPreference(PREF_EXECUTABLE_PATH);
        p.setSummary(getActivity().getPackageCodePath());
        p = findPreference(PREF_LEGAL_INFORMATION);
        int currentYear = Calendar.getInstance().get(Calendar.YEAR);
        p.setSummary(getString(R.string.legal_information_summary, currentYear));

        prefServiceBridge.getProfilePath(this);
    }

    private String getApplicationVersion(String version) {
        if (ChromeVersionInfo.isOfficialBuild()) {
            return version;
        }

        // For developer builds, show how recently the app was installed/updated.
        PackageInfo info;
        try {
            info = getActivity().getPackageManager().getPackageInfo(
                    getActivity().getPackageName(), 0);
        } catch (NameNotFoundException e) {
            return version;
        }
        CharSequence updateTimeString = DateUtils.getRelativeTimeSpanString(
                info.lastUpdateTime, System.currentTimeMillis(), 0);
        return getActivity().getString(R.string.version_with_update_time, version,
                updateTimeString);
    }

    @Override
    public void onGotProfilePath(String profilePath) {
        Preference pref = findPreference(PREF_PROFILE_PATH);
        if (pref != null) pref.setSummary(profilePath);
    }
}

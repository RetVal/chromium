#  
#  To use this DEPS file to re-create a Chromium release you
#  need the tools from http://dev.chromium.org installed.
#  
#  This DEPS file corresponds to Chromium 20.0.1088.0
#  
#  
#  
vars =  {'webkit_trunk': 'http://svn.webkit.org/repository/webkit/trunk'} 

deps_os = {
   'win': {
      'src/third_party/yasm/binaries':
      '/trunk/deps/third_party/yasm/binaries@74228',
      'src/third_party/nacl_sdk_binaries':
      '/trunk/deps/third_party/nacl_sdk_binaries@111576',
      'src/third_party/pefile':
      'http://pefile.googlecode.com/svn/trunk@63',
      'src/third_party/swig/win':
      '/trunk/deps/third_party/swig/win@69281',
      'src/rlz':
      'http://rlz.googlecode.com/svn/trunk@123',
      'src/chrome/tools/test/reference_build/chrome_win':
      '/trunk/deps/reference_builds/chrome_win@89574',
      'src/third_party/lighttpd':
      '/trunk/deps/third_party/lighttpd@33727',
      'src/third_party/psyco_win32':
      '/trunk/deps/third_party/psyco_win32@79861',
      'src/third_party/mingw-w64/mingw/bin':
      'http://src.chromium.org/native_client/trunk/deps/third_party/mingw-w64/mingw/bin@7955',
      'src/chrome_frame/tools/test/reference_build/chrome_win':
      '/trunk/deps/reference_builds/chrome_win@89574',
      'src/third_party/xulrunner-sdk':
      '/trunk/deps/third_party/xulrunner-sdk@119756',
      'src/third_party/cygwin':
      '/trunk/deps/third_party/cygwin@66844',
      'src/third_party/python_26':
      '/trunk/tools/third_party/python_26@89111',
      'src/third_party/syzygy/binaries':
      'http://sawbuck.googlecode.com/svn/trunk/syzygy/binaries@674',
      'src/third_party/nss':
      '/trunk/deps/third_party/nss@126189',
   },
   'mac': {
      'src/third_party/lighttpd':
      '/trunk/deps/third_party/lighttpd@33737',
      'src/third_party/GTM':
      'http://google-toolbox-for-mac.googlecode.com/svn/trunk@516',
      'src/third_party/pdfsqueeze':
      'http://pdfsqueeze.googlecode.com/svn/trunk@4',
      'src/chrome/installer/mac/third_party/xz/xz':
      '/trunk/deps/third_party/xz@87706',
      'src/third_party/swig/mac':
      '/trunk/deps/third_party/swig/mac@69281',
      'src/rlz':
      'http://rlz.googlecode.com/svn/trunk@123',
      'src/chrome/tools/test/reference_build/chrome_mac':
      '/trunk/deps/reference_builds/chrome_mac@89574',
      'src/third_party/nss':
      '/trunk/deps/third_party/nss@126189',
   },
   'unix': {
      'src/third_party/gold':
      '/trunk/deps/third_party/gold@124239',
      'src/third_party/swig/linux':
      '/trunk/deps/third_party/swig/linux@69281',
      'src/third_party/xdg-utils':
      '/trunk/deps/third_party/xdg-utils@93299',
      'src/third_party/openssl':
      '/trunk/deps/third_party/openssl@125658',
      'build/xvfb':
      '/trunk/tools/xvfb@121100',
      'src/third_party/cros':
      'http://git.chromium.org/chromiumos/platform/cros.git@377f51d8',
      'build/third_party/xvfb':
      '/trunk/tools/third_party/xvfb@125214',
      'src/third_party/WebKit/Tools/gdb':
      Var("webkit_trunk")[:-6] + '/trunk/Tools/gdb@112724',
      'build/third_party/cbuildbot_chromite':
      'https://git.chromium.org/chromiumos/chromite.git',
      'src/third_party/lss':
      'http://linux-syscall-support.googlecode.com/svn/trunk/lss@9',
      'src/chrome/tools/test/reference_build/chrome_linux':
      '/trunk/deps/reference_builds/chrome_linux@89574',
      'src/third_party/cros_system_api':
      'http://git.chromium.org/chromiumos/platform/system_api.git@70e37fee',
   },
}

deps = {
   'src/third_party/mozc/chrome/chromeos/renderer':
      'http://mozc.googlecode.com/svn/trunk/src/chrome/chromeos/renderer@83',
   'src/content/test/data/layout_tests/LayoutTests/fast/filesystem/resources':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/fast/filesystem/resources@112724',
   'src/native_client':
      'http://src.chromium.org/native_client/trunk/src/native_client@8134',
   'src/third_party/skia/include':
      'http://skia.googlecode.com/svn/trunk/include@3543',
   'build/scripts/private/data/reliability':
      '/trunk/src/chrome/test/data/reliability@109518',
   'src/third_party/flac':
      '/trunk/deps/third_party/flac@120197',
   'chromeos':
      '/trunk/src/tools/cros.DEPS@128845',
   'src/chrome/test/data/perf/frame_rate/content':
      '/trunk/deps/frame_rate/content@93671',
   'src/third_party/ots':
      'http://ots.googlecode.com/svn/trunk@83',
   'src/third_party/sfntly/cpp/src':
      'http://sfntly.googlecode.com/svn/trunk/cpp/src@128',
   'src/third_party/undoview':
      '/trunk/deps/third_party/undoview@119694',
   'src/googleurl':
      'http://google-url.googlecode.com/svn/trunk@169',
   'src/content/test/data/layout_tests/LayoutTests/media':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/media@112724',
   'src/native_client_sdk/src/site_scons':
      'http://src.chromium.org/native_client/trunk/src/native_client/site_scons@8134',
   'src/third_party/webgl_conformance':
      '/trunk/deps/third_party/webgl/sdk/tests@129652',
   'src/third_party/hunspell_dictionaries':
      '/trunk/deps/third_party/hunspell_dictionaries@79099',
   'src/content/test/data/layout_tests/LayoutTests/storage/indexeddb':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/storage/indexeddb@112724',
   'src/content/test/data/layout_tests/LayoutTests/http/tests/appcache':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/http/tests/appcache@112724',
   'build/third_party/lighttpd':
      '/trunk/deps/third_party/lighttpd@58968',
   'src/third_party/angle':
      'http://angleproject.googlecode.com/svn/trunk@1008',
   'src/third_party/skia/third_party/glu':
      'http://skia.googlecode.com/svn/trunk/third_party/glu@3543',
   'src/third_party/scons-2.0.1':
      'http://src.chromium.org/native_client/trunk/src/third_party/scons-2.0.1@7955',
   'src/third_party/webdriver/pylib':
      'http://selenium.googlecode.com/svn/trunk/py@13487',
   'src/tools/gyp':
      'http://gyp.googlecode.com/svn/trunk@1293',
   'src/third_party/libyuv':
      'http://libyuv.googlecode.com/svn/trunk@214',
   'src/third_party/icu':
      '/trunk/deps/third_party/icu46@122842',
   'src/third_party/libphonenumber/src/phonenumbers':
      'http://libphonenumber.googlecode.com/svn/trunk/cpp/src/phonenumbers@425',
   'src/third_party/libphonenumber/src/resources':
      'http://libphonenumber.googlecode.com/svn/trunk/resources@425',
   'src/third_party/safe_browsing/testing':
      'http://google-safe-browsing.googlecode.com/svn/trunk/testing@106',
   'src/third_party/v8-i18n':
      'http://v8-i18n.googlecode.com/svn/trunk@32',
   'src/content/test/data/layout_tests/LayoutTests/http/tests/workers':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/http/tests/workers@112724',
   'src/content/test/data/layout_tests/LayoutTests/fast/workers':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/fast/workers@112724',
   'src/third_party/pylib':
      'http://src.chromium.org/native_client/trunk/src/third_party/pylib@7955',
   'build/third_party/gsutil':
      'http://gsutil.googlecode.com/svn/trunk/src@145',
   'src/third_party/pyftpdlib/src':
      'http://pyftpdlib.googlecode.com/svn/trunk@977',
   'src/third_party/WebKit/LayoutTests':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests@112724',
   'src/content/test/data/layout_tests/LayoutTests/storage/domstorage':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/storage/domstorage@112724',
   'src/third_party/snappy/src':
      'http://snappy.googlecode.com/svn/trunk@37',
   'src/third_party/WebKit':
      '/trunk/deps/third_party/WebKit@76115',
   'src/third_party/WebKit/Tools/DumpRenderTree':
      Var("webkit_trunk")[:-6] + '/trunk/Tools/DumpRenderTree@112724',
   'src/tools/deps2git':
      '/trunk/tools/deps2git@128331',
   'src/third_party/libjpeg_turbo':
      '/trunk/deps/third_party/libjpeg_turbo@126090',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium-win/storage/domstorage':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium-win/storage/domstorage@112724',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium/fast/events':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium/fast/events@112724',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium-win/http/tests/workers':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium-win/http/tests/workers@112724',
   'depot_tools':
      '/trunk/tools/depot_tools@128555',
   'src/third_party/bidichecker':
      'http://bidichecker.googlecode.com/svn/trunk/lib@4',
   'commit-queue':
      '/trunk/tools/commit-queue@129188',
   'src/breakpad/src':
      'http://google-breakpad.googlecode.com/svn/trunk/src@939',
   'src/third_party/WebKit/Source':
      Var("webkit_trunk")[:-6] + '/trunk/Source@112724',
   'src/content/test/data/layout_tests/LayoutTests/http/tests/resources':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/http/tests/resources@112724',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium/fast/workers':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium/fast/workers@112724',
   'src/third_party/jsoncpp/source/src/lib_json':
      'http://jsoncpp.svn.sourceforge.net/svnroot/jsoncpp/trunk/jsoncpp/src/lib_json@248',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium-win/fast/workers':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium-win/fast/workers@112724',
   'build/scripts/gsd_generate_index':
      '/trunk/tools/gsd_generate_index@110568',
   'src/seccompsandbox':
      'http://seccompsandbox.googlecode.com/svn/trunk@178',
   'src/testing/gmock':
      'http://googlemock.googlecode.com/svn/trunk@374',
   'src/third_party/skia/src':
      'http://skia.googlecode.com/svn/trunk/src@3543',
   'src/tools/grit':
      'http://grit-i18n.googlecode.com/svn/trunk@18',
   'src/chrome/test/data/extensions/api_test/permissions/nacl_enabled/bin':
      'http://src.chromium.org/native_client/trunk/src/native_client/tests/prebuilt@8134',
   'src/third_party/smhasher/src':
      'http://smhasher.googlecode.com/svn/trunk@136',
   'src/third_party/webrtc':
      'http://webrtc.googlecode.com/svn/stable/src@1935',
   'src/third_party/ffmpeg':
      '/trunk/deps/third_party/ffmpeg@127595',
   'src/third_party/leveldatabase/src':
      'http://leveldb.googlecode.com/svn/trunk@61',
   'src/third_party/mozc/session':
      'http://mozc.googlecode.com/svn/trunk/src/session@83',
   'build':
      '/trunk/tools/build@130066',
   'src/chrome/test/data/perf/canvas_bench':
      '/trunk/deps/canvas_bench@122605',
   'src/sdch/open-vcdiff':
      'http://open-vcdiff.googlecode.com/svn/trunk@40',
   'src/third_party/speex':
      '/trunk/deps/third_party/speex@111570',
   'src/third_party/cacheinvalidation/files/src/google':
      'http://google-cache-invalidation-api.googlecode.com/svn/trunk/src/google@203',
   'src/tools/page_cycler/acid3':
      '/trunk/deps/page_cycler/acid3@102714',
   'src/build/util/support':
      '/trunk/deps/support@20411',
   'src/content/test/data/layout_tests/LayoutTests/http/tests/xmlhttprequest':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/http/tests/xmlhttprequest@112724',
   'src/third_party/jsoncpp/source/include':
      'http://jsoncpp.svn.sourceforge.net/svnroot/jsoncpp/trunk/jsoncpp/include@248',
   'src/third_party/libphonenumber/src/test':
      'http://libphonenumber.googlecode.com/svn/trunk/cpp/test@425',
   'src/third_party/libsrtp':
      '/trunk/deps/third_party/libsrtp@123853',
   'src/third_party/WebKit/Tools/Scripts':
      Var("webkit_trunk")[:-6] + '/trunk/Tools/Scripts@112724',
   'src/content/test/data/layout_tests/LayoutTests/platform/chromium-win/fast/events':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/platform/chromium-win/fast/events@112724',
   'src/third_party/yasm/source/patched-yasm':
      '/trunk/deps/third_party/yasm/patched-yasm@126079',
   'build/scripts/command_wrapper/bin':
      '/trunk/tools/command_wrapper/bin@111020',
   'src/third_party/libjingle/source':
      'http://libjingle.googlecode.com/svn/trunk@128',
   'src/content/test/data/layout_tests/LayoutTests/fast/events':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/fast/events@112724',
   'src':
      '/trunk/src@130055',
   'src/third_party/pymox/src':
      'http://pymox.googlecode.com/svn/trunk@70',
   'src/content/test/data/layout_tests/LayoutTests/http/tests/websocket/tests':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/http/tests/websocket/tests@112724',
   'src/chrome/browser/resources/software_rendering_list':
      '/trunk/deps/gpu/software_rendering_list@127865',
   'src/third_party/hunspell':
      '/trunk/deps/third_party/hunspell@129758',
   'src/content/test/data/layout_tests/LayoutTests/fast/js/resources':
      Var("webkit_trunk")[:-6] + '/trunk/LayoutTests/fast/js/resources@112724',
   'src/third_party/WebKit/Tools/TestWebKitAPI':
      Var("webkit_trunk")[:-6] + '/trunk/Tools/TestWebKitAPI@112724',
   'src/testing/gtest':
      'http://googletest.googlecode.com/svn/trunk@560',
   'src/v8':
      'http://v8.googlecode.com/svn/trunk@11136',
   'src/third_party/libvpx':
      '/trunk/deps/third_party/libvpx@125647',
   'src/third_party/swig/Lib':
      '/trunk/deps/third_party/swig/Lib@69281',
}

skip_child_includes =  ['breakpad', 'chrome_frame', 'native_client', 'native_client_sdk', 'o3d', 'pdf', 'sdch', 'skia', 'testing', 'third_party', 'v8'] 

hooks =  [{'action': ['python', 'src/build/download_nacl_toolchains.py', '--no-arm-trusted', '--optional-pnacl', '--pnacl-version', '8097', '--file-hash', 'pnacl_linux_x86_32', 'e368da00aa7fd5f1958f8e817c8928df7da70bee', '--file-hash', 'pnacl_linux_x86_64', '8d76fec06ff9975ace6bbbfd01f3ba729c4b9a75', '--file-hash', 'pnacl_translator', 'f9e28462709580ad7333d65c65dc56e40ed8752b', '--file-hash', 'pnacl_mac_x86_32', '4b7e08c62431459244a9e32fc1551918ee96cf34', '--x86-version', '8097', '--file-hash', 'mac_x86_newlib', 'd0a34734e0356d2fd611ab781c05ee994dec9b5e', '--file-hash', 'win_x86_newlib', '6ed8ac51f53cb15328165ff1143d9611c40fc5d8', '--file-hash', 'linux_x86_newlib', '569cfaa5820f18cfe179e5dcb9ef9e03b7d5cff9', '--file-hash', 'mac_x86', 'a8afcd5a55d4499d1df27cba910102ec0413d9a6', '--file-hash', 'win_x86', '253e3ed5fc292205d1953f78c52a72e1d42b9a18', '--file-hash', 'linux_x86', 'ad59800b550fa1c5d0bac15774a82ed78a510827', '--save-downloads-dir', 'src/native_client_sdk/src/build_tools/toolchain_archives', '--keep'], 'pattern': '.'}, {'action': ['python', 'src/tools/clang/scripts/update.py', '--mac-only'], 'pattern': '.'}, {'action': ['python', 'src/build/win/setup_cygwin_mount.py', '--win-only'], 'pattern': '.'}, {'action': ['python', 'src/build/util/lastchange.py', '-o', 'src/build/util/LASTCHANGE'], 'pattern': '.'}, {'action': ['python', 'src/build/gyp_chromium'], 'pattern': '.'}] 

include_rules =  ['+base', '+build', '+ipc', '+unicode', '+testing']
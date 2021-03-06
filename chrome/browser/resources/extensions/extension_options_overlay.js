// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('extensions', function() {
  'use strict';

  /**
   * The ExtensionOptionsOverlay will show an extension's options page using
   * an <extensionoptions> element.
   * @constructor
   */
  function ExtensionOptionsOverlay() {}

  cr.addSingletonGetter(ExtensionOptionsOverlay);

  ExtensionOptionsOverlay.prototype = {
    /**
     * The function that shows the given element in the overlay.
     * @type {?function(HTMLDivElement)} Function that receives the element to
     *     show in the overlay.
     * @private
     */
    showOverlay_: null,

    /**
     * Initialize the page.
     * @param {function(HTMLDivElement)} showOverlay The function to show or
     *     hide the ExtensionOptionsOverlay; this should take a single parameter
     *     which is either the overlay Div if the overlay should be displayed,
     *     or null if the overlay should be hidden.
     */
    initializePage: function(showOverlay) {
      var overlay = $('overlay');

      cr.ui.overlay.setupOverlay(overlay);
      cr.ui.overlay.globalInitialization();
      overlay.addEventListener('cancelOverlay', this.handleDismiss_.bind(this));

      this.showOverlay_ = showOverlay;
    },

    setInitialFocus: function() {
      this.getExtensionOptions_().focus();
    },

    /**
     * @return {?Element}
     * @private
     */
    getExtensionOptions_: function() {
      return $('extension-options-overlay-guest').querySelector(
          'extensionoptions');
    },

    /**
     * Handles a click on the close button.
     * @param {Event} event The click event.
     * @private
     */
    handleDismiss_: function(event) {
      this.setVisible_(false);
      var extensionoptions = this.getExtensionOptions_();
      if (extensionoptions)
        $('extension-options-overlay-guest').removeChild(extensionoptions);

      $('extension-options-overlay-icon').removeAttribute('src');

      // Remove the options query string.
      uber.replaceState({}, '');
    },

    /**
     * Associate an extension with the overlay and display it.
     * @param {string} extensionId The id of the extension whose options page
     *     should be displayed in the overlay.
     * @param {string} extensionName The name of the extension, which is used
     *     as the header of the overlay.
     * @param {string} extensionIcon The URL of the extension's icon.
     * @param {function():void} shownCallback A function called when
     *     showing completes.
     * @suppress {checkTypes}
     * TODO(vitalyp): remove the suppression after adding
     * chrome/renderer/resources/extensions/extension_options.js
     * to dependencies.
     */
    setExtensionAndShowOverlay: function(extensionId,
                                         extensionName,
                                         extensionIcon,
                                         shownCallback) {
      var overlay = $('extension-options-overlay');
      var overlayHeader = $('extension-options-overlay-header');
      var overlayGuest = $('extension-options-overlay-guest');
      var overlayStyle = window.getComputedStyle(overlay);

      $('extension-options-overlay-title').textContent = extensionName;
      $('extension-options-overlay-icon').src = extensionIcon;

      this.setVisible_(true);

      var extensionoptions = new window.ExtensionOptions();
      extensionoptions.extension = extensionId;

      // The <extensionoptions> content's size needs to be restricted to the
      // bounds of the overlay window. The overlay gives a minWidth and
      // maxHeight, but the maxHeight does not include our header height (title
      // and close button), so we need to subtract that to get the maxHeight
      // for the extension options.
      var maxHeight = parseInt(overlay.style.maxHeight, 10) -
                      overlayHeader.offsetHeight;
      var minWidth = parseInt(overlayStyle.minWidth, 10);

      extensionoptions.onclose = function() {
        cr.dispatchSimpleEvent($('overlay'), 'cancelOverlay');
      }.bind(this);

      // Track the current animation (used to grow/shrink the overlay content),
      // if any. Preferred size changes can fire more rapidly than the
      // animation speed, and multiple animations running at the same time has
      // undesirable effects.
      var animation = null;

      /**
       * Resize the overlay if the <extensionoptions> changes preferred size.
       * @param {{width: number, height: number}} evt
       */
      extensionoptions.onpreferredsizechanged = function(evt) {
        var oldWidth = parseInt(overlayStyle.width, 10);
        var oldHeight = parseInt(overlayStyle.height, 10);
        // The overlay must be slightly larger than the extension options to
        // avoid creating scrollbars.
        // TODO(paulmeyer): This shouldn't be necessary, but the preferred size
        // (coming from Blink) seems to be too small for some zoom levels. The
        // 2-pixel addition should be removed once this problem is investigated
        // and corrected.
        var newWidth = Math.max(evt.width + 2, minWidth);
        var newHeight = Math.min(evt.height + 2, maxHeight);

        // animationTime is the amount of time in ms that will be used to resize
        // the overlay. It is calculated by multiplying the pythagorean distance
        // between old and the new size (in px) with a constant speed of
        // 0.25 ms/px.
        var loading = document.documentElement.classList.contains('loading');
        var animationTime = loading ? 0 :
            0.25 * Math.sqrt(Math.pow(newWidth - oldWidth, 2) +
                             Math.pow(newHeight - oldHeight, 2));

        if (animation)
          animation.cancel();

        animation = overlay.animate([
          {width: oldWidth + 'px', height: oldHeight + 'px'},
          {width: newWidth + 'px', height: newHeight + 'px'}
        ], {
          duration: animationTime,
          delay: 0
        });

        animation.onfinish = function(e) {
          animation = null;

          // The <extensionoptions> element is ready to place back in the
          // overlay. Make sure that it's sized to take up the full width/height
          // of the overlay.
          overlayGuest.style.position = '';
          overlayGuest.style.left = '';
          overlayGuest.style.width = newWidth + 'px';
          overlayGuest.style.height = newHeight + 'px';

          if (shownCallback) {
            shownCallback();
            shownCallback = null;
          }
        };
      }.bind(this);

      // Move the <extensionoptions> off screen until the overlay is ready.
      overlayGuest.style.position = 'fixed';
      overlayGuest.style.left = window.outerWidth + 'px';
      // Begin rendering at the default dimensions. This is also necessary to
      // cancel any width/height set on a previous render.
      // TODO(kalman): This causes a visual jag where the overlay guest shows
      // up briefly. It would be better to render this off-screen first, then
      // swap it in place. See crbug.com/408274.
      // This may also solve crbug.com/431001 (width is 0 on initial render).
      overlayGuest.style.width = '';
      overlayGuest.style.height = '';

      overlayGuest.appendChild(extensionoptions);
    },

    /**
     * Toggles the visibility of the ExtensionOptionsOverlay.
     * @param {boolean} isVisible Whether the overlay should be visible.
     * @private
     */
    setVisible_: function(isVisible) {
      this.showOverlay_(isVisible ?
          /** @type {HTMLDivElement} */($('extension-options-overlay')) :
          null);
    }
  };

  // Export
  return {
    ExtensionOptionsOverlay: ExtensionOptionsOverlay
  };
});

/**
 * @file
 * JavaScript file for the AOS module.
 */

(function($){
  Drupal.behaviors.aos = {
    attach: function (context) {
      AOS.init();
    }
  };
})(jQuery);

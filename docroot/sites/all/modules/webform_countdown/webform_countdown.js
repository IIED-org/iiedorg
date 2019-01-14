(function ($) {
  Drupal.behaviors.webformCountdown = {
    attach: function (context, settings) {
      $.each(Drupal.settings.webformCountdown, function() {
        $('textarea[name$="[' + this.key + ']"]').once(this.key).counter({
          type: this.type,
          goal: this.max,
          msg: this.message,
          append: false 
        });
      });
    }
  };
})(jQuery);
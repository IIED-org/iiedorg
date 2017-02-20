(function ($) {
    Drupal.behaviors.paragraphAccordions = {
        attach: function (context) {
            $(function () {
                $( ".paragraphs-item-accordion" ).accordion({ active: false, collapsible: true, icons: false, heightStyle: "content", "header": "h3"});
            });
        }
    }
})(jQuery);
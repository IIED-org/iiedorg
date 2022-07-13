(function ($) {
  "use strict";
  Drupal.behaviors.custom_pub_admin = {
    attach: function (context) {
      var toggleButtonLabels = {
        edit: Drupal.t('Edit'),
        close: Drupal.t('Close')
      };
      $("td.custom_pub-option-edit-cell", context).html('<a href="#">' + toggleButtonLabels.edit + '</a>').css("text-align", "right");
      $("tr.custom_pub-form-edit", context).hide();
      $("td.custom_pub-option-edit-cell > a", context).bind('click', function () {
        var $this = $(this);
        var opt = [];
        var txt = $this.text();
        opt[toggleButtonLabels.edit] = toggleButtonLabels.close;
        opt[toggleButtonLabels.close] = toggleButtonLabels.edit;
        $this.parents('tr.custom_pub-option-row').next("tr.custom_pub-form-edit").toggle();
        $this.text(opt[txt]);
        return false;
      });
      $("th.close-custom-pub-table", context).html('<a>X</a>').css('text-align', 'right');
      $("th.close-custom-pub-table > a", context).css('cursor', 'pointer').click(function () {
        var $this = $(this);
        var $row = $this.parents("tr.custom_pub-form-edit");
        var $link = $row.prev('tr.custom_pub-option-row').find("td.custom_pub-option-edit-cell > a");
        $link.click();
        return false;
      });
    }
  };
})(jQuery);

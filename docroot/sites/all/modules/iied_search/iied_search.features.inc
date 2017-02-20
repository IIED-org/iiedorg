<?php
/**
 * @file
 * iied_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function iied_search_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_gd_infinite_scroll_settings_info().
 */
function iied_search_gd_infinite_scroll_settings_info() {
  $items = array(
    'apachesolr_scroll' => array(
      'pager_selector' => 'div.item-list > ul.pager',
      'next_selector' => 'li.pager-next a:first',
      'content_selector' => 'div.block-system-main div.block-inner',
      'items_selector' => 'div.content.clearfix',
      'load_more' => 1,
      'load_more_markup' => '<button class="load-more">@label</button>',
      'ajax_pager' => 0,
      'database_override' => TRUE,
      'url' => 'search/site/*',
    ),
  );
  return $items;
}

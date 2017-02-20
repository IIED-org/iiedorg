<?php
/**
 * @file
 * paragraphs_accordion.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function paragraphs_accordion_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function paragraphs_accordion_node_info() {
  $items = array(
    'page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('The basic ‘page’ content type is generally used for creating static pages (more permanent site pages). These are used for one off pages, for example an about page, or an ‘information for’ page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_paragraphs_info().
 */
function paragraphs_accordion_paragraphs_info() {
  $items = array(
    'accordion' => array(
      'name' => 'Accordion',
      'bundle' => 'accordion',
      'locked' => '1',
    ),
    'accordion_item' => array(
      'name' => 'Accordion Item',
      'bundle' => 'accordion_item',
      'locked' => '1',
    ),
  );
  return $items;
}

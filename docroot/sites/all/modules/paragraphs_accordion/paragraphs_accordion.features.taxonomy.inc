<?php
/**
 * @file
 * paragraphs_accordion.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function paragraphs_accordion_taxonomy_default_vocabularies() {
  return array(
    'group' => array(
      'name' => 'Collection',
      'machine_name' => 'group',
      'description' => 'Collections of pages (subset of theme)',
      'hierarchy' => 1,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'vocabulary_15' => array(
      'name' => 'Tag',
      'machine_name' => 'vocabulary_15',
      'description' => 'Globally used vocabulary to group content by tag',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'vocabulary_19' => array(
      'name' => 'Theme',
      'machine_name' => 'vocabulary_19',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -1,
    ),
  );
}

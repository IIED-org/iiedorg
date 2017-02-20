<?php
/**
 * @file
 * paragraphs_accordion.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function paragraphs_accordion_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_admin|taxonomy_term|group|form';
  $field_group->group_name = 'group_admin';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'group';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Admin',
    'weight' => '13',
    'children' => array(
      0 => 'field_main_image',
      1 => 'metatags',
      2 => 'path',
      3 => 'rabbit_hole',
      4 => 'redirect',
      5 => 'xmlsitemap',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Admin',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-admin field-group-fieldset',
        'description' => '<hr />',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_admin|taxonomy_term|group|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_container_first|taxonomy_term|vocabulary_19|form';
  $field_group->group_name = 'group_container_first';
  $field_group->entity_type = 'taxonomy_term';
  $field_group->bundle = 'vocabulary_19';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Container first',
    'weight' => '4',
    'children' => array(
      0 => 'field_collection_first',
      1 => 'field_collection_second',
      2 => 'field_collection_third',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-container-first field-group-tab',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_container_first|taxonomy_term|vocabulary_19|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin');
  t('Container first');

  return $field_groups;
}

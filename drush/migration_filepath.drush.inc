<?php

/**
 * @file
 * Defines drush command to rname files.
 */

/**
 * Implements hook_drush_command().
 */
function migration_filepath_drush_command() {
  $items = array();

  $items['migration_filepath'] = array(
    'description' => "Move files to subdirectories",
    'callback' => 'drush_migration_filepath',
    // We are using options rather than arguments so the source can be omitted
    // if needed (e.g. files are just directly under sites/default/files).
    'options' => array(
      'source_dir' => array(
        'description' => 'The directory under files where the to be relocated files are.',
        'example_value' => 'field/image',
      ),
      'target_dir' => array(
        'description' => 'The target directory where the files will be moved to and restructured.',
        'example_value' => 'images',
      ),
      'file_type' => array(
        'description' => 'The type of files to migrate.',
        'example_value' => 'doc',
      ),
    ),
    'examples' => array(
      'drush migration_filepath' => 'Moves files located in the public file directory to YYYY/MM/DD style subdirectory.',
    ),
    'aliases' => array('mf'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL
  );

  return $items;
}

/**
* Command callback to perform the file migration.
*
* The three optional options are the source and target directories and the type of files to move. By default
* only images are removed from the default path, e.g. sites/default/files.
*/
function drush_migration_filepath() {
  // Source directory to be cleaned up. All images in this directory will be relocated.
  $source_directory = rtrim(drush_get_option('source_dir', ''), '/');
  // Directory to place the new structure under. If does not exist will be created.
  $target_directory = rtrim(drush_get_option('target_dir', ''), '/');

  // Regular expression to find files in the source directory.
  // For now assume public files only.
  // public://field/image/imagefield_hENvtS.png
  $file_type = drush_get_option('file_type');
  switch ($file_type) {
    case "doc":
    case "ms":
      $extensions = array('doc', 'docx', 'xls', 'xlsx', 'ppt', 'pptx');
      $filetypes = array('application/msword', 'application/vnd.openxmlformats-officedocument.wordprocessingml.document', 'application/msexcel', 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet', 'application/mspowerpoint', 'application/vnd.openxmlformats-officedocument.presentationml.presentation');
      break;
    case "pdf":
      $extensions = array('pdf');
      $filetypes = array('application/pdf', 'text/pdf');
      break;
    case "img":
    default:
      $extensions = array('jpeg', 'jpg', 'gif', 'png', 'svg');
      $filetypes = array('image/jpeg', 'image/jpg', 'image/gif', 'image/png', 'image/svg');
  }

 // $tokens = array('@ext' => $extensions, '@filetypes' => $filetypes);
 // drush_print(dt('Extensions: @ext, filetypes: @filetypes', $tokens));

  // Construct a expression to find images located in the source directory.
  $file_pattern = "[^\/]*"; // Finds anything that does not contain "/", should be fine.

  // Append the trailing slash for the regular expression.
  // Note, in some instances drupal places under public:/// (three slashes)
  // even when no folder specified. Reason for this is not known yet.
  $source_pattern = $source_directory ? $source_directory . "\/" : '';

  $regex = "^public:\/\/" . $source_pattern . "(" . $file_pattern . ")\.(" . implode($extensions, '|') . ")$";

  // Query the database for files that match this pattern.

  $query = db_select('file_managed', 'f')
    ->condition('filemime', $filetypes , 'IN')
    ->condition('uri', $regex, 'REGEXP');
  //$query->condition('f.fid', '58776');
  $total_count = $query->countQuery()->execute()->fetchField();

  drush_print(dt('@count entries are to be moved.', array('@count' => $total_count)));

  // Select the files to be moved.
  $files = $query->fields('f', array('fid', 'filename', 'uri', 'timestamp'))
    ->execute()
    ->fetchAll();

  $count = 1;
  foreach ($files as $file) {
    preg_match_all("/$regex/i", $file->uri, $matches); // Note, $file->filename can be the SAME for different uri-s!
    $filename = $matches[1][0] . "." . $matches[2][0];

    $old_file_wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);

    // If the file has already been moved, or does not exist in the filesystem, move on.
    if (FALSE === ($status = $old_file_wrapper->url_stat($file->uri, STREAM_URL_STAT_QUIET))) {
      drush_log("File entry in the database does not exist on the filesystem.", 'notice');
      continue;
    }

    // Each file should go to the directory based on its timestamp.
    $target_directory_for_file = $target_directory . '/' . generate_directory_name($file->timestamp);
    // Trim off the starting backslash, if present.
    $target_directory_for_file = ltrim($target_directory_for_file, "/");

    // Construct a dummy URI for it so we can use the stream wrappers.
    $new_directory = file_build_uri($target_directory_for_file);
    $wrapper = file_stream_wrapper_get_instance_by_uri($new_directory);
    // Make sure that the new directory exists.
    $wrapper->mkdir($new_directory, 0755, TRUE);

    // Construct the new directory.
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $new_uri = file_build_uri($target_directory_for_file . '/' . $filename);
    $wrapper->rename($file->uri, $new_uri);

    $progress = round(($count / $total_count) * 100);
    drush_print($progress . '%');
    $query = db_update('file_managed')
      ->fields(array('uri' => $new_uri))
      ->condition('fid', $file->fid)
      ->execute();

    // There are links to some files directly, like:
    // sites/default/files/misty-forest_0.jpg
    //
    // There are also links to image style files, like:
    // sites/default/files/styles/300x150/public/misty-forest_0.jpg
    //
    // We will want to replace links to both.

    // The rich text fields with links to the above patterns are as follows.
    $fields_to_update = [
      'block_custom' => 'body',
      'field_data_body' => 'body_value',
      'field_data_field_additional_resources' => 'field_additional_resources_value',
      'field_data_field_basic_text' => 'field_basic_text_value',
      'field_data_field_contact_0' => 'field_contact_0_value',
      'field_data_field_description' => 'field_description_value',
      'field_data_field_downloads' => 'field_downloads_value',
      'field_data_field_funded_by' => 'field_funded_by_value',
      'field_data_field_glossary' => 'field_glossary_value',
      'field_data_field_howtoapply' => 'field_howtoapply_value',
      'field_data_field_links' => 'field_links_value',
      'field_data_field_overlay_text' => 'field_overlay_text_value',
      'field_data_field_series_badge' => 'field_series_badge_value',
      'field_data_field_source' => 'field_source_value',
    ];
    foreach ($fields_to_update as $table_name => $column_name) {
      // Replace references to links to files in the sites/default/files/ .
      $old_basic_path = 'sites/default/files/' . $filename;
      $new_basic_path = 'sites/default/files/' . $target_directory_for_file . '/' . $filename;
      $replace_basic = db_update($table_name)
        ->expression(
          $column_name,
          'replace(' . $column_name . ', :old, :new)',
          [':old' => $old_basic_path, ':new' =>  $new_basic_path]
        )
        ->execute();
      if ($replace_basic) {
        $log_message = dt(
          "Replaced @count link(s) to @old_path with @new_path in @table",
          array(
            '@count' => $replace_basic,
            '@old_path' => $old_basic_path,
            '@new_path' => $new_basic_path,
            '@table' => $table_name,
          )
        );        $replacement_log[] = $log_message;
        drush_print($log_message);
      }

      // Replace references to this file in the field_data_body field.
      $old_styles_path = 'public/' . $filename;
      $new_styles_path = 'public/' . $target_directory_for_file . '/' . $filename;
      $replace_styles = db_update($table_name)
        ->expression(
          $column_name,
          'replace(' . $column_name . ', :old, :new)',
          [':old' => $old_styles_path, ':new' =>  $new_styles_path]
        )
        ->execute();
      if ($replace_styles) {
        $log_message = dt("Replaced @count link(s) to @old_path with @new_path in @table",
        array(
          '@count' => $replace_styles,
          '@old_path' => $old_styles_path,
          '@new_path' => $new_styles_path,
          '@table' => $table_name,
        ));
        $replacement_log[] = $log_message;
        drush_print($log_message);
      }

      // Replace https://www.iied.org/sites/default/files/ with /sites/default/files/
      $old_https_path = 'https://www.iied.org/sites/default/files/';
      $new_https_path = '/sites/default/files/';
      $replace_https_links = db_update($table_name)
      ->expression(
        $column_name,
        'replace(' . $column_name . ', :old, :new)',
        [':old' => $old_https_path, ':new' =>  $new_https_path]
      )
      ->execute();
      if ($replace_https_links) {
        $log_message = dt("Replaced @count link(s) to @old_path with @new_path in @table",
        array(
          '@count' => $replace_https_links,
          '@old_path' => $old_https_path,
          '@new_path' => $new_https_path,
          '@table' => $table_name,
        ));
        $replacement_log[] = $log_message;
        drush_print($log_message);
      }
      // Replace  http://www.iied.org/sites/default/files/ with /sites/default/files/
      $old_http_path = 'http://www.iied.org/sites/default/files/';
      $new_http_path = '/sites/default/files/';
      $replace_http_links = db_update($table_name)
      ->expression(
        $column_name,
        'replace(' . $column_name . ', :old, :new)',
        [':old' => $old_http_path, ':new' =>  $new_http_path]
      )
      ->execute();
      if ($replace_http_links) {
        $log_message = dt(
          "Replaced @count link(s) to @old_path with @new_path in @table",
          array(
            '@count' => $replace_http_links,
            '@old_path' => $old_http_path,
            '@new_path' => $new_http_path,
            '@table' => $table_name,
          )
        );        $replacement_log[] = $log_message;
        drush_print($log_message);
      }
      // @todo - add block_custom to the target tables for replace query.
    }

    $count++;
  }
  // Print a summary of all the replacements.
  foreach ($replacement_log as $message) {
    drush_print($message);
  }
}

/**
* Generate the new file structure based on the timestamp.
*/
function generate_directory_name($timestamp) {
  $year = date('Y', $timestamp);
  $month = date('m', $timestamp);
  $day = date('d', $timestamp);
  return $year . '/' .$month . '/' . $day;
}
?>

<?php

/**
 * List all the infinite scroll settings.
 */
function gd_infinite_scroll_admin_page() {
  $content = array();

  $content[] = array(
    '#markup' => l(t('Add an infinite scroll'), 'admin/config/user-interface/gd-infinite-scroll/add', array('query' => drupal_get_destination())),
  );
  $settings = gd_infinite_scroll_settings();
  $header = array(t('Machine name'), t('Url'), t('Actions'));

  $rows = array();
  foreach ($settings as $name => $value) {
    $row = array();
    $row[] = $name;
    $row[] = check_plain($value['url']);
    if (isset($value['exported']) && $value['exported'] && (!isset($value['database_override']) || $value['database_override'] == FALSE)) {
      $row[] = 'in code<br>'
      . l(t('Edit'), 'admin/config/user-interface/gd-infinite-scroll/edit/' . $name);
    }
    elseif (isset($value['exported']) && $value['exported'] && isset($value['database_override']) && $value['database_override']) {
      $row[] = 'in code and overrided in database<br>'
      . l(t('Edit'), 'admin/config/user-interface/gd-infinite-scroll/edit/' . $name, array('query' => drupal_get_destination()))
      . '<br>' . l(t('Revert'), 'admin/config/user-interface/gd-infinite-scroll/delete/' . $name, array('query' => drupal_get_destination()));
    }
    else {
      $row[] = 'in database only<br>'
      . l(t('Edit'), 'admin/config/user-interface/gd-infinite-scroll/edit/' . $name, array('query' => drupal_get_destination()))
      . '<br>' . l(t('Delete'), 'admin/config/user-interface/gd-infinite-scroll/delete/' . $name, array('query' => drupal_get_destination()));
    }
    $rows[] = $row;
  }
  $content[] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array(),
    '#sticky' => FALSE,
  );
  return $content;
}

/**
 * Form API: Form builder for adding or editing a settings.
 */
function gd_infinite_scroll_form($form, &$form_state, $op, $settings = array()) {
  // Identifier.
  $form['name'] = array(
    '#title' => t('Machine name'),
    '#type' => 'machine_name',
    '#default_value' => !empty($settings) ? key($settings) : NULL,
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'gd_infinite_scroll_existing_machine_name',
    ),
  );

  if (!empty($settings)) {
    $default_values = reset($settings);
  }

  // URL Pattern where to use this settings.
  $form['url'] = array(
    '#title' => t('Url'),
    '#type' => 'textarea',
    '#default_value' => !empty($default_values['url']) ? $default_values['url'] : NULL,
    '#required' => FALSE,
    '#description' => t('The drupal URL(s) where to apply those settings.  Enter one url per line. The url(s) can contain a placeholder (Example: category/*). The settings will be applyed on page where the url(s) (exact or pattern) match and contain a pager too. To load your settings on drupal frontpage(s) use the placeholder <front>.'),
    '#element_validate' => array('gd_infinite_scroll_validate_url'),
  );

  // Settings :
  $form['pager_selector'] = array(
    '#title' => t('Pager selector'),
    '#type' => 'textfield',
    '#default_value' => !empty($default_values['pager_selector']) ? $default_values['pager_selector'] : 'ul.pager',
    '#required' => TRUE,
    '#description' => t('The jQuery wrapper selector of the pager'),
  );
  $form['next_selector'] = array(
    '#title' => t('Next selector'),
    '#type' => 'textfield',
    '#default_value' => !empty($default_values['next_selector']) ? $default_values['next_selector'] : 'li.pager-next a:first',
    '#required' => TRUE,
    '#description' => t("The jQuery wrapper selector of the next page link's in the pager"),
  );
  $form['content_selector'] = array(
    '#title' => t('Content selector'),
    '#type' => 'textfield',
    '#default_value' => !empty($default_values['content_selector']) ? $default_values['content_selector'] : '.view-content:not(.view-header .view-content, .view-footer .view-content, .view-content .view-content)',
    '#required' => TRUE,
    '#description' => t('The jQuery wrapper selector of the content, where to append the next items'),
  );
  $form['items_selector'] = array(
    '#title' => t('Items selector'),
    '#type' => 'textfield',
    '#default_value' => !empty($default_values['items_selector']) ? $default_values['items_selector'] : '.views-row:not(.views-row .views-row)',
    '#required' => TRUE,
    '#description' => t('The jQuery wrapper selector of the items of the list inside the content selector. Content selector + items selector must be valid.'),
  );

  $form['load_more'] = array(
    '#title' => t('Use a button to load more results instead of autoload on scroll.'),
    '#type' => 'checkbox',
    '#default_value' => !empty($default_values['load_more']) ? $default_values['load_more'] : FALSE,
    '#required' => FALSE,
  );
  $form['load_more_markup'] = array(
    '#title' => t('Load more button markup'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($default_values['load_more_markup']) ? $default_values['load_more_markup'] : '<button class="load-more">@label</button>',
    '#description' => t('The markup for the javascript button "load more". @label will be replaced with the result of t("Load more").'),
    '#states' => array(
      // Hide the settings when the load more checkbox is disabled.
      'invisible' => array(
       ':input[name="load_more"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['ajax_pager'] = array(
    '#title' => t('I just want the pager visible and in ajax (like views ajax).'),
    '#type' => 'checkbox',
    '#default_value' => !empty($default_values['ajax_pager']) ? $default_values['ajax_pager'] : FALSE,
    '#required' => FALSE,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'gd_infinite_scroll_form_submit';
  // By default, render the form using theme_system_settings_form() to accommodate themer.
  $form['#theme'] = 'system_settings_form';

  return $form;
}

/**
 * Form API: validate machine name callback.
 */
function gd_infinite_scroll_existing_machine_name($value, $element, &$form_state) {
  $settings = gd_infinite_scroll_settings_load($value);
  return !empty($settings);
}

/**
 * Form API: Form element validate handler for gd_infinite_scroll_form() url.
 */
function gd_infinite_scroll_validate_url(&$element, &$form_state) {
  if (empty($element['#value'])) {
    form_error($element, t("URL not valid."));
    return;
  }

  // Convert all new lines to Linux.
  $element['#value'] = preg_replace('/\n$/','',preg_replace('/^\n/','',preg_replace('/[\r\n]+/',"\n", $element['#value'])));
  $urls = explode("\n", $element['#value']);

  foreach ($urls as $url) {
    if ($url != '<front>' && !valid_url($url)) {
      form_error($element, t("URL not valid."));
      return;
    }
  }
}

/**
 * Form API: Form submit handler for gd_infinite_scroll_form().
 */
function gd_infinite_scroll_form_submit(&$form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  // Get identifier and URL a part.
  $name = $form_state['values']['name'];
  unset($form_state['values']['name']);
  $url = $form_state['values']['url'];
  unset($form_state['values']['url']);

  // Save all the settings values in an array.
  $settings = array();
  foreach ($form_state['values'] as $key => $value) {
    $settings[$key] = $value;
  }

  // Save or update this settings.
  gd_infinite_scroll_settings_save($name, $url, $settings);

  drupal_set_message(t('Your configuration has been saved.'));
}

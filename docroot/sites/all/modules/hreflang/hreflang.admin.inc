<?php

/**
 * @file
 * Admin page callbacks for Hreflang module.
 */

/**
 * Admin settings form.
 */
function hreflang_admin() {
  $form['hreflang_x_default'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add x-default hreflang tag for default language'),
    '#default_value' => variable_get('hreflang_x_default', FALSE),
    '#description'   => t('If enabled, an additional <a href="https://en.wikipedia.org/wiki/Hreflang#X-Default" rel="noreferrer">@html</a> tag will be created, pointing at the site default language.', array('@html' => 'hreflang="x-default"')),
  );
  return system_settings_form($form);
}

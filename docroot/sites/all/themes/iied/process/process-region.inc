<?php

/**
 * Implements hook_process_region().
 * created cb 2012-01-24 14:24
 * revised cb 2012-01-24 14:31
**/

 function iied_alpha_process_region(&$vars) {
  if (in_array($vars['elements']['#region'], array('content', 'menu', 'branding'))) {
    $theme = alpha_get_theme();

    switch ($vars['elements']['#region']) {
      case 'content':
        $vars['title_prefix'] = $theme->page['title_prefix'];
        $vars['title'] = $theme->page['title'];
        $vars['title_suffix'] = $theme->page['title_suffix'];
        $vars['tabs'] = $theme->page['tabs'];
        $vars['action_links'] = $theme->page['action_links'];
        $vars['title_hidden'] = $theme->page['title_hidden'];
        $vars['feed_icons'] = $theme->page['feed_icons'];
        break;

      case 'menu':
        $vars['main_menu'] = $theme->page['main_menu'];
        $vars['secondary_menu'] = $theme->page['secondary_menu'];
        break;

      case 'branding':
		    $vars['site_name'] = $theme->page['site_name'];
        $vars['linked_site_name'] = l($vars['site_name'], '<front>', array('attributes' => array('rel' => 'home', 'title' => t('Home')), 'html' => TRUE));
        $vars['site_slogan'] = $theme->page['site_slogan'];
        $vars['site_name_hidden'] = $theme->page['site_name_hidden'];
        $vars['site_slogan_hidden'] = $theme->page['site_slogan_hidden'];
        $vars['logo'] = '/sites/all/themes/iied/colours/set_' . date('d') % 4 . '/logo.svg';
		    $vars['logopng'] = '/sites/all/themes/iied/colours/set_' . date('d') % 4 . '/logo.png';
        $vars['logo_img'] = $vars['logo'] ? '<img src="' . $vars['logo'] . '" alt="IIED logo" id="logo" onerror="this.src=' . $vars['logopng'] . '" />' : '';
        $vars['linked_logo_img'] = $vars['logo'] ? l($vars['logo_img'], '<front>', array('attributes' => array('rel' => 'home', 'title' => 'IIED'), 'html' => TRUE)) : '';
        $vars['fiftylogo'] = '/sites/all/themes/iied/colours/set_' . date('d') % 4 . '/50years.svg';
        $vars['fiftylogo_img'] = $vars['fiftylogo'] ? '<img src="' . $vars['fiftylogo'] . '" alt="fifty years" id="fiftylogo"/>' : '';
        $vars['linked_fiftylogo_img'] = l($vars['fiftylogo_img'], '/iied-50-protecting-people-planet-for-half-century', array(
          'attributes' => array('rel' => 'home', 'title' => 'IIED @50'), 'query' => array(
                'utm_source' => 'nav',
                'utm_medium' => 'web'
              ), 'html' => TRUE));
        break;

    }
  }
 }

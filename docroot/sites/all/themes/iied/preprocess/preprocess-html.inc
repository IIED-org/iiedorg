<?php
/** 
 * Implements hook_process_html().
 * created cb 2012-01-24 15:48
**/

function iied_alpha_preprocess_html(&$vars) {
  // custom functionality here
  	$num =  date('d') % 4;
	$vars['stylesdir'] = '/sites/all/themes/iied/colours/';
	$vars['colourdir'] = $vars['stylesdir'] . 'set_' . $num . '/' ;

  /**
   * Loop through to add various size apple icons
   */
  $apple_icon_sizes = array(76,120,152,180);

  foreach($apple_icon_sizes as $size){
    $apple = array(
      '#tag' => 'link',
      '#attributes' => array(
        'href' => '/'.path_to_theme().'/images/apple-icon-'.$size.'x'.$size.'.png',
        'rel' => 'apple-touch-icon',
        'sizes' => $size . 'x' . $size,
      ),
    );
    drupal_add_html_head($apple, 'apple-touch-icon-'.$size);
  }
}

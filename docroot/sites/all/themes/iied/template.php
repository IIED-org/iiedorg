<?php

/**
 * template.php v.3.0 cb 2012-02-02T11:37
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 *
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */

 function iied_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Search'); // Change the text on the label element
    $form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
    $form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/colours/set_' . date('d') % 4 . '/search-button.gif');

    $path_args = arg();

  // Insert the current search term (if applicable).
 $terms = (count($path_args) == 3 && (strpos($path_args[0], 'search') !== false)) ? filter_xss($path_args[2]) : 'Search';
 $form['search_block_form']['#default_value'] = $terms;


// Add extra attributes to the text box
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search') {this.value = '';}";
  }

  if ($form_id == 'custom-search-blocks-form-1') {
     $form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/colours/set_' . date('d') % 4 . '/search-button.gif');
  }

  if ($form_id == 'user_profile_form') {
    unset($form['contact']);
  }
}
/**
* Process variables for search-result.tpl.php.
*
* @see search-result.tpl.php
function iied_preprocess_search_result(&$variables) {
  // Remove user name and modification date from search results
  	$variables['info'] = '';
}
*/

function iied_apachesolr_search_noresults() {
  return t('<ul class="noresults">
<li>Check if your spelling is correct, or try removing filters.</li>
<li>Remove quotes around phrases to match each word individually: <em>"blue drop"</em> will match less than <em>blue drop</em>.</li>
<li>You can require or exclude terms using + and -: <em>big +blue drop</em> will require a match on <em>blue</em> while <em>big blue -drop</em> will exclude results that contain <em>drop</em>.</li>
<li>Try searching our <a href="http://pubs.iied.org">publications library</a>.</li>
</ul>');
}

function iied_preprocess_user_profile(&$variables){
	drupal_add_js('https://rss2json.com/gfapi.js', array('type' => 'external', 'scope' => 'footer'));
	drupal_add_js('sites/all/libraries/iied/pubsrss3.js', array('scope' => 'footer'));
}

function iied_css_alter(&$css) {
  foreach ($css as $key => $value) {
    if (preg_match('/^ie::(\S*)/', $key)) {
      unset($css[$key]);
    }
    else {
      $css[$key]['browsers']['IE'] = TRUE;
    }
  }
}

function iied_facetapi_deactivate_widget($variables) {
  return '<i class="fa fa-times-circle"></i>';
}

function iied_preprocess_page(&$variables) {
	if (arg(0) == 'taxonomy') {
		drupal_add_js('https://rss2json.com/gfapi.js', array('type' => 'external', 'scope' => 'footer'));
		drupal_add_js('sites/all/libraries/iied/pubsrss2.js', array('scope' => 'footer'));
		$variables['scripts'] = drupal_get_js();
	}
}

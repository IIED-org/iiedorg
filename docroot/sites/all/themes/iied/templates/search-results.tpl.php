<?php

/**
 * @file
 * Default theme implementation for displaying search results.
 *
 * This template collects each invocation of theme_search_result(). This and
 * the child template are dependent to one another sharing the markup for
 * definition lists.
 *
 * Note that modules may implement their own search type and theme function
 * completely bypassing this template.
 *
 * Available variables:
 * - $search_results: All results as it is rendered through
 *   search-result.tpl.php
 * - $module: The machine-readable name of the module (tab) being searched, such
 *   as "node" or "user".
 *
 *
 * @see template_preprocess_search_results()
 */
?>
<?php if ($search_results): ?>
  <h2><?php print t('Search results');?></h2>
 <?php global $pager_page_array, $pager_total_items;
	$count = variable_get('apachesolr_rows', 10);
	$start = $pager_page_array[0] * $count + 1;
	print t('Showing %start to %end of %total results found.', 
  	array(
    '%start' => $start,
    '%end' => min($pager_total_items[0], $start + $count - 1),
    '%total' => $pager_total_items[0]
  	)
	); 
?>
  <ol class="search-results <?php print $module; ?>-results">
    <?php print $search_results; ?>
  </ol>
  <?php print $pager; ?>
<?php else : ?>
  <h2><?php print t('Your search yielded no results');?></h2>
  <?php print search_help('search#noresults', drupal_help_arg()); ?>
<?php endif; ?>
<?php if (arg(2)):?>
  	<div class="search-footer"><p><strong>If you are unable to find what you are looking for, please also <a href="http://pubs.iied.org/search.php?k=<?php print(arg(2)); ?>">search our publications database for &quot;<?php print(urldecode(arg(2))); ?>&quot;</a>.</strong></p></div>
<?php else : ?>
   	<div class="search-footer"><p><strong>If you are unable to find what you are looking for, please also search our <a href="http://pubs.iied.org">publications database</a>.</strong></p></div>
<?php endif; ?>

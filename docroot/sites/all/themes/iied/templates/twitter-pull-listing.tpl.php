<?php
// $Id: twitter-pull-listing.tpl.php,v 1.2.2.3 2011/01/11 02:49:27 inadarei Exp $

/**
 * @file
 * Theme template for a list of tweets.
 *
 * Available variables in the theme include:
 *
 * 1) An array of $tweets, where each tweet object has:
 *   $tweet->id
 *   $tweet->username
 *   $tweet->userphoto
 *   $tweet->text
 *   $tweet->timestamp
 *
 * 2) $twitkey string containing initial keyword.
 *
 * 3) $title
 *
 */
?>
<?php if ($lazy_load): ?>
  <?php print $lazy_load; ?>
<?php else: ?>

<div class="tweets-pulled-listing">

  <?php if (!empty($title)): ?>
    <h2><?php print $title; ?></h2>
  <?php endif; ?>

  <?php if (is_array($tweets)): ?>
    <?php $tweet_count = count($tweets); ?>
    
    <ul class="tweets-pulled-listing">
    <?php foreach ($tweets as $tweet_key => $tweet): ?>
      <li>
      <?php if ($tweet->username != "XXXXXX"): ?>
        <div class="tweet-authorphoto"><a href="http://twitter.com/<?php print $tweet->username; ?>" title="<?php print $tweet->username; ?>"><img src="<?php print $tweet->userphoto; ?>" alt="<?php print $tweet->username; ?>" /></a></div>
	  <?php else: ?>
        <div class="tweet-authorphoto"><img src="<?php print base_path() . path_to_theme() . '/images/tweet.png'; ?>" alt="IIED" /></div>
        <?php endif; ?>
        <span class="tweet-author"><?php print l($tweet->username, 'http://twitter.com/' . $tweet->username); ?></span>
        <span class="tweet-text"><?php print twitter_pull_add_links($tweet->text); ?></span>
         <?php if ($tweet->username != "IIED"): ?>
		<span class="retweet"><img src="<?php print base_path() . path_to_theme() . '/images/retweet.png'; ?>" alt="Retweet" /> <?php print l('@' . $tweet->username, 'http://twitter.com/' . $tweet->username); ?> retweeted by <?php print l("@IIED", 'http://twitter.com/IIED'); ?></span>
        <?php endif; ?>
		<?php if ($tweet_key < $tweet_count - 1): ?>
          <div class="tweet-divider"></div>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>

<?php endif; ?>


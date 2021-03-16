<?php

/**
 * @file
 * Theme implementation for bibdk_subject_hierarchy_item.
 *
 * @var $hierarchy
 */
?>

<div class="subjects-sublists">
  <div class="subjects-close-button icon icon-x-altx-alt">&nbsp;</div>
  <div class="subjects-breadcrumb">
    <?php echo drupal_render($breadcrumbs); ?>
  </div>
  <div class="subjects-sublists-heading">
    <?php echo drupal_render($heading); ?>
  </div>
  <div class="subjects-sublist">
    <ul>
    <?php foreach ($hierarchy as $key => $item) : ?>
      <?php echo render($item); ?>
    <?php endforeach; ?>
    </ul>
  </div>
</div>

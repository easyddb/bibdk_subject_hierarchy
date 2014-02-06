<?php

/**
 * @file
 * Theme implementation for bibdk_subject_hierarchy.
 */
?>

<div id="bibdk-subject-hierarchy" class="themes clearfix">
  <div class="container clearfix">
    <div class="bibdk-subject-hierarchy-header">
      <?php print $header; ?>
    </div>
    <?php foreach ($rows as $row) : ?>
      <?php print $row; ?>
    <?php endforeach; ?>
  </div>
</div>


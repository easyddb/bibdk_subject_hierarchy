<?php

/**
 * @file
 * Theme implementation for bibdk_subject_hierarchy.
 */

?>

<div id="bibdk-subject-hierarchy" class="themes clearfix">

  <div class="container clearfix">

    <div class="bibdk-subject-hierarchy-header">
      <?php echo t($variables['header'], array(), array('context' => 'bibdk_subject_hierarchy')); ?>
    </div>

    <div class="themes_row clearfix">

      <?php foreach ($variables['hierarchy'] as $key => $item) {
        $attributes_label['id'] = array('subject-hierarchy-label-link-0-' . $key);
        $attributes_label['class'] = array('use-ajax');
        $attributes_note['id'] = array('subject-hierarchy-note-link-0-' . $key);
        $attributes_note['class'] = array('use-ajax');
        $url = 'bibdk_subject_hierarchy/nojs/' . $key;

        if (in_array($key, array(4, 8))) {
          $row = $key / 4;
          echo '      <div class="themes__sublists__wrapper row-' . $row . '"></div>' . "\n";
          echo '    </div><div class="themes_row  clearfix">' . "\n";
        }
        ?>
        <div class="themes__item item-<?php echo $key; ?>">
          <p>
            <span class="header"><?php echo l($item['ord'], $url, array('attributes' => $attributes_label)); ?></span>
            <span class="note"><?php echo l(t($item['note'], array(), array('context' => 'bibdk_subject_hierarchy')), $url, array('attributes' => $attributes_note)) . "\n"; ?></span>
          </p>
        </div>
      <?php
      }
      ?>
      <div class="themes__sublists__wrapper row-3"></div>

    </div>
    <!-- .themes__row -->
  </div>
</div>


<?php

/**
 * @file
 * Theme implementation for bibdk_subject_hierarchy.
 */

// dpm($variables['hierarchy'][4]);

?>

<div id="bibdk-subject-hierarchy" class="clearfix">

  <h3><?php echo t('LABEL_SUBJECT_HIERARCHY_HEADER', array(), array('context' => 'bibdk_subject_hierarchy')); ?></h3>

<?php foreach ($variables['hierarchy'] as $key => $item) {
  $attributes['id']       = array('bibdk-subject-hierarchy-item-0-' . $key);
  $attributes['class']    = array('bibdk-subject-hierarchy-item');
  $attributes['data-rel'] = 'subject-hierarchy';
  $url = 'search/' . $variables['path'] . '/' . trim($item['cql']);
  $query = NULL;
?>

  <div class="span6 subject-hierarchy-item hierarchy-level0">
    <p class="header"><?php echo l($item['ord'], $url . $query, array('attributes' => $attributes)); ?><p>
    <p class="note"><?php echo t($item['note'], array(), array('context' => 'bibdk_subject_hierarchy')); ?></p>
  </div>

<?php } ?>

</div>
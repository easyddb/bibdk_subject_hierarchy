<?php

/**
 * @file
 * Theme implementation for bibdk_subject_hierarchy.
 */

dpm($variables['hierarchy']);

?>

<div id="bibdk-subject-hierarchy">

  <h3>foobar block</h3>

<?php foreach ($variables['hierarchy'] as $key => $item) {

  $attributes['id'][] = 'bibdk-subject-hierarchy-item-0-' . $key;
  $attributes['data-rel'] = 'subject-hierarchy';
  $attributes['class'][] = 'bibdk-subject-hierarchy-item';
  $url = 'search/' . $variables['path'] . '/' . trim($item['cql']);
  $query = NULL;
?>

  <div class="subject-hierarchy-item hierarchy-level0">

<?php
    echo l($item['ord'], $url . $query, array('attributes' => $attributes));
?>

  </div>

<?php } ?>

</div>
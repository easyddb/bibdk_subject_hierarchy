<?php #dpm($variables); ?>
<div class="themes_row clearfix">

  <?php foreach ($items as $item): ?>
    <?php $index = $item['index']; ?>
    <div class="themes__item item-<?php print $row; ?>">
      <p>
        <span class="header">
          <?php print "<a href='$baseurl/$index' id='subject-hierarchy-label-link-0-$index' class='use-ajax'>" ?>
          <?php print $item['ord']; ?>
          </a>
        </span>

        <span class="note">
          <?php print "<a href='$baseurl/$index' id='subject-hierarchy-note-link-0-$index' class='use-ajax'>" ?>
          <?php print t($item['note'], array(), array('context' => 'bibdk_subject_hierarchy')); ?>
          </a>
        </span>
      </p>
    </div>
  <?php endforeach; ?>

  <?php print "<div class='themes__sublists__wrapper row-$row' style='display: $display'>" . $visible_row . "</div>"; ?>
</div>
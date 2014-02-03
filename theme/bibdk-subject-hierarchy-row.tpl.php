<div class="themes_row clearfix">

  <?php foreach ($items as $item): ?>
    <div class="themes__item item-<?php print $key; ?>">
      <p>
        <span class="header">
          <?php print "<a href='bibdk_subject_hierarchy/nojs/$key' id='subject-hierarchy-label-link-0-$key' class='use-ajax'>" ?>
          <?php print $item['ord']; ?>
          </a>
        </span>

        <span class="note">
          <?php print "<a href='bibdk_subject_hierarchy/nojs/$key' id='subject-hierarchy-note-link-0-$key' class='use-ajax'>" ?>
          <?php print t($item['note'], array(), array('context' => 'bibdk_subject_hierarchy')); ?>
          </a>
        </span>
      </p>
    </div>
  <?php endforeach; ?>

  <?php print "<div class='themes__sublists__wrapper row-$key' style='display: $display'>" . $visible . "</div>"; ?>
</div>
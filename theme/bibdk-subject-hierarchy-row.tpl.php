<?php

/**
 * @file
 * Hierarchy row template.
 *
 * @var array $items
 * @var string $row
 * @var string $display
 * @var string $visible_row
 */
?>
<div class="subjects-row clearfix">
  <?php foreach ($items as $index => $item): ?>
    <div class="<?php print $item['wrapper_classes']; ?>">
      <p>
        <span class="header">
          <a href="<?php print $item['url']; ?>"
             id="<?php print $item['label_id']; ?>" class="use-ajax">
          <?php print $item['ord']; ?>
          </a>
        </span>

        <?php if(isset($item['note'])): ?>
          <span class="note">
            <a href="<?php print $item['url']; ?>"
               id="<?php print $item['note_id']; ?>" class="use-ajax">
            <?php print $item['note']; ?>
            </a>
          </span>
        <?php endif; ?>
      </p>
    </div>
  <?php endforeach; ?>

  <?php print "<div class='subjects-sublist-wrapper row-$row' style='display: $display'>" . $visible_row . "</div>"; ?>
</div>

<?php

/**
 * @file
 * Provides subject hierarchy as a Panels content type.
 */

$plugin = [
  'title' => t('Bibdk subject hierarchy, pane'),
  'single' => TRUE,
  'description' => t('Subject hierarchy, pane'),
  'category' => t('Bibliotek.dk'),
  'edit form' => 'bibdk_subject_hierarchy_subject_hierarchy_content_type_edit_form',
  'render callback' => 'bibdk_subject_hierarchy_subject_hierarchy_content_type_render',
];

/**
 * Return the pane contents.
 */
function bibdk_subject_hierarchy_subject_hierarchy_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->content = bibdk_subject_hierarchy_get_block_content();
  return $block;
}

/**
 * Pane settings form.
 */
function bibdk_subject_hierarchy_subject_hierarchy_content_type_edit_form($form, &$form_state) {
  return $form;
}

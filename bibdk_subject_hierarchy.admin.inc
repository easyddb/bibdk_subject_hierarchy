<?php

/**
 * @file
 * Admin settings for bibdk subject hierarchy.
 */

/**
 * General settings.
 */
function bibdk_subject_hierarchy_admin() {
  $form['bibdk_subject_hierarchy'] = [
    '#type' => 'fieldset',
    '#title' => t('Subject hierarchy suggestions', [], ['context' => 'bibdk_subject_hierarchy']),
    '#description' => t('Edit settings for the subject hierarchy.', [], ['context' => 'bibdk_subject_hierarchy']),
    '#tree' => FALSE,
    '#theme' => 'bibdk_subject_hierarchy-admin-form',
  ];

  $form['bibdk_subject_hierarchy']['bibdk_subject_hierarchy_email_to'] = [
    '#type' => 'textfield',
    '#title' => t('Email address to recieve suggestions', [], ['context' => 'bibdk_subject_hierarchy']),
    '#default_value' => variable_get('bibdk_subject_hierarchy_email_to', NULL),
  ];

  $form['bibdk_subject_hierarchy']['bibdk_subject_hierarchy_email_from'] = [
    '#type' => 'textfield',
    '#title' => t('Return email address on suggestions', [], ['context' => 'bibdk_subject_hierarchy']),
    '#default_value' => variable_get('bibdk_subject_hierarchy_email_from', NULL),
  ];

  $form['bibdk_subject_hierarchy']['bibdk_subject_hierarchy_interval'] = [
    '#type' => 'textfield',
    '#title' => t('Interval for cron.'),
    '#description' => t('Set interval in miliseconds.'),
    '#default_value' => variable_get('bibdk_subject_hierarchy_interval', BIBDK_ONE_WEEK),
  ];

  return system_settings_form($form);
}

/**
 * Validation handler for bibdk_search_controls_admin.
 */
function bibdk_subject_hierarchy_admin_validate($form, $form_state) {
  if (!filter_var($form_state['values']['bibdk_subject_hierarchy_email_to'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('bibdk_subject_hierarchy_email_to', t('Invalid "to" email address.'));
  }
  if (!filter_var($form_state['values']['bibdk_subject_hierarchy_email_from'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('bibdk_subject_hierarchy_email_from', t('Invalid "from" email address.'));
  }
  return $form;
}

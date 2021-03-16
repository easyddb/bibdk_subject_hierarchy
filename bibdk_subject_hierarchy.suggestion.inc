<?php

/**
 * @file
 * Bibdk subject hierarchy autocomplete functions.
 */

/**
 * Create subject hierarchy suggestion form.
 */
function bibdk_subject_hierarchy_suggestion_form($form, &$form_state) {
  if (!isset($form_state['stage'])) {
    $form_state['stage'] = 'suggest_subject';
  }

  $form['#attributes'] = [
    'class' => ['subject-hierarchy-suggestion-form', 'clearfix'],
  ];

  $form['#attached']['js'] = [
    drupal_get_path('module', 'bibdk_subject_hierarchy') . '/js/bibdk_subject_hierarchy.js',
  ];

  switch ($form_state['stage']) {
    case 'suggest_subject':
      $form['#attributes'] = [
        'class' => [
          'subject-hierarchy-suggestion-form',
          'send-suggestion',
          'clearfix',
        ],
      ];
      return bibdk_subject_hierarchy_send_suggestion_form($form, $form_state);

    case 'is_confirmed':
      return bibdk_subject_hierarchy_confirmed_suggestion_form($form, $form_state);

    default:
      return bibdk_subject_hierarchy_send_suggestion_form($form, $form_state);
  }
}

/**
 * Hierarchy suggestion send form.
 *
 * @return array
 *   Form array.
 */
function bibdk_subject_hierarchy_send_suggestion_form($form, &$form_state) {
  $header = [
    '#type' => 'container',
    '#id' => 'search-hierarchy-suggestion-header',
    '#attributes' => [
      'class' => ['header', 'send-suggestion'],
    ],
  ];

  $header['headline'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('HEADER_HIERARCHY_SEND_SUGGESTION', [], ['context' => 'bibdk_subject_hierarchy']),
  ];

  $header['suggestion_text'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t("TEXT_HIERARCHY_SEND_SUGGESTION", [], ['context' => 'bibdk_subject_hierarchy']),
  ];

  $form['bibdk_subject_hierarchy'] = [
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#attributes' => [
      'class' => ['header', 'fieldset-hierarchy-suggestion'],
    ],
  ];

  $form['bibdk_subject_hierarchy']['subject_hierarchy_suggestion_header'] = $header;

  $form['bibdk_subject_hierarchy']['subject_hierarchy_suggestion'] = [
    '#type' => 'textfield',
    '#attributes' => [
      'class' => ['search-hierarchy-suggestion-input searchfield-input'],
      'placeholder' => t("PLACEHOLDER_HIERARCHY_SEND_SUGGESTION", [], ['context' => 'bibdk_subject_hierarchy']),
    ],
    '#default_value' => $values['subject_hierarchy_suggestion'] ?? NULL,
  ];

  $form['bibdk_subject_hierarchy']['subject_hierarchy_submit_suggestion'] = [
    '#type' => 'submit',
    '#value' => t("BUTTON_HIERARCHY_SEND_SUGGESTION", [], ['context' => 'bibdk_subject_hierarchy']),
    '#attributes' => [
      'class' => ['searchfield-submit'],
    ],
    '#ajax' => [
      'callback' => 'bibdk_subject_hierarchy_suggestion_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-searchresult',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ],
  ];

  return $form;
}

/**
 * Create subject hierarchy suggestion conformation form.
 *
 * @return array
 *   Confirmed suggestion form.
 */
function bibdk_subject_hierarchy_confirmed_suggestion_form($form, $form_state) {
  $header = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['header', 'confirmed-suggestion'],
    ],
  ];

  $header['close'] = [
    '#theme' => 'link',
    '#path' => '',
    '#text' => '&nbsp;',
    '#options' => [
      'attributes' => [
        'class' => ['subjects-close-button close icon icon-x-altx-alt'],
      ],
      'html' => TRUE,
    ],
  ];

  $header['headline'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('HEADER_HIERARCHY_CONFIRMED_SUGGESTION', [], ['context' => 'bibdk_subject_hierarchy']),
  ];

  $header['suggestion_text'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('TEXT_HIERARCHY_CONFIRMED_SUGGESTION', [], ['context' => 'bibdk_subject_hierarchy']),
  ];

  $form['bibdk_subject_hierarchy_suggestionresults'] = [
    'header' => $header,
  ];

  return $form;
}

/**
 * Hierarchy suggestion form validation.
 */
function bibdk_subject_hierarchy_suggestion_form_validate($form, &$form_state) {
  $placeholder = t("PLACEHOLDER_HIERARCHY_SEND_SUGGESTION", [], ['context' => 'bibdk_subject_hierarchy']);
  if ($form_state['stage'] == 'suggest_subject') {
    $suggestion = $form_state['values']['subject_hierarchy_suggestion'] ?? NULL;
    if (empty($suggestion) || $suggestion == $placeholder) {
      form_set_error('subject_hierarchy_suggestion', t('suggestion cant be empty'));
    }
    $form_state['values']['subject_hierarchy_suggestion'] = check_plain($suggestion);
  }
}

/**
 * Suggestion form next stage.
 *
 * @return string
 *   Stage status.
 */
function bibdk_subject_hierarchy_suggestion_move_to_next_stage($form, $form_state) {
  $state = '';
  if ($form_state['stage'] == 'suggest_subject') {
    $state = 'is_confirmed';
  }

  return $state;
}

/**
 * Subject hierarchy suggestion form submit method.
 *
 * @return string
 *   Rendered suggestion form.
 */
function bibdk_subject_hierarchy_suggestion_form_submit($form, &$form_state) {
  if ($form_state['stage'] == 'is_confirmed') {
    $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
    $suggestion = check_plain($form_state['values']['subject_hierarchy_suggestion']);
    $to = variable_get('bibdk_subject_hierarchy_email_to', NULL);
    $from = variable_get('bibdk_subject_hierarchy_email_from', NULL);
    if ($to) {
      drupal_mail('bibdk_subject_hierarchy', 'suggestion', $to, language_default(), $params = ['!suggestion' => $suggestion], $from);
    }
  }
  else {
    $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
    $form_state['new_stage'] = bibdk_subject_hierarchy_suggestion_move_to_next_stage($form, $form_state);
  }

  if (isset($form_state['multistep_values']['form_build_id'])) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }

  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['stage'] = $form_state['new_stage'];
  $form_state['rebuild'] = TRUE;

  $suggestion_form = [
    '#type' => 'container',
    '#attributes' => [
      'id' => ['bibdk-subject-hierarchy-searchresult'],
      'class' => ['bibdk-subject-hierarchy-searchresult', 'subjects-sublists'],
    ],
    'suggestion_form' => $form,
  ];

  return drupal_render($suggestion_form);
}

/**
 * Implements hook_mail().
 */
function bibdk_subject_hierarchy_mail($key, &$message, $params) {
  if ($key == 'suggestion') {
    $langcode = $message['language']->language;
    $message['subject'] = t('EMAIL_SUBJECT_HIERARCHY_SUGGESTION', [], [
      'langcode' => $langcode,
      'context' => 'bibdk_subject_hierarchy',
    ]);
    $message['body'][] = t("EMAIL_BODY_HIERARCHY_SUGGESTION\n\n!suggestion", $params, [
      'langcode' => $langcode,
      'context' => 'bibdk_subject_hierarchy',
    ]);
  }
}

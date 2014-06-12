<?php

/** Create subject hierachy suggestion form
 *  * @return array
 */
function bibdk_subject_hierachy_suggestion_form($form, &$form_state) {

  if ( !isset($form_state['stage']) ) {
    $form_state['stage'] = 'suggest_subject';
  }
  
  switch ( $form_state['stage'] ) {

    case 'suggest_subject':
      return bibdk_subject_hierachy_send_suggestion_form($form, $form_state);
      break;  
  
    case 'confirm_suggestion': 
      return bibdk_subject_hierachy_confirm_suggestion_form($form, $form_state);  
      break;
 
    case 'is_confirmed': 
      return bibdk_subject_hierachy_confirmed_suggestion_form($form, $form_state);  
      break;
 
    default:
      return bibdk_subject_hierachy_send_suggestion_form($form, $form_state);
      break; 
  }
   
  return $form;

}



function bibdk_subject_hierachy_send_suggestion_form($form, &$form_state) {

  $header = array(
    '#type' => 'container',
    '#id' => 'search-hierarchy-suggestion-header',
    '#attributes' => array(
      'class' => array('header'),
    ),
  );

  $header['headline'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Suggestions for new subjects?', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $header['suggestion_text'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t("Didn't you find the subject you were looking for? If you have suggestions for new subjects - write it in the box.", array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $form['subject_hierarchy_suggestion_header'] = $header;

  $form['subject_hierarchy_suggestion'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'class' => array('search-hierarchy-suggestion-input'),
      'placeholder' => t('suggest a subject for the subject hierarchy'),
    ),
    '#default_value' => isset($values['subject_hierarchy_suggestion']) ? $values['subject_hierarchy_suggestion'] : NULL,
  );

  $form['subject_hierarchy_submit_suggestion'] = array(
    '#type' => 'submit',
    '#value' => t('Send suggestion'),
    '#attributes' => array(
      'class' => array('searchfield-submit'),
    ),
    '#ajax' => array(
      'callback' => 'bibdk_subject_hierachy_suggestion_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-searchresult',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );
  
  return $form;

}



/** Create subject hierachy suggestion conformation form
 *  * @return array
 */
function bibdk_subject_hierachy_confirm_suggestion_form($form, &$form_state) {

  $values = isset($form_state['multistep_values']['suggest_subject']) ? $form_state['multistep_values']['suggest_subject'] : array();
  
  $suggestion = isset($values['subject_hierarchy_suggestion']) ? $values['subject_hierarchy_suggestion'] : NULL;

  $header = array(
    '#type' => 'container',
    '#id' => 'search-hierarchy-suggestion-header',
    '#attributes' => array(
      'class' => array('header'),
    ),
  );

  $header['close'] = array(
    '#theme' => 'link',
    '#path' => '',
    '#text' => '&nbsp;',
    '#options' => array(
      'attributes' => array(
        'class' => array('themes__close-button close icon icon-blue-x'),
      ),
      'html' => true,
    ),
  );

  $header['headline'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Suggestion for new subject', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $header['suggestion_intro'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('Here is your suggestion', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $header['suggestion_text'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'class' => array('suggestion-confirm-text'),
    ),
    '#value' => $suggestion,
  );

  $form['subject_hierarchy_suggestion_header'] = $header;

  $form['subject_hierarchy_suggestion'] = array(
    '#type' => 'hidden',
    '#default_value' => $suggestion,
  );

  $form['subject_hierarchy_submit_confirmation'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm suggestion'),
    '#attributes' => array(
      'class' => array('searchfield-submit'),
    ),
    '#ajax' => array(
      'callback' => 'bibdk_subject_hierachy_suggestion_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-searchresult',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );

  return $form;

}



/** Create subject hierachy suggestion conformation form
 *  * @return array
 */
function bibdk_subject_hierachy_confirmed_suggestion_form($form, &$form_state) {

  $values = isset($form_state['multistep_values']['confirm_suggestion']) ? $form_state['multistep_values']['confirm_suggestion'] : array();

  $suggestion = isset($values['subject_hierarchy_suggestion']) ? $values['subject_hierarchy_suggestion'] : NULL;

  $header = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('header'),
    ),
  );

  $header['close'] = array(
    '#theme' => 'link',
    '#path' => '',
    '#text' => '&nbsp;',
    '#options' => array(
      'attributes' => array(
        'class' => array('themes__close-button close icon icon-blue-x'),
      ),
      'html' => true,
    ),
  );

  $header['headline'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Thank you for your suggestion', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $header['suggestion_text'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => t('We will look at suggestions next time we update the subject hierarchy.', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $form['bibdk_subject_hierarchy_suggestionresults'] = array(
    'header' => $header,
  );

  return $form;

}



function bibdk_subject_hierachy_suggestion_form_validate($form, &$form_state) {
     
  switch ($form_state['stage']) { 
       
    case 'suggest_subject':
    case 'confirm_suggestion': 
      $suggestion = isset($form_state['values']['subject_hierarchy_suggestion']) ? $form_state['values']['subject_hierarchy_suggestion'] : NULL;
      if ( empty($suggestion) ) {
        form_set_error('subject_hierarchy_suggestion', t('suggestion cant be empty'));
      }
      $form_state['values']['subject_hierarchy_suggestion'] = check_plain($suggestion);
      break;  
  
  }
  
  return;

}



function bibdk_subject_hierachy_suggestion_move_to_next_stage($form, &$form_state) {
 
  switch ( $form_state['stage'] ) {
    case 'suggest_subject':
      return 'confirm_suggestion';
     break;
    case 'confirm_suggestion':
      return 'is_confirmed';
     break;
  }
 
}



/** Subject hierachy suggestion form submit method
 *
 *  * @return array
 */
function bibdk_subject_hierachy_suggestion_form_submit($form, &$form_state) {
   
  watchdog('suggestion_form', '<pre>' . print_r($form_state['stage'], 1) . '</pre>');

  switch ( $form_state['stage'] ) {
     
    case 'is_confirmed':
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      // admin_suggestion_email($form, $form_state);
     break;
  
    default:
      $form_state['multistep_values'][$form_state['stage']] = $form_state['values'];
      $form_state['new_stage'] = bibdk_subject_hierachy_suggestion_move_to_next_stage($form, $form_state);
     break;
  
  }
 
  if ( isset($form_state['multistep_values']['form_build_id']) ) {
    $form_state['values']['form_build_id'] = $form_state['multistep_values']['form_build_id'];
  }

  $form_state['multistep_values']['form_build_id'] = $form_state['values']['form_build_id'];
  $form_state['stage'] = $form_state['new_stage'];
  $form_state['rebuild'] = TRUE;

  $suggestion_form = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('bibdk-subject-hierarchy-searchresult'),
      'class' => array('bibdk-subject-hierarchy-searchresult', 'themes__sublists'),
    ),
    'suggestion_form' => $form,
  );

  return drupal_render($suggestion_form);     

}

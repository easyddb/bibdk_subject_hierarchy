<?php

/** Create subject hierachy suggestion form
 *  * @return array
 */
function bibdk_subject_hierachy_suggestion_form($form, &$form_state) {

  if ( !isset($form_state['stage']) ) {
    $form_state['stage'] = 'suggest_subject';
  }
  
  $form = array();
  $form = bibdk_subject_hierachy_suggestion_get_header($form, $form_state);

  switch ($form_state['stage']) {
     
    case 'suggest_subject':
      return bibdk_subject_hierachy_send_suggestion_form($form, $form_state);
     break;  
  
    case 'confirm_suggestion': 
      return bibdk_subject_hierachy_confirm_suggestion_form($form, $form_state);  
     break;
 
    default:
      return bibdk_subject_hierachy_suggestion_form($form, $form_state);
     break; 
  }
   
  return $form;

}



function bibdk_subject_hierachy_send_suggestion_form($form, &$form_state) {
     
  $header = array(
    '#type' => 'container',
    '#id' => 'search-hierarchy-suggestion-form',
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
    '#value' => t('Did you not find the subject you were looking for? If you have suggestions for new subjects - write it in the box.', array(), array('context' => 'bibdk_subject_hierarchy') ),
  );

  $form['subject-hierarchy-suggestion-header'] = $header;

  $form['subject-hierarchy-suggestion'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'class' => array('search-hierarchy-suggestion-input'),
      'placeholder' => t('suggest a subject for the subject hierarchy'),
    ),
  );

  $form['subject-hierarchy-submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send suggestion'),
    '#attributes' => array(
      'class' => array('searchfield-submit'),
    ),
    '#ajax' => array(
      'callback' => 'bibdk_subject_hierachy_suggestion_confirmation_form',
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

// watchdog('confirmation_form', '<pre>' . print_r($form_state, 1) . '</pre>');

  $suggestion = $form_state['values']['subject-hierarchy-suggestion'];

  $header = array(
    '#type' => 'container',
    '#id' => 'search-hierarchy-suggestion-confirmation-form',
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

  $form['subject-hierarchy-suggestion-header'] = $header;

  $form['subject-hierarchy-suggestion'] = array(
    '#type' => 'hidden',
    '#value' => $suggestion,
    '#attributes' => array(
      'value' => $suggestion,
    ),
  );

  $form['subject-hierarchy-submit'] = array(
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

/*
  $return['bibdk-subject-hierarchy-suggestion-container'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('bibdk-subject-hierarchy-searchresult'),
      'class' => array('bibdk-subject-hierarchy-searchresult', 'themes__sublists'),
    ),
    'suggestionform' => $form,
  );
*/

  return $form;

}


function bibdk_subject_hierachy_suggestion_validate($form, &$form_state) {
     
  if ($form_state['triggering_element']['#value'] == 'Back') {
    return;
  }  
     
  switch ($form_state['stage']) { 
       
    case 'rate_the_room':
      return customer_survey_rate_the_room_validate($form, $form_state);
     break;  
  
    case 'rate_the_service': 
      return customer_survey_rate_the_service_validate($form, $form_state);  
     break;
 
    case 'personal_details': 
      return customer_survey_personal_details_validate($form, $form_state);  
     break;
  
  }
}




/** Subject hierachy suggestion form submit method
 *
 *  * @return array
 */
function bibdk_subject_hierachy_suggestion_form_submit($form, &$form_state) {

  $suggestion = $form_state['values']['subject-hierarchy-suggestion'];

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

  $return['bibdk-subject-hierarchy-suggestionresults'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('bibdk-subject-hierarchy-searchresult'),
      'class' => array('bibdk-subject-hierarchy-searchresult', 'themes__sublists'),
    ),
    'header' => $header,
    'suggestion' => $suggestion,
  );

  $commands[] = array(
    'command' => 'afterAjaxSubmit',
    'value' => drupal_render($return),
    'selector' => '#bibdk-subject-hierarchy-searchresult'
  );

  return array('#type' => 'ajax', '#commands' => $commands);

}


/** Get searchresult on term
 *
 * @param $term
 */
/*
function bibdk_subject_hierachy_searchfield_custom_search($term) {
  $params['agency'] = variable_get('ting_agency');
  $params['profile'] = variable_get('ting_search_profile', FALSE);
  $params['numResults'] = 0;
  $params['objectFormat'] = 'DKABM';
  $params['collectionType'] = 'work';
  $params['query'] = $term;

  $client = new ting_client_class();
  $opensearch_result = $client->do_request('search', $params);

  $return['result']['#markup'] = t('A search on %term gives %results results', array('%term' => $term, '%results' => $opensearch_result->numTotalObjects)) . ' ';
  $return['link'] = array(
    '#theme' => 'link',
    '#path' => 'search/work/' . $term,
    '#text' => t('Show result'),
    '#options' => array(
      'attributes' => array(),
      'html' => false,
    ),
  );

  return $return;

}
*/
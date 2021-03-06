<?php

/** Get searchresult from subject hierarchy
 *
 * @param $hierarchy
 * @param $term
 * @return array
 */
function _get_bibdk_subject_hierarchy_searchresult($term) {
  $hierarchy = _get_bibdk_subject_hierarchy();
  $path = _bibdk_subject_hierarchy_search_info();
  $result = array(
    'number_of_groups' => 0,
    'number_of_results' => 0,
    'result' => array(),
  );
  $indexes = _get_bibdk_subject_hierarchy_searchresult_index($hierarchy, $term);

  foreach ($indexes as $group_index => $group_elements) {
    $result['number_of_groups']++;
    $arr = explode(',', $group_index);
    $breadcrumb = _get_bibdk_subject_hierarchy_breadcrumbs($hierarchy, $arr, $path);
    $items = array();
    foreach ($group_elements as $element) {
      $result['number_of_results']++;
      $items[] = drupal_render(_get_bibdk_subject_hierarchy_searchresult_item($hierarchy, $element, $path));
    }

    $group_items = array(
      'breadcrumb' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('themes__breadcrumb'),
        ),
        'items' => $breadcrumb,
      ),
      'elements' => array(
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => array(
          'class' => 'themes__sublist',
        ),
      )
    );

    $result['result'][] = $group_items;
  }
  return $result;
}

/** Get indexes of searchresults
 *
 * Traverses the subjecthierarcky and returns the indexes where the item contains the searchterm $searchterm
 *
 * @param $hierarchy
 * @param $searchterm
 * @param array $hierarchy_level_index Records which level in the hierarchy we are traversing
 * @param array $results Keeps the current result indexes
 * @return array
 */
function _get_bibdk_subject_hierarchy_searchresult_index($hierarchy, $searchterm, $hierarchy_level_index = array(), &$results = array()) {

  foreach ($hierarchy as $key => $item) {
    // set level_index of current  item
    $current_level_index = $hierarchy_level_index;
    $current_level_index[] = $key;

    // if item contains $searchterm
    if (isset($item['ord']) && strripos($item['ord'], $searchterm) !== false) {
      $hierarchy_level_index_string = implode(',', $hierarchy_level_index);
      $current_level_index_string = implode(',', $current_level_index);
      $results[$hierarchy_level_index_string][] = $current_level_index_string;
    }
    // hierarchy contains more levels
    if (isset($item['term'])) {
      _get_bibdk_subject_hierarchy_searchresult_index($item['term'], $searchterm, $current_level_index, $results);
    }
  }
  return $results;
}

/** Returns an item in the subject hierarchy given an index
 *
 * @param $emnehierarki
 * @param $key
 * @param $path
 * @return array
 */
function _get_bibdk_subject_hierarchy_searchresult_item($emnehierarki, $key, $path) {

  $key_arr = explode(',', $key);
  $level_index = array_shift($key_arr);
  $link = array();

  if (isset($emnehierarki[$level_index])) {
    $item = _get_bibdk_subject_hierarchy_level($emnehierarki[$level_index], $key_arr);
    $link = _get_bibdk_subject_hierarchy_searchresult_item_link($item, $key, $path);
  }
  return $link;
}

/** Get link to subject hierarchy searchresult item
 *
 * @param $item
 * @param $key
 * @param $path
 * @return array
 */
function _get_bibdk_subject_hierarchy_searchresult_item_link($item, $key, $path) {
  if (empty($item)) {
    return array();
  }

  if (!empty($item['cql']) && empty($item['term'])) {
    $url = 'search/' . $path . '/' . trim($item['cql']);
    $class = array();
  }
  else {
    $url = 'bibdk_subject_hierarchy/nojs/' . $key;
    $class = array('use-ajax', 'nesting');
  }
  $link = array(
    '#theme' => 'link',
    '#text' => $item['ord'],
    '#path' => $url,
    '#options' => array(
      'attributes' => array(
        'class' => $class,
      ),
      'html' => false,
    ),
  );
  return $link;

}


/** Create subject hierarchy search form
 *  * @return array
 */
function bibdk_subject_hierarchy_searchfield_form($form, &$form_state) {
  $form = array();
  $form['search-hierarchy-input'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'class' => array('searchfield-input'),
      'placeholder' => t('search in subject hierarchy'),
    ),
  );

  $form['search-hierarchy-submit'] = array(
    '#type' => 'submit',
    '#value' => t('search'),
    '#attributes' => array(
      'class' => array('searchfield-submit'),
    ),
    '#ajax' => array(
      'callback' => 'bibdk_subject_hierarchy_searchfield_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-searchresult',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );
  return $form;
}

/** Subject hierarchy search form submit method
 *
 *  * @return array
 */
function bibdk_subject_hierarchy_searchfield_form_submit($form, &$form_state) {

  $search_term = $form_state['values']['search-hierarchy-input'];

  $searchresult = _get_bibdk_subject_hierarchy_searchresult($search_term);
  $search_in_well = bibdk_subject_hierarchy_searchfield_custom_search($search_term);

  $result_text = t('You got !number_of_results results in !number_of_groups groups', array(
    '!number_of_results' => $searchresult['number_of_results'],
    '!number_of_groups' => $searchresult['number_of_groups'],
  ));

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
    '#value' => t('Subject hierarchy searchresult on @term', array('@term' => $search_term))
  );

  $header['result_text'] = array(
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => $result_text,
  );

  $suggestionform = array();
  if ( variable_get('bibdk_subject_hierarchy_email_to', NULL) ) {
    $suggestionform = drupal_get_form('bibdk_subject_hierarchy_suggestion_form');
  }

  $return['bibdk-subject-hierarchy-searchresults'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('bibdk-subject-hierarchy-searchresult'),
      'class' => array('bibdk-subject-hierarchy-searchresult', 'themes__sublists'),
    ),
    'header' => $header,
    'result' => $searchresult['result'],
    'well_result' => $search_in_well,
    'suggestionform' => $suggestionform,
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
function bibdk_subject_hierarchy_searchfield_custom_search($term) {
  module_load_include('client.inc', 'ting');
  
  // First, try to get same query on cql.
  $obj = new ccl2cql($term);
  $term = $obj->getCql();
  
  $result = ting_do_search($term, 0, 0, array());

  $return['result']['#markup'] = t('A search on %term gives %results results', array('%term' => $term, '%results' => $result->numTotalObjects)) . ' ';
  $return['link'] = array(
    '#theme' => 'link',
    '#path' => 'search/ting/' . $term,
    '#text' => t('Show result'),
    '#options' => array(
      'attributes' => array(),
      'html' => false,
    ),
  );

  return $return;

}

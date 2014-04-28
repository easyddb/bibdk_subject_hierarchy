<?php

/**
 * @param $hierachy
 * @param $term
 * @return array
 * TODO: break it down
 */
function _get_bibdk_subject_hierarchy_searchresult($term) {
  $hierarchy = _get_bibdk_subject_hierarchy();
  $indexes = _get_bibdk_subject_hierarchy_searchresult_index($hierarchy, $term);
  $path = _bibdk_subject_hierarchy_search_info();

  $result = array();

  // create groups
  // todo: move to search index function
  $groups = array();
  foreach ($indexes as $index) {
    $arr = explode(',', $index);
    $element_id = array_pop($arr);
    $group_id = implode(',', $arr);
    $groups[$group_id][$element_id] = $index;
  }
  $result['number_of_groups'] = count($groups);
  $result['number_of_results'] = count($indexes);

  foreach ($groups as $id => $group) {
    $arr = explode(',', $id);
    $breadcrumb = _get_bibdk_subject_hierarchy_breadcrumbs($hierarchy, $arr, $path);
    $items = array();
    foreach ($group as $element_id => $element) {
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
      )
    );

    $result['result'][] = $group_items;
  }
  return $result;
}

function _get_bibdk_subject_hierarchy_searchresult_index($hierachy, $term, $currentkey = array(), &$results = array()) {
  foreach ($hierachy as $key => $item) {
    $current = $currentkey;
    $current[] = $key;
    if (strripos($item['ord'], $term) !== false) {
      $results[] = implode(',', $current);
    }
    if (isset($item['term'])) {
      _get_bibdk_subject_hierarchy_searchresult_index($item['term'], $term, $current, $results);
    }
  }

  return $results;
}

function _get_bibdk_subject_hierarchy_searchresult_item($emnehierarki, $key, $path) {

  $key_arr = explode(',', $key);
  $level = array_shift($key_arr);
  $item = _get_bibdk_subject_hierarchy_level($emnehierarki[$level], $key_arr);

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


/** Create subject hierachy search form
 *  * @return array
 */
function bibdk_subject_hierachy_searchfield_form($form, &$form_state) {
  $form = array();
  $form['input'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'class' => array('searchfield-input'),
      'placeholder' => t('search in subject hierachy'),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('search'),
    '#attributes' =>array(
      'class' => array('searchfield-submit'),
    ),
    '#ajax' => array(
      'callback' => 'bibdk_subject_hierachy_searchfield_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-content',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );
  return $form;
}

/** Create subject hierachy search form
 *  * @return array
 */
function bibdk_subject_hierachy_searchfield_form_submit($form, &$form_state) {

  $search_term = $form_state['values']['input'];

  $searchresult = _get_bibdk_subject_hierarchy_searchresult($search_term);


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
    '#text' => 'close',
    '#options' => array(
      'attributes' => array(
        'class' => array(),
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

  $return['bibdk-subject-hierarchy-searchresults'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => array('bibdk-subject-hierarchy-content'),
      'class' => array('bibdk-subject-hierarchy-searchresult', 'themes__sublists'),
    ),
    'header' => $header,
    'result' => $searchresult['result'],
  );

  return $return;

}

function bibdk_subject_hierachy_searchfield_custom_search($term) {
  $params['agency'] = variable_get('ting_agency');
  $params['profile'] = variable_get('ting_search_profile', FALSE);
  $params['numResults'] = 0;
  $params['objectFormat'] = 'DKABM';
  $params['collectionType'] = 'work';
  $params['query'] = $term;

  $client = new ting_client_class();
  $opensearch_result = $client->do_request('search', $params);




}


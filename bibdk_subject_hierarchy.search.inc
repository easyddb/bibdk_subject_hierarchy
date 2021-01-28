<?php

/**
 * @file
 * Bibdk subject hierarchy search functions.
 */

/**
 * Get search result from subject hierarchy.
 *
 * @param string $term
 *   The term being searched.
 *
 * @return array
 *   Grouped search result.
 */
function _get_bibdk_subject_hierarchy_searchresult($term) {
  $hierarchy = _get_bibdk_subject_hierarchy();
  $path = _bibdk_subject_hierarchy_search_info();
  $result = [
    'number_of_groups' => 0,
    'number_of_results' => 0,
    'result' => [],
  ];
  $indexes = _get_bibdk_subject_hierarchy_searchresult_index($hierarchy, $term);

  foreach ($indexes as $group_index => $group_elements) {
    $result['number_of_groups']++;
    $arr = explode(',', $group_index);
    $breadcrumb = _get_bibdk_subject_hierarchy_breadcrumbs($hierarchy, $arr, $path);
    $items = [];
    foreach ($group_elements as $element) {
      $result['number_of_results']++;
      $search_result = _get_bibdk_subject_hierarchy_searchresult_item($hierarchy, $element, $path);
      $items[] = drupal_render($search_result);
    }

    $group_items = [
      'breadcrumb' => [
        '#type' => 'container',
        '#attributes' => [
          'class' => ['subjects-breadcrumb'],
        ],
        'items' => $breadcrumb,
      ],
      'elements' => [
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => [
          'class' => 'subjects-sublist',
        ],
      ],
    ];

    $result['result'][] = $group_items;
  }

  return $result;
}

/**
 * Get indexes of searchresults.
 *
 * Traverses the subjecthierarcky and returns the indexes where the item
 * contains the searchterm $searchterm.
 *
 * @param array $hierarchy
 *   Hierarchy data.
 * @param string $searchterm
 *   Searched term.
 * @param array $hierarchy_level_index
 *   Records which level in the hierarchy we are traversing.
 * @param array $results
 *   Keeps the current result indexes.
 *
 * @return array
 *   Results array.
 */
function _get_bibdk_subject_hierarchy_searchresult_index($hierarchy, $searchterm, $hierarchy_level_index = [], &$results = []) {
  foreach ($hierarchy as $key => $item) {
    // Set level_index of current  item.
    $current_level_index = $hierarchy_level_index;
    $current_level_index[] = $key;

    // If item contains $searchterm.
    if (isset($item['ord']) && mb_strripos($item['ord'], $searchterm) !== FALSE) {
      $hierarchy_level_index_string = implode(',', $hierarchy_level_index);
      $current_level_index_string = implode(',', $current_level_index);
      $results[$hierarchy_level_index_string][] = $current_level_index_string;
    }
    // Hierarchy contains more levels.
    if (isset($item['term'])) {
      _get_bibdk_subject_hierarchy_searchresult_index($item['term'], $searchterm, $current_level_index, $results);
    }
  }
  return $results;
}

/**
 * Returns an item in the subject hierarchy given an index.
 *
 * @param array $emnehierarki
 *   Hierarchy tree.
 * @param string $key
 *   Id of item in the hierarchy tree.
 * @param string $path
 *   Search type.
 *
 * @return array
 *   Item renderable array.
 */
function _get_bibdk_subject_hierarchy_searchresult_item($emnehierarki, $key, $path) {
  $key_arr = explode(',', $key);
  $level_index = array_shift($key_arr);
  $link = [];

  if (isset($emnehierarki[$level_index])) {
    $item = _get_bibdk_subject_hierarchy_level($emnehierarki[$level_index], $key_arr);
    $link = _get_bibdk_subject_hierarchy_searchresult_item_link($item, $key, $path);
  }
  return $link;
}

/**
 * Get link to subject hierarchy search result item.
 *
 * @param array $item
 *   Search result item.
 * @param string $key
 *   Id of item in the hierarchy tree.
 * @param string $path
 *   Search type.
 *
 * @return array
 *   Link renderable array.
 */
function _get_bibdk_subject_hierarchy_searchresult_item_link($item, $key, $path) {
  if (empty($item)) {
    return [];
  }

  if (!empty($item['cql']) && empty($item['term'])) {
    $url = 'search/' . $path . '/' . trim($item['cql']);
    $class = [];
  }
  else {
    $url = 'bibdk_subject_hierarchy/nojs/' . $key;
    $class = ['use-ajax', 'nesting'];
  }

  return [
    '#theme' => 'link',
    '#text' => $item['ord'],
    '#path' => $url,
    '#options' => [
      'attributes' => [
        'class' => $class,
      ],
      'html' => FALSE,
    ],
  ];
}

/**
 * Create subject hierarchy search form.
 *
 * @return array
 *   Searchfield form array.
 */
function bibdk_subject_hierarchy_searchfield_form($form, &$form_state) {
  $form = [];
  $form['search-hierarchy-input'] = [
    '#type' => 'textfield',
    '#attributes' => [
      'class' => ['searchfield-input'],
      'placeholder' => t('search in subject hierarchy', [], ['context' => 'bibdk_subject_hierarchy']),
      'aria-label' => t('Search subject hierarchy', [], ['context' => 'bibdk_subject_hierarchy']),
    ],
  ];

  $form['search-hierarchy-submit'] = [
    '#type' => 'submit',
    '#value' => t('search'),
    '#attributes' => [
      'class' => ['searchfield-submit'],
    ],
    '#ajax' => [
      'callback' => 'bibdk_subject_hierarchy_searchfield_form_submit',
      'wrapper' => 'bibdk-subject-hierarchy-searchresult',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ],
  ];

  return $form;
}

/**
 * Subject hierarchy search form submit method.
 *
 * @return array
 *   Custom ajax command.
 *
 * @throws \TingClientException
 */
function bibdk_subject_hierarchy_searchfield_form_submit($form, $form_state) {
  $search_term = $form_state['values']['search-hierarchy-input'];
  $searchresult = _get_bibdk_subject_hierarchy_searchresult($search_term);
  $search_in_well = bibdk_subject_hierarchy_searchfield_custom_search($search_term);

  $result_text = t('You got !number_of_results results in !number_of_groups groups', [
    '!number_of_results' => $searchresult['number_of_results'],
    '!number_of_groups' => $searchresult['number_of_groups'],
  ]);

  $header = [
    '#type' => 'container',
    '#attributes' => [
      'class' => ['header'],
    ],
  ];

  $header['close'] = [
    '#theme' => 'link',
    '#path' => '',
    '#text' => '&nbsp;',
    '#options' => [
      'attributes' => [
        'alt' => t('close subject hierarchy submenu.', [], ['context' => 'bibdk_subject_hierarchy']),
        'class' => ['subjects-close-button close icon icon-x-altx-alt'],
      ],
      'html' => TRUE,
    ],
  ];

  $header['headline'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h1',
    '#value' => t('Subject hierarchy searchresult on @term', ['@term' => $search_term]),
  ];

  $header['result_text'] = [
    '#theme' => 'html_tag',
    '#tag' => 'h2',
    '#value' => $result_text,
  ];

  $suggestionform = [];
  if (variable_get('bibdk_subject_hierarchy_email_to', NULL)) {
    $suggestionform = drupal_get_form('bibdk_subject_hierarchy_suggestion_form');
  }

  $return['bibdk-subject-hierarchy-searchresults'] = [
    '#type' => 'container',
    '#attributes' => [
      'id' => ['bibdk-subject-hierarchy-searchresult'],
      'class' => ['bibdk-subject-hierarchy-searchresult', 'subjects-sublists'],
    ],
    'header' => $header,
    'result' => $searchresult['result'],
    'well_result' => $search_in_well,
    'suggestionform' => $suggestionform,
  ];

  $commands[] = [
    'command' => 'afterAjaxSubmit',
    'value' => drupal_render($return),
    'selector' => '#bibdk-subject-hierarchy-searchresult',
  ];

  return ['#type' => 'ajax', '#commands' => $commands];
}

/**
 * Get search result on term.
 *
 * @param string $term
 *   Searched term.
 *
 * @return array
 *   Renderable array of search result.
 *
 * @throws \TingClientException
 */
function bibdk_subject_hierarchy_searchfield_custom_search($term) {
  module_load_include('client.inc', 'ting');

  $result = opensearch_do_search($term, 0, 0, []);

  $return['result']['#markup'] = t('A search on %term gives %results results', [
    '%term' => $term,
    '%results' => $result->numTotalObjects,
  ]) . ' ';
  $return['link'] = [
    '#theme' => 'link',
    '#path' => 'search/ting/' . trim($term),
    '#text' => t('Show result'),
    '#options' => [
      'attributes' => [],
      'html' => FALSE,
    ],
  ];

  return $return;
}

<?php

/**
 * @file
 * Page callbacks and relevant forms (and friends).
 */

/**
 * Bit of a helper function for transforming to strings.
 *
 * Select options often have issues with "0", which happens to be one of our
 * states... Map around it to avoid issues.
 *
 * @param array $form_state
 *   Reference to form state, where we can stash a mapping for use in
 *   islandora_batch_map_value().
 *
 * @return array
 *   An array of two arrays:
 *   - the first maps state number to our key used for the "options" arrays.
 *   - the second maps our key in an "options" array to a human-readable
 *     version, for use as the actual options array.
 */
function islandora_batch_get_mappings(&$form_state) {
  $keys = array(
    ISLANDORA_BATCH_STATE__ERROR => 'error',
    ISLANDORA_BATCH_STATE__NOT_READY => 'not-ready',
    ISLANDORA_BATCH_STATE__READY => 'ready',
    ISLANDORA_BATCH_STATE__PENDING_CHILDREN => 'pending-children',
    ISLANDORA_BATCH_STATE__DONE => 'done',
  );
  $strings = array(
    ISLANDORA_BATCH_STATE__ERROR => t('Error'),
    ISLANDORA_BATCH_STATE__NOT_READY => t('Not ready to ingest'),
    ISLANDORA_BATCH_STATE__READY => t('Ready to ingest'),
    ISLANDORA_BATCH_STATE__PENDING_CHILDREN => t('Not ready to ingest; children pending'),
    ISLANDORA_BATCH_STATE__DONE => t('Ingested'),
  );
  $options = array();
  foreach ($keys as $key => $value) {
    $options[$value] = $strings[$key];
  }
  $form_state['stash']['keys'] = array_flip($keys);

  return array($keys, $options);
}

/**
 * Map option value back to state number.
 *
 * @param string $value
 *   A value for one of the options dealt with in
 *   islandora_batch_get_mappings().
 * @param array $form_state
 *   The form state, so we can access our "stashed" mapping.
 *
 * @return int
 *   The mapped state number, corresponding to $value.
 */
function islandora_batch_map_value($value, $form_state) {
  return $form_state['stash']['keys'][$value];
}

/**
 * Menu page callback to delete a single item.
 */
function islandora_batch_delete_item_page_callback($id) {
  return drupal_get_form('islandora_batch_delete_item_form', $id);
}

/**
 * Form building function to delete a single item.
 */
function islandora_batch_delete_item_form($form, &$form_state, $id) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  return confirm_form($form, t('Are you sure you want to delete "@item" from the ingest queue?', array(
    '@item' => $id,
  )), 'islandora_batch/reports/queue');
}

/**
 * Form submission handler, to delete and single item.
 */
function islandora_batch_delete_item_form_submit(&$form, &$form_state) {
  list($id) = $form_state['build_info']['args'];
  $transaction = db_transaction();
  db_delete('islandora_batch_state')
    ->condition('id', $id)
    ->execute();
  db_delete('islandora_batch_resources')
    ->condition('id', $id)
    ->execute();
  db_delete('islandora_batch_queue')
    ->condition('id', $id)
    ->execute();
  drupal_set_message(t('Deleted "@item" from queue.', array(
    '@item' => $id,
  )));
}

/**
 * Menu page callback, to set the state for a single item.
 */
function islandora_batch_set_item_state_page_callback($id) {
  return drupal_get_form('islandora_batch_set_item_state_form', $id);
}

/**
 * Form buidling function, to set the state for a single item.
 */
function islandora_batch_set_item_state_form($form, &$form_state, $id) {
  list($keys, $options) = islandora_batch_get_mappings($form_state);

  $current = db_select('islandora_batch_state', 's')
    ->fields('s', array('state'))
    ->condition('id', $id)
    ->execute()
    ->fetchField();
  $form['state'] = array(
    '#type' => 'select',
    '#title' => t('State'),
    '#description' => t('The state of the given object in the batch queue.'),
    '#default_value' => $keys[(int) $current],
    '#options' => $options,
  );
  return confirm_form($form, t('Are you sure you want to change the state of "@item" in the queue?', array(
    '@item' => $id,
  )), 'islandora_batch/reports/queue');
}

/**
 * Form submission handler, to set the state for a single item.
 */
function islandora_batch_set_item_state_form_submit(&$form, &$form_state) {
  list($id) = $form_state['build_info']['args'];
  $state = $form_state['values']['state'];
  db_update('islandora_batch_state')
    ->fields(array('state' => islandora_batch_map_value($state, $form_state)))
    ->condition('id', $id)
    ->execute();
  drupal_set_message(t('Updated "@item" to "@state", in queue.', array(
    '@item' => $id,
    '@state' => $form['state']['#options'][$state],
  )));
}

/**
 * Menu callback, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_page_callback($id) {
  return drupal_get_form('islandora_batch_set_set_items_state_form', $id);
}

/**
 * Form building function, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_form($form, &$form_state, $id) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  list($keys, $options) = islandora_batch_get_mappings($form_state);

  $form['source_state'] = array(
    '#type' => 'select',
    '#title' => t('Change from'),
    '#description' => t('Items in the set with this state will be changed.'),
    '#empty_option' => t('-- Any --'),
    '#empty_value' => 'any',
    '#options' => $options,
  );
  $form['dest_state'] = array(
    '#type' => 'select',
    '#title' => t('Change to'),
    '#description' => t('Items will be set to this state.'),
    '#options' => $options,
  );

  return confirm_form($form, t('Are you sure you want to change the state of items in set "@set" in the queue?', array(
    '@set' => $id,
  )), "islandora_batch/reports/set");
}

/**
 * Form submission handler, to set the state for all items in a batch set.
 */
function islandora_batch_set_set_items_state_form_submit(&$form, &$form_state) {
  list($id) = $form_state['build_info']['args'];
  $source_state = $form_state['values']['source_state'];
  $dest_state = $form_state['values']['dest_state'];
  $s = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('id'))
    ->condition('sid', $id);
  $q = db_update('islandora_batch_state')
    ->fields(array('state' => islandora_batch_map_value($dest_state, $form_state)))
    ->condition('id', $s, 'IN');

  if ($source_state !== 'any') {
    $q->condition('state', islandora_batch_map_value($source_state, $form_state));
  }

  $updated = $q->execute();

  drupal_set_message(format_plural($updated, 'Updated 1 item in queue to "@state".', 'Updated @count items in queue to "@state".', array(
    '@state' => $form['dest_state']['#options'][$dest_state],
  )));
}

/**
 * Menu page callback, to delete a set and all contained items.
 */
function islandora_batch_delete_set_page_callback($id) {
  return drupal_get_form('islandora_batch_delete_set_form', $id);
}

/**
 * Form building function, to delete a set and all contained items.
 */
function islandora_batch_delete_set_form($form, &$form_state, $id) {
  form_load_include($form_state, 'inc', 'islandora_batch', 'includes/menu');
  return confirm_form($form, t('Are you sure you want to delete the items in set "@set" from the queue?', array(
    '@set' => $id,
  )), "islandora_batch/reports/set");
}

/**
 * Form submission handler, to delete a set and all contained items.
 */
function islandora_batch_delete_set_form_submit(&$form, &$form_state) {
  list($id) = $form_state['build_info']['args'];
  $transaction = db_transaction();
  $s = db_select('islandora_batch_queue', 'q')
    ->fields('q', array('id'))
    ->condition('sid', $id);
  db_delete('islandora_batch_state')
    ->condition('id', $s, 'IN')
    ->execute();
  db_delete('islandora_batch_resources')
    ->condition('id', $s, 'IN')
    ->execute();
  $deleted = db_delete('islandora_batch_queue')
    ->condition('sid', $id)
    ->execute();
  db_delete('islandora_batch_set')
    ->condition('id', $id)
    ->execute();

  // Mark any files as being unused--free to be delete.
  $q = db_select('file_usage', 'u')
    ->fields('u', array('fid'))
    ->condition('module', 'islandora_batch')
    ->condition('type', 'islandora_batch_set')
    ->execute();
  $files = file_load_multiple($q->fetchCol());
  foreach ($files as $file) {
    file_usage_delete($file, 'islandora_batch', 'islandora_batch_set', $id);
  }

  drupal_set_message(format_plural($deleted, 'Deleted 1 item from queue.', 'Deleted @count items from queue.'));
}

<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Determine if the given item exists in the queue, independent of state.
 *
 * @param string $id
 *   A string representing the ID of an item in the queue.
 *
 * @return bool
 *   TRUE if there's an entry for the given ID in the queue; otherwise, FALSE.
 */
function islandora_batch_item_exists($id) {
  $result = db_select('islandora_batch_queue', 'q')
    ->fields('q', array())
    ->condition('id', $id)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result > 0;
}

/**
 * Determine if an item was created by the given user.
 *
 * @param string $id
 *   A string representing the ID of an item in the queue.
 * @param object $user
 *   An object containing a loaded Drupal user. Defaults to the global $user,
 *   if not provided.
 *
 * @return bool
 *   TRUE if the item is in a set owned by the given user; otherwise, FALSE.
 */
function islandora_batch_item_is_owner($id, $user = NULL) {
  if ($user === NULL) {
    global $user;
  }

  $q = db_select('islandora_batch_queue', 'q');
  $a = $q->join('islandora_batch_set', 's', 's.id = q.sid');

  $result = $q->fields('q', 'id')
    ->condition('q.id', $id)
    ->condition('s.uid', $user->uid)
    ->countQuery()
    ->execute()
    ->fetchField();
  return $result > 0;
}

/**
 * Determine if a set with the given ID exists.
 *
 * @param int $id
 *   An integer identifying a batch set.
 *
 * @return bool
 *   TRUE if the batch exists; otherwise, FALSE.
 */
function islandora_batch_set_exists($id) {
  $result = db_select('islandora_batch_set', 's')
    ->fields('s', array())
    ->condition('id', $id)
    ->countQuery()
    ->execute()
    ->fetchField();

  return $result > 0;
}

/**
 * Determine if the set exists and belongs to the given user.
 *
 * @param int $id
 *   An integer identifying a batch set.
 * @param object $user
 *   An object containing a loaded Drupal user. Defaults to the global $user,
 *   if not provided.
 *
 * @return bool
 *   TRUE if the batch exists was created by the given user; otherwise, FALSE.
 */
function islandora_batch_set_is_owner($id, $user = NULL) {
  if ($user === NULL) {
    global $user;
  }

  $result = db_select('islandora_batch_set', 's')
    ->fields('s', array())
    ->condition('id', $id)
    ->condition('uid', $user->uid)
    ->countQuery()
    ->execute()
    ->fetchField();

  return $result > 0;
}

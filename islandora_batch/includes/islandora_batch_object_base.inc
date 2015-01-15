<?php

/**
 * @file
 * Abstract base class for objects ingested via batch framework.
 */

/**
 * Batch interface.
 *
 * Implementing classes should subclass some version of FedoraObject, so
 */
abstract class IslandoraBatchObject extends IslandoraNewFedoraObject {
  /**
   * The initial batch state.
   *
   * @return int
   *   Used to determine the initial state of the object in the queue.
   *   - 0: Not ready to be processed.
   *   - 1: Ready to be processed.
   *   - 2: Partially processed (waiting for children to be processed).
   *   - 3: Fully processed... Push to the back-end.
   */
  public function initialBatchState() {
    return ISLANDORA_BATCH_STATE__READY;
  }

  /**
   * Perform arbitrary processing before ingest.
   *
   * @return int
   *   Return the state after processing. If it is "fully processed", this
   *   object should be pushed to the back-end.
   *   If the state is "partially processed", we should be careful that the
   *   work does not get performed again next time around.
   */
  abstract public function batchProcess();

  /**
   * Get the resources to which we should link in the database.
   *
   * Should be in the format used by
   * IslandoraBatchPreprocessor::addToDatabase().
   */
  abstract public function getResources();

  /**
   * Get the child objects, for those content models which require them.
   *
   * Things like books and newspapers need two different layers of objects...
   * the definition of this function should let them be reliably created.
   *
   * Should only be used during preprocessing.
   *
   * @return array
   *   An array of IslandoraBatchObjects, which will be put into the database
   */
  public function getChildren(IslandoraTuque $connection) {
    return array();
  }

  /**
   * Add relationships (during preprocessing).
   *
   * Add relationships to the object.
   */
  abstract public function addRelationships();

  /**
   * Get the ID of this object in the queue.
   *
   * Only valid to call after we've been added to the database, and before
   * we have actually been ingested.
   *
   * @return int
   *   The ID of this object, as an integer.
   */
  public function getBatchId() {
    $id = db_select('islandora_batch_queue', 'q')
      ->fields('q', array('bid'))
      ->condition('id', $this->id)
      ->execute()
      ->fetchField();
    if ($id) {
      return intval($id);
    }
    else {
      throw new IslandoraBatchNoIdException(t('No ID found for @id...  Is it in the queue yet?', array(
        '@id' => $this->id,
      )));
    }
  }
}

class IslandoraBatchNoIdException extends Exception {}

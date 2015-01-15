<?php

/**
 * @file
 * Abstract preprocessor base class for batch.
 */

/**
 * Lock exception.
 */
class IslandoraBatchFailedToLockException extends Exception {

}

/**
 * Abstract preprocessor base class.
 */
abstract class IslandoraBatchPreprocessor {

  /**
   * @var array $parameters
   *   Should be populated with the parameters passed in.
   */
  protected $parameters = array();
  /**
   * @var IslandoraTuque $connection
   *   An IslandoraTuque object from which we can obtain IDs.
   */
  protected $connection;

  /**
   * @var string|int $setId
   *   A string/int identifying the set of objects.
   */
  protected $setId;

  /**
   * Constructor must be able to receive an associative array of parameters.
   *
   * @param array $parameters
   *   An associative array of parameters for the batch process. These will
   *   probably just be the result of a simple transformation from the
   *   command line, or something which could have been constructed from a
   *   form.
   *   Available parameters are from the particular concrete implementation.
   */
  public function __construct(IslandoraTuque $connection, array $parameters) {
    $this->parameters = $parameters;
    $this->connection = $connection;

    global $user;
    $insert = db_insert('islandora_batch_set')
      ->fields(array(
        'uid' => $user->uid,
        'created' => time(),
      ));
    $this->setId = $insert->execute();

    $lock = lock_acquire('islandora_batch_processing');
    if (!$lock) {
      throw new IslandoraBatchFailedToLockException(
          t('Failed to acquire lock for batch ingest.')
      );
    }
  }

  /**
   * PHP Magic function.
   *
   * Ensure we still have/can get our lock when we wake up, as we can be
   * serialized.
   */
  public function __wakeup() {
    $lock = lock_acquire('islandora_batch_processing');
    if (!$lock) {
      throw new IslandoraBatchFailedToLockException(
          t('Failed to acquire lock for batch ingest.')
      );
    }
  }

  /**
   * Actually cause the preprocessing to happen.
   *
   * In most cases, this should be fairly quick--not requiring multiple
   * iterations as a batch process.
   */
  abstract public function preprocess();

  /**
   * Get an identifier we can use for an object.
   *
   * @return string
   *   An id to use in the back-end (Fedora).
   */
  protected function getId() {
    // Not specifying a namespace should result in one from the default
    // declared in the fedora.fcfg ("changeme" by default).
    $namespace = (isset($this->parameters['namespace'])) ?
      $this->parameters['namespace'] :
      NULL;

    // TODO: Implement some form of caching, so we can get multiple at a time.
    return $this->connection->repository->api->m->getNextPid($namespace);
  }

  /**
   * Accessor for the set ID.
   */
  public function getSetId() {
    return $this->setId;
  }

  /**
   * Add the provided object to the database.
   *
   * Should be called from inside the "preprocess" method.
   *
   * @param IslandoraBatchObject $object
   *   The object to be later processed and added to Fedora.
   * @param array $resources
   *   An associative array of arrays, mapping types of resources to an array
   *   of relevant resources for the given object.
   *
   * @return string
   *   The PID of the object (one will be allocated if the object does not
   *   already have one; otherwise, that on the object will be returned).
   */
  protected function addToDatabase(AbstractFedoraObject $object, array $resources = array(), $parent = NULL) {
    if (!isset($object->id)) {
      $object->id = $this->getId();
    }

    db_insert('islandora_batch_queue')
      ->fields(array('id', 'parent', 'data', 'sid'))
      // Could queue up multiple in a static cache, to dump all together?
      ->values(array($object->id, $parent, serialize($object), $this->setId))
      ->execute();
    db_insert('islandora_batch_state')
      ->fields(array('id', 'state'))
      ->values(array($object->id, $object->initialBatchState()))
      ->execute();

    foreach ($resources as $type => $typed_resources) {
      foreach ($typed_resources as $resource) {
        db_insert('islandora_batch_resources')
          ->fields(array('id', 'type', 'resource'))
          ->values(array($object->id, $type, $resource))
          ->execute();
      }
    }

    return $object->id;
  }
}

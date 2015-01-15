<?php
/**
 * @file
 * Utility functions for use with the Islandora Simple Workflow module.
 */

/**
 * Query for inactive objects.
 *
 * @return array
 *   An array containing the results of the inactive objects query.
 */
function islandora_simple_workflow_get_inactive_objects() {
  $connection = new IslandoraTuque();
  $query = <<<EOQ
SELECT ?object ?title FROM <#ri> WHERE
{
  ?object <fedora-model:label> ?title ;
          <fedora-model:state> <info:fedora/fedora-system:def/model#Inactive>
}
ORDER BY ?object
EOQ;
  $results = $connection->repository->ri->sparqlQuery($query);
  return $results;
}

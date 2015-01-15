<?php

/**
 * @file
 * API documentation.
 */

/**
 * Notify other modules about the addition of objects to a compound.
 *
 * @param array $objects
 *   An array of AbstractObjects added to the parents, to produce compounds.
 * @param array $parent_pids
 *   An array of PIDs representing the parents to which each object was added,
 *   to make each member of $objects a member of multiple compounds.
 */
function hook_islandora_compound_object_children_added_to_parent($objects, $parent_pids) {

}

/**
 * Notify other modules about the removal of objects from a compound.
 *
 * @param array $objects
 *   An array of AbstractObjects removed from the parents.
 * @param array $parent_pids
 *   An array of PIDs representing the parents to which each object was added,
 *   to make each member of $objects a member of multiple compounds.
 */
function hook_islandora_compound_object_children_removed_from_parent($object, $parent_pids) {

}

<?php

/**
 * @file
 * Holds helper functions for islandora_ip_embargo.
 */

/**
 * Get all lists for table.
 *
 * This is not a general function it is used for pagination.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_lists() {
  $query = db_select("islandora_ip_embargo_lists");
  $query = $query->fields('islandora_ip_embargo_lists');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->limit(10);

  return $query->execute();
}

/**
 * Fetch range records for a given list.
 *
 * @param int $lid
 *   list id, PK for the table islandora_ip_embargo_ip_ranges
 *
 * @return DatabaseStatementInterface
 *   query result
 */
function islandora_ip_embargo_get_ranges($lid) {
  $ranges = db_select('islandora_ip_embargo_ip_ranges')
  ->fields('islandora_ip_embargo_ip_ranges')
  ->condition('lid', $lid)
  ->execute();
  return $ranges;
}

/**
 * Create a list.
 *
 * @param string $name
 *   The name of the new list.
 */
function islandora_ip_embargo_add_list($name, $description) {
  db_insert('islandora_ip_embargo_lists')
  ->fields(array('name' => $name, 'description' => $description))
  ->execute();
}

/**
 * Edit a list.
 *
 * @param string $name
 *   The name of the list.
 * @param int $list_identifier
 *   The identifier of the list.
 */
function islandora_ip_embargo_edit_list($name, $description, $list_identifier) {
  db_update('islandora_ip_embargo_lists')
  ->fields(array('name' => $name, 'description' => $description))
  ->condition('lid', $list_identifier)
  ->execute();
}

/**
 * Remove a list.
 *
 * @param int $list_identifier
 *   The identifier of the list to delete.
 */
function islandora_ip_embargo_remove_list($list_identifier) {
  db_delete('islandora_ip_embargo_lists')
    ->condition('lid', $list_identifier)
    ->execute();
}

/**
 * Get the a list object.
 *
 * @param string $field
 *   Field in islandora_ip_embargo_lists to use for the lookup.
 *   Must be one of 'lid' | 'name'; or error.
 *
 * @param string $value
 *   Value of the field used for the lookup.
 *
 * @return object
 *   The list object.
 */
function islandora_ip_embargo_get_list_object_by($field, $value) {
  if (!in_array($field, array('lid', 'name'))) {
    throw new Exception("Incorrect field: ($field) given for list lookup");
  }
  $list = db_select('islandora_ip_embargo_lists', 'i')
  ->fields("i")
  ->condition($field, $value)
  ->range(0, 1)
  ->execute()->fetchObject();
  return $field == 'lid' ? $list->name : $list->lid;
}

/**
 * Get the name of a list.
 *
 * @param int $list_identifier
 *   The identifier of the list to delete.
 *
 * @return string
 *   The human readable name of the list.
 */
function islandora_ip_embargo_get_list_name($list_identifier) {
  $list = db_select('islandora_ip_embargo_lists')
    ->fields('islandora_ip_embargo_lists')
    ->condition('lid', $list_identifier)
    ->execute();
  $data = $list->fetchObject();

  return $data->name;
}

function islandora_ip_embargo_get_list_object_by_lid($lid) {
  $list = db_select('islandora_ip_embargo_lists', 'i')
  ->fields("i")
  ->condition('lid', $lid)
  ->range(0, 1)
  ->execute()->fetchObject();
  return $list;
}

/**
 * Given a list id, returns the list name.
 *
 * @param string|int $list_identifier
 *   value corresponding to the lid field in the
 *   `islandora_ip_embargo_lists` table.
 */
function islandora_ip_embargo_get_list_name($list_identifier) {
  return islandora_ip_embargo_get_list_object_by('lid', $list_identifier);
}

/**
 * Get a list's information.
 *
 * @param int $list_identifier
 *   The identifier of the list.
 * @param bool $page_results
 *   TRUE to page results for display, otherwise return all results.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_lists_information($list_identifier, $page_results = TRUE) {
  $query = db_select("islandora_ip_embargo_lists", 'lists');
  $query->join('islandora_ip_embargo_ip_ranges', 'ranges', 'lists.lid = ranges.lid');
  $query = $query->fields('ranges');
  $query = $query->fields('lists');
  $query = $query->condition('lists.lid', $list_identifier);
  if ($page_results) {
    $query = $query->extend('TableSort');
    $query = $query->extend('PagerDefault');
    $query = $query->limit(10);
  }
  return $query->execute();
}

/**
 * Remove an IP address range.
 *
 * @param int $range_identifier
 *   The identifier of the range to delete.
 */
function islandora_ip_embargo_remove_range($range_identifier) {
  db_delete('islandora_ip_embargo_ip_ranges')
    ->condition('rid', $range_identifier)
    ->execute();
}

/**
 * Create an IP address range on a list.
 *
 * @param int $list_identifier
 *   The identifier of the list.
 * @param string $low_end
 *   The low end of the IP range.
 * @param string $high_end
 *   The high end of the IP range.
 */
function islandora_ip_embargo_add_ip_range($list_identifier, $low_end, $high_end) {
  db_insert('islandora_ip_embargo_ip_ranges')
    ->fields(array(
      'low_end' => $low_end,
      'high_end' => $high_end,
      'lid' => $list_identifier,
    ))
    ->execute();
}

/**
 * Get an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to get the embargo for.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_embargo($islandora_object_id) {
  $embargoes = db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('pid', $islandora_object_id)
    ->execute();

  return $embargoes;
}

/**
 * Gets embargoes associated with a list.
 *
 * @param string $list_identifier
 *   The id of the list to find embargoes for.
 *
 * @return SelectQuery
 *   The query return.
 */
function islandora_ip_embargo_get_embargos($list_identifier) {
  $embargoes = db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('lid', $list_identifier)
    ->execute();

  return $embargoes;
}

/**
 * Update an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to update the embargo for.
 * @param int $list_id
 *   The IP address range list to use for the embargo.
 * @param int $expiry
 *   The timestamp to stop the embargo at.
 */
function islandora_ip_embargo_update_embargo($islandora_object_id, $list_id, $expiry = NULL, $public_dsids = NULL) {
  cache_clear_all('islandora_ip_embargo_restrict_access', 'cache');
  cache_clear_all('islandora_ip_embargo_restricted_datastreams', 'cache');
  db_update('islandora_ip_embargo_embargoes')
  ->fields(array(
    'pid' => $islandora_object_id,
    'lid' => $list_id,
    'expiry' => $expiry,
    'public_dsids' => $public_dsids,
  ))
  ->condition('pid', $islandora_object_id)
  ->execute();
}

/**
 * Set an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to set the embargo for.
 * @param int $list_id
 *   The IP address range list to use for the embargo.
 * @param int $expiry
 *   The timestamp to stop the embargo at.
 */
function islandora_ip_embargo_set_embargo($islandora_object_id, $list_id, $expiry = NULL, $public_dsids = NULL) {
  cache_clear_all('islandora_ip_embargo_restrict_access', 'cache');
  cache_clear_all('islandora_ip_embargo_restricted_datastreams', 'cache');
  cache_clear_all('islandora_ip_embargo_nearest_embargoes', 'cache');
  db_insert('islandora_ip_embargo_embargoes')
    ->fields(array(
      'pid' => $islandora_object_id,
      'lid' => $list_id,
      'expiry' => $expiry,
      'public_dsids' => $public_dsids,
    ))
    ->execute();
  module_invoke_all('islandora_ip_embargo_set', $islandora_object_id, array(
    'lid' => $list_id,
    'expiry' => $expiry,
  ));
}

/**
 * Remove an object's embargo information.
 *
 * @param string $islandora_object_id
 *   The PID of the object to remove the embargo from.
 */
function islandora_ip_embargo_remove_embargo($islandora_object_id) {
  db_delete('islandora_ip_embargo_embargoes')
    ->condition('pid', $islandora_object_id)
    ->execute();
  module_invoke_all('islandora_ip_embargo_lifted', $islandora_object_id);
}

/**
 * Will check if the current user's request IP is in an indicated range.
 *
 * @param string $low
 *   The low end bound of the IP address range.
 * @param string $high
 *   The high end bound of the IP address range.
 */
function islandora_ip_embargo_ip_in_range($low, $high) {
  $comparable_address = ip2long(ip_address());
  $comparable_high = ip2long($high);
  $comparable_low = ip2long($low);
  if ($comparable_address >= $comparable_low && $comparable_address <= $comparable_high) {
    return TRUE;
  }
  $djips = explode(',', variable_get('islandora_ip_embargo_djatoka_public_ip_address', '127.0.0.1'));
  foreach($djips as $djip) {
    if ($comparable_address == ip2long(trim($djip))) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Remove all embargoes if the expiry timestamp has passed.
 *
 * @param int $time_stamp
 *   The timestamp to judge embargo expiries by.
 */
function islandora_ip_embargo_cron_cleanup($time_stamp) {
  $num_deleted = db_delete('islandora_ip_embargo_embargoes')
    ->condition('expiry', $time_stamp, '<=')
    ->execute();

  watchdog(
    'Cron Job End',
    "The ip embargo cron job has ended, @num_deleted embargos have been removed.",
    array('@num_deleted' => $num_deleted)
  );
}

/**
 * This will send islandora_ip_embargo_to_be_lifted events via rules module.
 *
 * @param int $days_before
 *   How many days before an embargo is lifted to send the event.
 */
function islandora_ip_embargo_send_embargo_lift_events($days_before) {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/batch');
  $event_threshold = time() + $days_before * 24 * 60 * 60;

  // If rules is enabled trigger a batch of events for embargoes being lifted.
  if (module_exists('rules')) {
    batch_set(array(
      'operations' => array(
        array(
          'islandora_ip_embargo_embargo_lift_event_batch',
          array($event_threshold),
        ),
      ),
      'finished' => 'islandora_ip_embargo_lift_event_batch_finished',
      'title' => t('Sending out embargo lift events.'),
      'init_message' => t('Event batch starting.'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Event batch has encountered an error.'),
      'file' => drupal_get_path('module', 'islandora_ip_embargo') . '/includes/batch.inc',
    ));

    // This is a hack around the broken batch API.
    // https://drupal.org/node/638712#comment-2289138
    $batch =& batch_get();
    $batch['progressive'] = FALSE;

    batch_process();
  }
}

/**
 * This will get the number of embargoes that expire after a given time.
 *
 * Only counts those that have not had their event triggered.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound.
 *
 * @return int
 *   The number of embargoes.
 */
function islandora_ip_embargo_count_embargoes_before_timestamp($event_threshold) {
  return db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->countQuery()
    ->execute()
    ->fetchField();
}

/**
 * This function will get the list of embargoes that expire after a given time.
 *
 * Only returns those that have not had their event triggered.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound.
 * @param int $offset
 *   The offset for the query.
 * @param int $limit
 *   The limit for the query.
 *
 * @return object
 *   The results object from the query.
 */
function islandora_ip_embargo_get_embargoes_before_timestamp($event_threshold, $offset, $limit) {
  return db_select('islandora_ip_embargo_embargoes')
    ->fields('islandora_ip_embargo_embargoes')
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->orderBy('pid', 'ASC')
    ->range($offset, $limit)
    ->execute();
}

/**
 * Updates the table to indicated the objects' embargo lift events were sent.
 *
 * @param int $event_threshold
 *   Timestamp of upper bound for objects to set the triggered state on.
 *
 * @return object
 *   The results object from the update.
 */
function islandora_ip_embargo_set_event_triggered($event_threshold) {
  return db_update('islandora_ip_embargo_embargoes')
    ->fields(array('expiry_event_triggered' => 1))
    ->condition('expiry', $event_threshold, '<=')
    ->condition('expiry_event_triggered', 0, '=')
    ->execute();
}

/**
 * Retrieve all objects effected by embargos, using pageination.
 *
 * @param int $limit
 *   The limit to set on the query.
 *
 * @return array
 *   The list of embargoed objects.
 */
function islandora_ip_embargo_get_all_embargo_objects($limit = 10) {
  $query = db_select('islandora_ip_embargo_embargoes');
  $query = $query->extend('TableSort');
  $query = $query->extend('PagerDefault');
  $query = $query->fields('islandora_ip_embargo_embargoes');
  $query = $query->limit($limit);
  $embargoes = $query->execute();

  return $embargoes;
}

/**
 * Checks if we should restrict access to the object based on ip embargoes.
 *
 * @param string $pid
 *   The PID of the object to check.
 *
 * @return bool
 *   True if one should restrict access. False if one should not.
 */
function islandora_ip_embargo_restrict_access($pid) {
  $restricted = &drupal_static(__FUNCTION__ . '_restricted');

  if (!isset($restricted)) {
    if (FALSE !== ($cache = cache_get('islandora_ip_embargo_restrict_access'))) {
      $restricted = $cache->data;
    }
    else {
      $restricted = array();
    }
  }

  if (!empty($restricted) && array_key_exists($pid, $restricted)) {
    return $restricted[$pid];
  }

  $open_access = FALSE;

  $embargo_result = islandora_ip_embargo_get_embargo($pid);
  if ($embargo_result->rowCount()) {
    $embargo_info = $embargo_result->fetchObject();
    $list_results = islandora_ip_embargo_get_lists_information($embargo_info->lid, FALSE);

    if ($list_results->rowCount()) {
      while ($list_info = $list_results->fetchObject()) {
        if (islandora_ip_embargo_ip_in_range($list_info->low_end, $list_info->high_end)) {
          $open_access = TRUE;
        }
      }
    }
    else {
      $open_access = TRUE;
    }
  }
  else {
    $open_access = TRUE;
  }
  $restricted[$pid] = !$open_access;
  cache_set('islandora_ip_embargo_restrict_access', $restricted, 'cache', time() + 3600);
  return !$open_access;
}

/**
 * For a given node, find the nearest ancestor (or self) with an embargo set.
 *
 * @param IslandoraFedoraObject $islandora_object
 *   Islandora object.
 *
 * @return IslandoraFedoraObject
 *   The nearest embargoed ancestor; or NULL.
 */
function islandora_ip_embarga_nearest_embargo($islandora_object) {
  if(!$islandora_object) {
    $e = new Exception();
    watchdog_exception('bad fedora object', $e, "ID not defined for :pid, value :v", array(':pid' => gettype($islandora_object), ':v' => (int)$islandora_object));
    return $islandora_object;
  }
  $pid = $islandora_object->id;

  // Get cached embargoes info.
  $nearest_embargoes = &drupal_static(__FUNCTION__ . '_ne');
  if (!isset($nearest_embargoes)) {
    if (FALSE !== ($cache = cache_get('islandora_ip_embargo_nearest_embargoes'))) {
      $nearest_embargoes = $cache->data;
    }
    else {
      $nearest_embargoes = array();
    }
  }
  if (!empty($nearest_embargoes) && array_key_exists($pid, $nearest_embargoes)) {
    return $nearest_embargoes[$pid];
  }

  $stvar_cycles = __FUNCTION__ . '_cycle_detection';
  $already_seen = &drupal_static($stvar_cycles);
  if (!empty($already_seen) && in_array($pid, $already_seen)) {
    $e = new Exception('this node has already been seen in DFS');
    watchdog_exception('cycle detection', $e, ":pid is part of a cycle in the graph", array(':pid' => $pid));

    drupal_static_reset($stvar_cycles);
    $nearest_embargoes[$pid] = $islandora_object;
    cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
    return $islandora_object;
  }
  else {
    $already_seen[] = $pid;
  }
  $embargo = islandora_ip_embargo_get_embargo($pid);

  if ($embargo->rowCount()) {
    $nearest_embargoes[$pid] = $islandora_object;
    cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
    return $islandora_object;
  }
  else {
    $ancestor_rels = array('isConstituentOf', 'isMemberOfCollection');

    // Try to catch errors of the form `Fatal error: Call to a member function
    // get() on a non-object in
    // /sites/all/modules/islandora_ip_embargo/includes/utilities.inc on line 477`.

    if (!$islandora_object->relationships) {
      $e = new Exception();
      watchdog_exception('bad fedora object', $e, "Relationships not defined for :pid", array(':pid' => $pid));
      drupal_static_reset($stvar_cycles);
      $nearest_embargoes[$pid] = $islandora_object;
      cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
      return $islandora_object;
    }
    $relationships = $islandora_object->relationships->get();
    foreach ($ancestor_rels as $preferred_link) {
      foreach ($relationships as $rel) {
        if ($rel['predicate']['value'] === $preferred_link) {
          $ancestor = islandora_object_load($rel['object']['value']);
          if(!$ancestor) {
            drupal_static_reset($stvar_cycles);
            $nearest_embargoes[$pid] = $islandora_object;
            cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
            return $islandora_object;
          }

          $nearest_embargoes[$pid] = islandora_ip_embarga_nearest_embargo($ancestor);
          cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
          return $nearest_embargoes[$pid];
        }
      }
    }
    drupal_static_reset($stvar_cycles);
    $nearest_embargoes[$pid] = $islandora_object;
    cache_set('islandora_ip_embargo_nearest_embargoes', $nearest_embargoes, 'cache', time() + 3600);
    return $islandora_object;
  }
}

/**
 * Check that record to be created is not already stored.
 *
 * @param int $lid
 *   list ID
 * @param string $low
 *   IPV4 address that starts the allowed range.
 * @param string $high
 *   IPV4 address that ends the allowed range.
 *
 * @return bool
 *   true if an existing record matches the one being tested.
 */
function islandora_ip_embargo_is_duplicate_range($lid, $low, $high) {
  foreach (islandora_ip_embargo_get_ranges($lid) as $range) {
    if ($range->low_end == $low && $range->high_end == $high) {
      drupal_set_message(t("Duplicate IP range. Nothing changed."));
      return TRUE;
    }
  }
}

/**
 * For a given PID, return the array of DSIDs saved as public.
 *
 * @param string $pid
 *   Islandora PID
 *
 * @return array
 *   Simple array with one DSID per element, or empty, if no record exists.
 */
function islandora_ip_embargo_get_public_datastreams($pid) {
  $dsids = array();
  $records = db_select('islandora_ip_embargo_embargoes', 'd')
    ->fields('d', array('public_dsids'))
    ->condition('pid', $pid)
    ->execute();

  if (!$records->rowCount()) {
    $dsids = variable_get('islandora_ip_embargo_global_public_datastreams', array('TN'));
  }
  else {
    $dsids = explode(',', $records->fetchField());
  }

  return $dsids;
}

/**
 * Return an object's DSIDs as an array for a checkboxes form element.
 *
 * @param FedoraObject $islandora_object
 *   Islandora object.
 *
 * @return array
 *   Associative array such as that obtained from drupal_map_assoc().
 */
function islandora_ip_embargo_get_object_datastreams($islandora_object) {

  foreach ($islandora_object as $datastream) {
    $dsid = strtoupper($datastream->id);
    $dsid_options[$dsid] = $dsid;
  }
  return $dsid_options;
}

/**
 * Check if current path is public datastream.
 *
 * @param array $dsids
 *   array of DSIDs, as returned from _get_public_datastreams.
 *
 * @return bool
 *   Return TRUE if current path matches one of the input DSIDs.
 */
function islandora_ip_embargo_requesting_public_dsid(array $dsids) {
  foreach ($dsids as $dsid) {
    if (strpos(current_path(), "datastream/$dsid") !== FALSE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Parse the drush option --expiry for use in CRUD and logging.
 *
 * @param NULL|stirng $expiry
 *   String date as YYYY-MM-DD.
 *
 * @return NULL|int
 *   UNIX timestamp or NULL.
 */
function islandora_ip_embargo_parse_expiry($expiry = NULL) {

  // Set up expiry date from params.
  if ($expiry !== NULL) {
    $date_elements = explode('-', $expiry);
    if (count($date_elements) != 3) {
      drush_set_error("Expiry argument expects the date given as YYYY-MM-DD. Got $date_elements");
      return;
    }
    $expiry_elements = array(
      'year' => $date_elements[0],
      'month' => $date_elements[1],
      'day' => $date_elements[2],
    );
    $expiry = mktime(
        0, 0, 0, $expiry_elements['month'], $expiry_elements['day'], $expiry_elements['year']
    );
  }
  return $expiry;
}

/**
 * Return an array of PIDs, depending on the drush option.
 *
 * @return array
 *   Containing either the value of option --pids, or the array obtained from
 *   passing the contents of option --pidsfile through explode().
 * @throws Exception
 *   Either --pid or --pidsfile must be used, not both.
 */
function islandora_ip_embargo_pid_or_pids() {
  foreach (array('pid', 'pidsfile') as $option) {
    $$option = drush_get_option($option);
  }
  if ($pid Xor $pidsfile) {
    return $pid ? array($pid) : explode(',', file_get_contents($pidsfile));
  }
  else {
    throw new Exception("Must supply either --pid or --pidsfile.");
  }
}

<?php

/**
 * @file
 * Drush interface to ip_embargo functionality.
 */

/**
 * Drush commands declaration.
 *
 * @return array
 *   drush commands data
 */
function islandora_ip_embargo_drush_command() {
  $items = array();
  $items['islandora-ip-embargo-addRange'] = array(
    'description' => 'Add a network address range to the embargo list.',
    'options' => array(
      'low' => array(
        'description' => 'First address in the IP range (e.g, "172.16.0.0").',
        'required' => 'TRUE',
      ),
      'high' => array(
        'description' => 'Last address in the IP range (e.g., "172.16.255.255").',
        'required' => 'TRUE',
      ),
      'list' => array(
        'description' => 'Name of the list to add this range to (will be created if necessary).',
        'required' => 'TRUE',
      ),
    ),
    'aliases' => array('iipeadd'),
  );
  $items['islandora-ip-embargo-list-lists'] = array(
    'description' => 'Returns all the network lists defined on the system.',
    'aliases' => array('iipell'),
  );
  $items['islandora-ip-embargo-remove-embargo'] = array(
    'description' => 'Remove an embargo from islandora objects by pid or list of pids.',
    'options' => array(
      'pid' => array(
        'description' => 'Islandora pid to un-embargo.',
        'required' => FALSE,
      ),
      'pidsfile' => array(
        'description' => 'Path to file containing comma-separated pids.',
        'required' => FALSE,
      ),
    ),
    'aliases' => array('iipere'),
  );
  $items['islandora-ip-embargo-set-embargo'] = array(
    'description' => dt('Set embargo for a single pid or for many (using the pidsfile option).'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
    'drupal dependencies' => array(
      'islandora',
      'islandora_ip_embargo',
    ),
    'aliases' => array('iipese'),
    'options' => array(
      'pidsfile' => array(
        'description' => dt('File containing a comma-separated string of pids'),
        'required' => FALSE,
      ),
      'pid' => array(
        'description' => 'Islandora pid to embargo.',
        'required' => FALSE,
      ),
      'list' => array(
        'description' => dt('The network list to set.'),
        'required' => TRUE,
      ),
      'expiry' => array(
        'description' => dt("When to expire the embargo."),
        'required' => FALSE,
      ),
      'public_dsids' => array(
        'description' => dt("Datastream IDs that should be accessible to roles with the 'Access public datastreams' permission."),
        'required' => FALSE,
      ),
    ),
  );
  return $items;
}

/**
 * Drush command; adds IP range to a named list.
 *
 * Creates the list if it does not already exist.
 *
 * @return NULL
 *   no return
 */
function drush_islandora_ip_embargo_addRange() {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
  $params = array(
    'low' => drush_get_option('low'),
    'high' => drush_get_option('high'),
    'list' => drush_get_option('list'),
  );

  $low = $params['low'];
  $high = $params['high'];
  // Check that input vars are valid IPs.
  foreach (array($low, $high) as $ip) {
    if (filter_var($ip, FILTER_VALIDATE_IP) === FALSE) {
      drush_set_error("Invalid IP: $ip");
      return;
    }
  }

  $target_list = $params['list'];
  $target_list_id = FALSE;
  $lists = islandora_ip_embargo_get_lists();

  foreach ($lists as $list) {
    if ($target_list == $list->name) {
      $target_list_id = $list->lid;
      if (islandora_ip_embargo_is_duplicate_range($list->lid, $low, $high)) {
        return;
      }
      break;
    }
  }
  if (!$target_list_id) {
    islandora_ip_embargo_add_list($target_list);
    $target_list_id = islandora_ip_embargo_get_list_object_by('name', $target_list);
    drupal_set_message(t('Created list %l', array('%l' => $target_list)));
  }

  islandora_ip_embargo_add_ip_range($target_list_id, $low, $high);
}

/**
 * Drush command; prints named lists and their constituent ranges to stdOut.
 */
function drush_islandora_ip_embargo_list_lists() {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');

  $lists = islandora_ip_embargo_get_lists();
  printf("Network lists\n==============\n");
  foreach ($lists as $list) {
    printf("%s\n", $list->name);
    foreach (islandora_ip_embargo_get_ranges($list->lid) as $range) {
      printf("%s - %s\n", $range->low_end, $range->high_end);
    }
    printf("\n");
  }
}

/**
 * Drush command; given a list of pids, embargo each.
 */
function drush_islandora_ip_embargo_set_embargo() {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
  $pids = islandora_ip_embargo_pid_or_pids();
  $expiry = drush_get_option('expiry', NULL);
  $list = drush_get_option('list');
  $dsids = array_map('trim', explode(',', drush_get_option('public_dsids', NULL)));

  $params = array(
    'expiry' => $expiry,
    'public_dsids' => implode(',', $dsids),
  );

  foreach ($pids as $pid) {
    $params['pid'] = $pid;
    $params['list'] = $list;
    islandora_ip_embargo_make_embargo_from_params($params);
  }
}

/**
 * Remove all embargoes from a pid or list of pids.
 */
function drush_islandora_ip_embargo_remove_embargo() {
  module_load_include('inc', 'islandora_ip_embargo', 'includes/utilities');
  $pids = islandora_ip_embargo_pid_or_pids();
  foreach ($pids as $pid) {
    islandora_ip_embargo_remove_embargo($pid);
    drupal_set_message(t(
            "Embargo(es) removed for pid %pid", array('%pid' => $pid))
    );
  }
}

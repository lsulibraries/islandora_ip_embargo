BUILD STATUS
------------
Current build status:
[![Build Status](https://travis-ci.org/Islandora/islandora_ip_embargo.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_ip_embargo)

CI Server:
http://jenkins.discoverygarden.ca

ISLANDORA IP EMBARGO
==================

CONTENTS OF THIS FILE
---------------------

 * summary
 * configuration

SUMMARY
-------

A Drupal based approach to embargoing content based on IP ranges.
This module is not integrated with the Islandora Embargo module.
This module breaks islandora_xml_site_map with its redirects. 

CONFIGURATION
-------------

Drupal cron should be configured to run once a day to clean out expired
embargoes.  While not a dependency more functionality can be found by
installing the Rules https://drupal.org/project/rules module.

COMMANDLINE INTERFACE
---------------------

To see existing IP range lists for a site:

- islandora-ip-embargo-list-lists (iipell)

  `drush iipell`

To add a network range to a list:

- islandora-ip-embargo-addRange (iipeadd)

  `drush iipeadd --list my_ip_list --low nnn.nn.nn.nn --high nnn.nn.nn.nn`


To set or remove an embargo from an item or items (note that these commands take either a single PID or file containing comma-separated PIDs):

- islandora-ip-embargo-set-embargo (iipese)

  `drush iipese --list=iplist_a --pid=namespace:pid --expiry=yyyy-mm-dd`

  `drush iipese --list=iplist_a --pidsfile=/path/to/myListOfPIDs.txt`

- islandora-ip-embargo-remove-embargo

  `drush iipere --pid=namespace:pid`

  `drush iipere --pidsfile=/path/to/myListOfPIDs.txt`

NOTES
---------

A Drupal based approach to embargoing content based on IP ranges.
This module is not integrated with the Islandora Embargo module.
This module breaks islandora_xml_site_map with its redirects. 

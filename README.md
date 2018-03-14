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
 * Public Datastreams

SUMMARY
-------

A Drupal based approach to embargoing content based on IP ranges.
This module is not integrated with the Islandora Embargo module.
This module breaks islandora_xml_site_map with its redirects.

EMBARGO INHERITANCE
-------------------

By taking advantage of RELS-EXT, an embargo set at the collection or compound 
object level applies to its members or constituents until overridden at a lower
level. In a collection with an embargo set, setting an member object's embargo 
list to '' removes the embargo for that item by creating a local record.

CONFIGURATION
-------------

Drupal cron should be configured to run once a day to clean out expired
embargoes.  While not a dependency more functionality can be found by
installing the Rules https://drupal.org/project/rules module.

 * __Djatoka:__
If the IP address of djatoka is not in the whitelisted IP range, jp2s may not 
appear for embargoed items. Set the IP for djatoka at 
`/admin/islandora/tools/ip_embargo/misc`. See Google Groups page 
https://groups.google.com/forum/#!topic/islandora-dev/J4zucPacUU0 for 
some discussion of the problem.

PUBLIC DATASTREAMS
------------------

When you want to restrict access to only some datastreams, use the global and 
item-level configuration pages to specify the datastreams that should be public.
Here, 'public' can be defined by a site administrator by assigning the permission,
`Access datastreams marked as public`; assigning it to `Anonymous user` means that
these datastreams are truly public, that they will not be withheld by the 
mechanisms of this module. Assign to other roles for different behavior.

DRUSH INTERFACE
---------------------

To see existing IP range lists for a site:

- islandora-ip-embargo-list-lists (iipell)

  `drush iipell`

To add a network range to a list:

- islandora-ip-embargo-addRange (iipeadd)

  `drush iipeadd --list=my_ip_list --low=nnn.nn.nn.nn --high=nnn.nn.nn.nn`


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

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

 * __Djatoka:__
If the IP address of djatoka is not in the whitelisted IP range, jp2s may not 
appear for embargoed items. Set the IP for djatoka at 
`/admin/islandora/tools/ip_embargo/misc`. See Google Groups page 
https://groups.google.com/forum/#!topic/islandora-dev/J4zucPacUU0 for 
some discussion of the problem.

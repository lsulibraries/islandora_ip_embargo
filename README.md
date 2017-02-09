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

 * Summary
 * Functionality
 	* Permissions
 	* Admin panel
 	* Object view 	
 * Clean up
 * Command line interface

SUMMARY
-------

Islandora IP Embargo denies access to specific datastreams to specific IP ranges.

PERMISSIONS
-----------

Permissions are granted per role through the Islandora People/Permissions interface.  Four distinct permissions are available:
- "Administer embargoes" permits access to the IP Embargo administration page.
- "Control embargoes" permits CRUD (creation, revision, updating, & deletion) of embargoes.
- "Manage embargoes" permits managing embargoed objects.
- "Access public datastreams" permits access to datastreams marked as public. 

ADMIN PANEL
-----------

Roles with "[which permission]" can access the admin panel at http://yoursite.edu/admin/islandora/tools/ip_embargo .  

Here one can create new "Network Address Lists", which define what IP ranges for allowing embargo.  A list may hold several IP ranges.  All lists are available sitewide to every object.  Each embargoed item may be assigned to only one list, however.

The "Islandora IP Embargo Settings" tab holds customizations.  Important to note -- if jp2 images are blocked from display, your server's public IP address must be named here as the Djatoka public IP.  Here, you can also set the default datastreams available to everyone on embargoed objects.  Thumbnail (TN) is the default, though you may prefer metadata as well by default.

OBJECT VIEW
-------------

Roles with "[which permission]" see an "IP Embargo" tab above an Islandora object.  

Within this tab, you may specify one embargo list (a list of IP ranges created in the admin panel) to the object.  Setting the embargo list to None removes any embargo from the item.  An expiration date (or "Never Expire") is setable, as well as overriding which datastreams to exclude from the embargo.  In some cases, you may intend for no datastreams to bypass the embargo, while in others you may wish to make not only the thumbnail (TN) but also the metadata available to everyone regardless of embargo.  

Settings inherit to all of an object's child objects on first assignment.  If you apply an embargo to a parent compound object, it will apply the same embargo to each of its children.  If you apply an embargo to a collection, it will apply that embargo to every object in that collection (compound or simple).  The current inheritance for the object is displayed in the text "Inherited from ..."

CLEAN UP
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


To embargo an item or items:

- islandora-ip-embargo-set-embargo (iipese)

  `drush iipese --lists iplist_a,iplist_b --pid namespace:pid,namespace:pid --expiry yyyy-mm-dd --public_dsids TN,RELS-EXT,MODS,DC`


NOTES
---------

A Drupal based approach to embargoing content based on IP ranges.
This module is not integrated with the Islandora Embargo module.
This module breaks islandora_xml_site_map with its redirects. 

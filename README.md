# Islandora IP Embargo[![Build Status](https://travis-ci.org/Islandora-Labs/islandora_ip_embargo.png?branch=7.x)](https://travis-ci.org/Islandora-labs/islandora_ip_embargo)

## Introduction

A Drupal based approach to embargoing content based on IP ranges.

## Requirements

 * summary
 * configuration
 * Public Datastreams



The following Drupal modules are required:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Access Override](https://github.com/discoverygarden/islandora_access_override)

### Optional Requirements

The following Drupal modules, while not required, add more functionality to this module:

* [Rules](https://drupal.org/project/rules)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

### Drupal cron configuration

Drupal cron should be minimally configured to run once a day to clean out expired embargoes.

### Network address lists

Go to `yoursite.com/admin/islandora/tools/ip_embargo` to manage network address lists. At least one list is required in order to use IP embargo, because these lists are what will appear in the 'IP address range list' drop-down in the 'IP Embargo' tab for any given object, and an IP embargo can't be set without an IP address range list.

#### Adding a list

1. Click 'Add network list'.
2. Enter a name for the list and click 'Create list'.
3. Now an [IP address range can be added](#adding-an-ip-address-range) if desired.

NOTE: Without an IP address range, no IP addresses will be able to access objects embargoed with that list.

#### Deleting a list

NOTE: If the list to be deleted has an IP address range, [the IP address ranges in that list must be deleted](#deleting-an-ip-address-range) before the list can be deleted.

1. Select a network list.
2. Click the 'Delete' below the table of network lists.

#### Adding an IP address range

IP address ranges can be specified to be allowed to access embargoed items that use a certain list by adding an IP address range to that list. Without an IP address range, no IP addresses will be able to access objects embargoed with that list.

1. Click on the list to be edited.
2. Click 'Add IP address range'.
3. Enter the IP address range of the addresses to be ALLOWED to access any object that gets embargoed with this list.

#### Deleting an IP address range

1. Click on the list to be edited.
2. Select the range(s) to delete.
3. Click the 'Delete ranges' button below the table of ranges.

### Module settings

Islandora IP Embargo settings can be edited in a form located at `yoursite.com/admin/islandora/tools/ip_embargo/misc`. This page allows an admin to set a custom redirect page for embargoed objects, set the number of days before an embargo alert, set the overlay text for embargoed items, and set the colour of the overlay text on embargoed items.

## Usage

### Embargoing objects

1. Go to the object to be embargoed; this object can also be a collection.
2. Click the 'IP Embargo' tab, fill out the form, and click 'Set Embargo'.
3. Now this object will appear in the [managing embargoed objects form](#manage-embargoed-objects)

### Manage embargoed objects

Go to `yoursite.com/admin/islandora/tools/ip_embargo/manage`. From there, objects can be selected and removed from their embargo by clicking the 'Delete' button. This page is also useful for finding embargoed objects to edit or examine by clicking on the desired object in the table.

## Troubleshooting

This module is not integrated with the Islandora Embargo module.
This module breaks islandora_xml_site_map with its redirects.

EMBARGO INHERITANCE
-------------------

By taking advantage of RELS-EXT, an embargo set at the collection or compound
object level applies to its members or constituents until overridden at a lower
level. In a collection with an embargo set, setting an member object's embargo
list to '' removes the embargo for that item by creating a local record.

## Maintainers/Sponsors

Current maintainers:

* [William Panting](https://github.com/willtp87)
* [discoverygarden](https://github.com/discoverygarden)

## License

<<<<<<< HEAD
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
=======
[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
>>>>>>> upstream/7.x

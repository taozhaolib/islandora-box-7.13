# Islandora Internet Archive BookReader [![Build Status](https://travis-ci.org/Islandora/islandora_internet_archive_bookreader.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_internet_archive_bookreader)

## Introduction

A Islandora wrapper for the Internet Archive BookReader.

This module doesn't do much on its own, its assummed that it will be used in conjunction with a solution pack, where it will be provided as a viewer.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Djatoka](http://sourceforge.net/apps/mediawiki/djatoka/index.php?title=Main_Page)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search/) (Optional)


## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

Download/clone the [Internet Archive BookReader](https://github.com/Islandora/internet_archive_bookreader.git) to `sites/all/libraries/bookreader`.

Internet Archive BookReader [Developer documentation](http://openlibrary.org/dev/docs/bookreader)

This module requires that you set up Djatoka, please follow the steps outlined at [here](https://wiki.duraspace.org/pages/viewpage.action?pageId=34658947).

## Configuration

Set the 'djatoka image compression level', 'Solr field relating pages to book PIDs ', 'Overlay Opacity', and select the 'Default page view' in Administration » Islandora » Internet Archive BookReader (admin/islandora/internet_archive_bookreader).

![Configuration](http://i.imgur.com/R3AhKpB.png)

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Alan Stanley](https://github.com/ajstanley)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

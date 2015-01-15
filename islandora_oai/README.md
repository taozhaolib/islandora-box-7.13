# Islandora OAI [![Build Status](https://travis-ci.org/Islandora/islandora_oai.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_oai)

## Introduction

This module provides an implementation the Open Archives Initiative Protocol for Metadata Harvesting (OAI-PMH) for Islandora fedora repository with a solr index. By implementing the islanodra_oa module, you can expose content (its metadata) as an OAI-PMH repository. It will then be accessible by OAI harvesters For further OAI documentation. For more info, please see [this]( http://www.openarchives.org/OAI/openarchivesprotocol.html) link.

FUNCTIONALITY
--------------

Receive OAI-PMH request of service providers in form of HTTP request (GET or POST)
Handle the OAI-PMH request
Respond to OAI-PMH request in form of HTTP response (XML) to the service provider

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Solr Search](https://github.com/Islandora/islandora_solr_search/)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the basic details of the repository in Administration » Islandora » Islandora OAI (admin/islandora/islandora-oai).

![Configuration](http://i.imgur.com/fDCZm5U.png)

After you have exposed content types and some fields, your repository is available at /oai2

Some example requests are as follows:

* `*/oai2?verb=Identify`
* `*/oai2?verb=ListMetadataFormats`
* `*/oai2?verb=ListIdentifiers&metadataPrefix=oai_dc`
* `*/oai2?verb=GetRecord&metadataPrefix=oai_dc&identifier=PID`
* `*/oai2?verb=ListRecords&metadataPrefix=oai_dc`

If the XACML module is present you will need to configure the `rels.isViewableBy` fields in the admin page such that the OAI requests respect these object restrictions.

### Customization

By default the vanilla islandora_oai module provides a very basic output. It is possible to add additional content to the description field of the repository. This includes pointing at other harvesters and repositories, branding information etc. An example of how to implement these can be referenced in the [6.x version of the module](https://github.com/Islandora/islandora_oai/blob/6.x/islandora_oai.module#L534-L604).

### Notes

The original 6.x version of this module was based off of the OAI2ForCCK module located [here](http://drupal.org/project/oai2forcck).

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Jordan Dukart](https://github.com/jordandukart)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)


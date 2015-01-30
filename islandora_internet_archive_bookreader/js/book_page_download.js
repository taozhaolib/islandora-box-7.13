/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 * @file
 * Javascript file for islandora solr search facets
 */

(function ($) {

  // Adds facet toggle functionality
  Drupal.behaviors.addDownloadLinksDiv = {
    attach: function(context, settings) {
      
      var url_medium_size = Drupal.settings.islandora_internet_archive_bookreader.medium_size;
      var url_thumbnail = Drupal.settings.islandora_internet_archive_bookreader.thumbnail;
      var url_full_tiff = Drupal.settings.islandora_internet_archive_bookreader.tiff_size;
      var downloadLinksDiv = $("<div id='book-page-download-links'></div>");
      //var showDownloadBtn = $('<button id="book-page-download-links-btn" >');
      var showDownloadBtn = $('<label id="book-page-download-links-btn" >');
      var showDownloadBtnTxt = $('<span id="book-page-download-btntxt-show" class="book-page-download-btntxt"> Page image download links &gt;&gt;</span>');
      var hideDownloadBtnTxt = $('<span id="book-page-download-btntxt-hide" class="book-page-download-btntxt" > &lt;&lt; Hide page download links</span>');
      var downloadLinksTable = $("<div id='downloadTableDiv'><table id='downloadLinksTable'><tr>"+
                          "<td><a class=\"download_link\" href=\""+url_thumbnail+"\" >Thumbnail Image</a></td>"+
                          "<td><a class=\"download_link\" href=\""+url_medium_size+"\" >Medium Sized JPEG</a></td>"+
                          "<td><a class=\"download_link\" href=\""+url_full_tiff+"\" >Full Size TIFF</a></td>"+
                          "</tr></table></div>");
    
      if(! $("#book-page-download-links").length>0) {
          $("#book-page-viewer").append("<BR>").append(downloadLinksDiv).append("<BR>");
      }
      showDownloadBtn.prepend(showDownloadBtnTxt);
      downloadLinksDiv.prepend(showDownloadBtn).append(downloadLinksTable);
      
//      showDownloadBtn.click(function(){
//         downloadLinksTable.toggle('1500');
//         showDownloadBtnTxt.toggle();
//         hideDownloadBtnTxt.toggle();
//      }).prepend(showDownloadBtnTxt).prepend(hideDownloadBtnTxt);
      
      hideDownloadBtnTxt.hide();
      //downloadLinksTable.hide();
      downloadLinksTable.find('td').addClass('downloadLinksTableCell');
    }
  }

})(jQuery);


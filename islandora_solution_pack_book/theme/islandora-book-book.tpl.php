<?php
/**
 * @file
 * Template file to style output.
 */
?>
<?php if (isset($viewer)): ?>
  <div id="book-viewer">
    <?php print $viewer; ?>
  </div>
<?php endif; ?>
<!-- @todo Add table of metadata values -->
<br><br>
<div><span id="div-heading-bookmetadata">The metadata of the book:</span></div>
<div id="div-bookmetadata">
<!--    <table>
        <tr>
            <td>
                The Information about this book:
            </td>
        </tr>
    </table>-->
</div>

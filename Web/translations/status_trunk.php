<?php
  include('../engine/engine.inc');
  include('translations.inc');

  $page = new Page;
  $page->addRssFeed('status_trunk_rss.php');
  $page->printHead('Translations Status (Unstable Trunk) - Frhed');
  
  $page->printRssHeading('Translations Status (Unstable Trunk)', 'status_trunk_rss.php');
  try {
    $status = New TranslationsStatus('status_trunk.xml');
    $status->svnUrl = 'http://frhed.svn.sourceforge.net/viewvc/frhed/trunk/Translations/';
    $status->printTOC();
    $status->printStatus();
    $status->printTranslators();
  }
  catch (Exception $ex) { //If problems with translations status...
    print("<p>The translations status is currently not available...</p>\n");
  }
  $page->printFoot();
?>
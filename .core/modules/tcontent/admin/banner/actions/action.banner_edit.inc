<?php
/**
 * @var $this NewsNews
 */

use Ekv\News\NewsRoot;
use Ekv\News\Tags\NewsTagsRoot;

$bannerID = $this->bannerID;

$sql_where = "
    `id` = '{$bannerID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
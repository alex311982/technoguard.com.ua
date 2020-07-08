<?php
/**
 * @var $this NewsNews
 */

use Ekv\News\NewsRoot;
use Ekv\News\Tags\NewsTagsRoot;

$directionheaderID = $this->indexblockID;

$sql_where = "
    `id` = '{$directionheaderID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
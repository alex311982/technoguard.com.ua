<?php
/**
 * @var $this NewsNews
 */

use Ekv\News\NewsRoot;
use Ekv\News\Tags\NewsTagsRoot;

$directionquoteID = $this->directionquoteID;

$sql_where = "
    `id` = '{$directionquoteID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
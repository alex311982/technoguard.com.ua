<?php
/**
 * @var $this NewsNews
 */

use Ekv\News\NewsRoot;
use Ekv\News\Tags\NewsTagsRoot;

$aboutID = $this->aboutID;

$sql_where = "
    `id` = '{$aboutID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
<?php
/**
 * @var $this NewsNews
 */

use Ekv\News\NewsRoot;
use Ekv\News\Tags\NewsTagsRoot;

$newsID = $this->newsID;

$sql_where = "
    `id` = '{$newsID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->displayEditTpl();
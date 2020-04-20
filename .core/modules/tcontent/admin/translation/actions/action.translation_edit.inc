<?php

$translationID = $this->translationID;

$sql_where = "
    `id` = '{$translationID}'
";
$result = $this->DB->RetrieveRecord('', '', '', $sql_where);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
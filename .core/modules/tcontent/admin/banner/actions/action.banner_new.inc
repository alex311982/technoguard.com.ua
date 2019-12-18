<?php

$result = $this->DB->AddNew();

//$_POST["_input_is_active"] = 1; // make is_active selected by default
$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');

?>

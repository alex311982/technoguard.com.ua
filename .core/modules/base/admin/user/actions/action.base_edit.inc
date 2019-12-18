<?php
    $result = $this->DB->RetrieveRecord($this->id, "id");

	$this->TML->assign('result', $result);
	$this->Display('common:page.edit.tpl');
?>

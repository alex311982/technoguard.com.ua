<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$result = $this->DB->RetrieveRecord((int)$this->id);
	//$result = $this->DB->RetrieveRecord($this->id);
	$this->TML->assign('result', $result);
	$this->Display('common:page.edit.tpl');
?>

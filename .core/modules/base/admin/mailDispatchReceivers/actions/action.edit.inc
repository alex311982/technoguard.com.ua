<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */

	$result = $this->DB->RetrieveRecord($this->id);
	
	$this->TML->assign('result', $result);
	$this->Display('common:page.edit.tpl');
?>

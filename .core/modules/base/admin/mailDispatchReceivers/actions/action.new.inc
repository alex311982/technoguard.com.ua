<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */

	$result = $this->DB->AddNew();
	$this->TML->assign('result', $result);
	$this->Display('common:page.edit.tpl');

?>

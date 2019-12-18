<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
    //--- assigns smarty flag for enabling send button in the toolbar ---//
    $this->canBeSendButtonDisplayedOnEditPages();

	$result = $this->DB->RetrieveRecord($this->id);
	
	$this->TML->assign('result', $result);
	$this->TML->assign("custom_toolbar", "common:toolbars/mail_dispatch/edit.tpl");
	$this->Display('common:page.edit.tpl');
?>

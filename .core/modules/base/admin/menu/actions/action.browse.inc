<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
 	$this->AddTextMessage('Please note, only 2 levels will be shown in the menu');
 	if(defined('SUPER_ADMIN') && SUPER_ADMIN=='yes')
	  $result = $this->DB->RetrieveRecords('', array('parent_id' => $this->ParentId));
  else
    header('Location:/'.SITE_BACKEND);
	$this->TML->assign('result', $result);
	$this->Display('common:page.browse.tpl');
?>

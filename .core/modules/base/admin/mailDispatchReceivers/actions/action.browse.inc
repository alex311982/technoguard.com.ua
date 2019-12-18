<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
    $this->manageAccessDependencies();
	$where = array();
	$this->DB->default_order = array(
								"date"	=>	"DESC"
							);
	$where = array();	
	$result = $this->DB->RetrieveRecords("", $where);
	$this->TML->assign('result', $result);
	$this->Display('common:page.browse.tpl');
?>

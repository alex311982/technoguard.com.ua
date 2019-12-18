<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
	$where = array();
	$this->DB->default_order = array(
								"date_creation"	=>	"DESC"
							);

	$where = array();
	$result = $this->DB->RetrieveRecords("", $where);
	$this->TML->assign('result', $result);


    $this->TML->assign("toolbar_create_disable", 1 );
    $this->TML->assign("toolbar_edit_disable", 1 );
	$this->Display('common:page.browse.tpl');
?>

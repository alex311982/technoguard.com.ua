<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
 	$where = array();
	$result = $this->DB->RetrieveRecords("", $where);
		
	if(!empty($result["records"])){		
//		foreach ($result["records"] as &$val){
//		}
	}

	$this->TML->assign('result', $result);
	$this->Display('common:page.browse.tpl');
?>

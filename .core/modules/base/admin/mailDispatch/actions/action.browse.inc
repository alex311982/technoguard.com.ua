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
								"date"	=>	"DESC"
							);
 	if ( @$_REQUEST['delivery_status'] == "success"){
		$this->AddTextMessage("Рассылка произведена успешно");
	}else if(isset($_REQUEST["task_add"])){
        if($_REQUEST["task_add"] == "success"){
            $this->AddTextMessage("Рассылка поставлена в очередь");
        }
    }


	$where = array();
	$result = $this->DB->RetrieveRecords("", $where);
	$this->TML->assign('result', $result);

	//$this->TML->assign("custom_toolbar", "common:toolbars/mail_dispatch/browse.tpl");
    if( $this->canBeSendButtonDisplayed() ){
        $this->TML->assign("manual_toolbar_block", "mailDispatch/send.tpl" );
    }
	$this->Display('common:page.browse.tpl');
?>

<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
    $this->manageAccessDependencies();
	$this->ResetMessages();
	$this->ResetErrors();
	$VARS = $this->get_parsed_vars();
	$VARS["email"] = trim($VARS["email"]);

	$id = (int)$VARS['id'];
	//#------------------- edit -------------------#//
	if($id > 0)
	{
		$id = $this->DB->UpdateRecord($id, $VARS);
        
        if($this->IsError() == false){
            EkvAdminLog::addLogRecord($id, $VARS["email"], "edit");
        }
	}

	//#------------------- new item -------------------#//
	else
	{
		$VARS["hash"] = MailDispatch::getEmailHash($VARS["email"]);
		$VARS["date"] = phpdate2mysqldate(time());
		$is_err = false;
		if(empty($VARS["email"])){
			$this->DB->AddManualError("Зполните поле 'Email'");
			$is_err = true;
		}
		elseif( MailDispatch::checkEmailCorrectness($VARS["email"]) == false ){
			$this->DB->AddManualError("Некорректный email");
			$is_err = true;
		}
		$id = $this->DB->InsertRecord($VARS);
        
		if($id > 0){
			EkvAdminLog::addLogRecord($id, $VARS["email"], "create");
		}
	}
	

	
	if($this->IsError())
	{
		//$this->AddTextError(PAGES_EXISTS);
		$result = $this->DB->AddNew();
		$result['record'] = $result['fields'];
		$this->DB->push_vals_to_fields($this->get_incoming_vars(), &$result['record']);
		$this->TML->assign('result', $result);
		$this->Display('common:page.edit.tpl');
	}
	else
	{
		$url = '/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/browse'.PAGE_EXTENSION;
		force_location($url);
	}

?>

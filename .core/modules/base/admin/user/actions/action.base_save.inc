<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2010
 *    Author: xxx xxx
 *   Purpose:
 * ============================================================
 */

   	$table = $this->DB->table;
    $table_pk = "id";
	$VARS = $this->get_parsed_vars();

	$id_item = @(int)$VARS[$table_pk];

   unset($VARS["reg_date"]);
    unset($VARS["hash"]);

	//#------------------- EDIT -------------------#//
	if($id_item > 0)
	{
		$old_info = $this->DB->BGetInfoById($table, $id_item, $table_pk);
        if( fCheckEmail($VARS["email"]) == false ){
            $this->DB->AddManualError("Некорректный email");
        }

		$id = $this->DB->UpdateRecord($id_item, $VARS, "", $table_pk);
	}
	//#------------------- INSERT -------------------#//
	else
	{
        /*
        $VARS["enabled"] = 1;
		$VARS["date"] = phpdate2mysqldate(time());
		$id = $this->DB->InsertRecord($VARS);
         * 
         */
	}

	if($id > 0 && !$this->IsError())
	{
        
		$url = compose_url_backend('/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/base_browse');
		$extra_url = "";
		$this->ChangeLocation($url, $extra_url);
	}
	else
	{
		$result = $this->DB->AddNew();
		$this->TML->assign('result', $result);
		$this->Display('common:page.edit.tpl');
	}

?>

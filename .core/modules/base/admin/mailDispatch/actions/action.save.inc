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

 	$err = array();
	$VARS = $this->get_parsed_vars();
	$base_path = $this->base_file_upload_path;
	$id_delivery = (int)$VARS['id'];
	// checking up is user trying to upload file
	$file_is_uploading = false;
	foreach ($_FILES as $k=>$val){
		if ($val['error'] != 4){
			$file_is_uploading = true;
		}
	}
    //pa($_REQUEST);	
	
	
	// DO UPDATE
	if($id_delivery > 0)
	{
		// get old info
		$old_info = $this->DB->BGetInfoById($this->DB->table, $id_delivery );
		
		$return_names = $this->UploadAttachingFiles($err);

		if (!empty($return_names) && $file_is_uploading == true ){
			foreach ($return_names as $k=>$val){
				if (!empty($val)){
					$VARS[$k] = $val;
				}			
			}
		}
		$id = $this->DB->UpdateRecord((int)$VARS['id'], $VARS);
		
		//if error - delete uploaded files
		if ($this->IsError()){
			foreach ($return_names as $k=>$val){
				if (!empty($val)){
					$path_full = $base_path.$val;
					unlink($path_full);
				}
			}
		}elseif( !$this->IsError() && $file_is_uploading == true ){ //delete old photos of delivery
			foreach ( $return_names as $k=>$val ){
				//delete old only if new is uploaded
				if (!empty($val)){
					//echo "<h1>val = $val</h1>";
					$old_image = $old_info[$k]; //$old_info['file1']
					$path_full = $base_path.$old_image;
					@unlink($path_full);
				}
			}
		}elseif($this->IsError() == false){

        }
	}
	// DO INSERT
	else
	{
		$VARS['date'] = phpdate2mysqldate(time());
		$VARS['status'] = "pending";
		
		// uploading
		$return_names = $this->UploadAttachingFiles($err);
		//pa($return_names);
		if (!empty($return_names) && $file_is_uploading == true ){
			foreach ($return_names as $k=>$val){
				if (!empty($val)){
					$VARS[$k] = $val;
				}			
			}
		}
		
		$id = $this->DB->InsertRecord($VARS);
		$id_delivery = $id;
		//if error - delete uploaded files
		if ($this->IsError()){
			foreach ($return_names as $k=>$val){
				if (!empty($val)){
					$path_full = $base_path.$val;
					unlink($path_full);
				}
			}
		}else{
            
        }
	}
	
	if($id > 0 && !$this->IsError())
	{
		$url = "{$this->base_module_url}/browse.html";
		// save and make delivery at once
		if (@$_REQUEST['do_action'] == "send" && $this->canBeSendButtonDisplayed()){
			$url = "{$this->base_module_url}/send.html?id=$id_delivery";
		}
        elseif(@$_REQUEST["do_action"] == "put2queue"){
            $url = "{$this->base_module_url}/put2queue.html?id={$id_delivery}";           
        }
		//$this->ChangeLocation($url);
		force_location($url);
	}
	else
	{
		$result = $this->DB->AddNew();
		$this->TML->assign('result', $result);
		$this->TML->assign("custom_toolbar", "common:toolbars/mail_dispatch/edit.tpl");
		$this->Display('common:page.edit.tpl');
	}

?>
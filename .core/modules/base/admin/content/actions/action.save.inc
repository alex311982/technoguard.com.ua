<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$VARS = $this->get_parsed_vars();

	$id = (int)$VARS['id'];
	//#------------------- edit -------------------#//
	if($id > 0)
	{
		$id = $this->DB->UpdateRecord($id, $VARS);
	}

	//#------------------- new item -------------------#//
	else
	{
		$id = $this->DB->InsertRecord($VARS);
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

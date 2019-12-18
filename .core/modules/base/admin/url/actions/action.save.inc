<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$VARS = $this->get_parsed_vars();
	if($VARS['id'] > 0)
	{
		$id = $this->DB->UpdateRecord((int)$VARS['id'], $VARS);
	}
	else
	{
		$id = $this->DB->InsertRecord($VARS);
	}

	if($id > 0)
	{
		$url = '/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/browse'.PAGE_EXTENSION;
	}
	else
	{
		$_REQUEST = $this->get_incoming_vars();
		$url = '/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/edit'.PAGE_EXTENSION;
	}
	force_location($url);

?>

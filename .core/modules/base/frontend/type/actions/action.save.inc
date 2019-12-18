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
		$url = compose_url_backend('/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/browse');
		force_location($url);
	}
	else
	{
		$this->Display('common:page.edit.tpl');
	}

?>

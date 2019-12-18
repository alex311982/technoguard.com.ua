<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$VARS = $this->get_parsed_vars();

	$VARS['key'] = str_replace (" ", "_", $VARS['key']);
  $VARS['key'] = str_replace('"', '', $VARS['key']);
	$VARS['key'] = str_replace('"', '', $VARS['key']);
	$check = $this->DB->RetrieveRecord($VARS['key'], 'key');

	if(!empty($check['record']) && $check['record']['id']['value'] != @$VARS['id'])
	{
		$id = 0;
	}
	else
	{
		if($VARS['id'] > 0)
		{
			$id = $this->DB->UpdateRecord((int)$VARS['id'], $VARS);
		}
		else
		{
			$id = $this->DB->InsertRecord($VARS);
		}
	}

	if(empty($id))
	{
		$this->AddTextError(SETTINGS_EXISTS);
		$result = $this->DB->AddNew();
		$result['record'] = $result['fields'];
		$this->DB->push_vals_to_fields($this->get_incoming_vars(), $result['record']);
		$this->TML->assign('result', $result);
		$this->Display('common:page.edit.tpl');
	}
	else
	{
		$url = '/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/browse'.PAGE_EXTENSION;
		force_location($url);
	}
?>

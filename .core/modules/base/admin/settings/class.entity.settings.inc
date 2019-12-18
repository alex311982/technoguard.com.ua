<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class BaseSettings extends Entity
	{
		function BaseSettings()
		{
			parent::__construct();
			if(!empty($_REQUEST['_input_id'])) $this->id = (int)$_REQUEST['_input_id'];
		}

		function SetBreadcrump()
		{
			$breadcrumps = array();
			$breadcrumps []= array('title' => SETTINGS_BREADCRUMP,
						'href' => 'browse');
			if($this->id > 0)
			{
				$details = $this->DB->RetrieveRecord((int)$this->id);
				$breadcrumps []= array('title' => $details['record']['title']['value'],
							'href' => 'edit?id='.$details['record']['id']['value']);
			}
			elseif( ($this->action == 'new') || ($this->action == 'save') )
			{
				$breadcrumps []= array('title' => SETTINGS_CREATE,
							'href' => 'new');
			}
			return $breadcrumps;
		}
	}

?>

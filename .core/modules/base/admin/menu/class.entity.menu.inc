<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class BaseMenu extends Entity
	{
		var $ParentId = 0;
		var $RedirectVars = array('ParentId');
		
		function BaseMenu()
		{
			parent::__construct();
			if(isset($_REQUEST['ParentId']))
				$this->ParentId = (int)$_REQUEST['ParentId'];
		}
		
		function SetBreadcrump()
		{
			$breadcrumps = array();
			$breadcrumps []= array('title' => 'Backend Menu',
									'href' => '../../base/menu/browse');

			if($this->ParentId > 0)
			{
				$r = $this->DB->GetBackendMenu();
				$path = $this->DB->GetPath($r, $this->ParentId);
				foreach($path as $p)
				{
					$breadcrumps []= array('title' => $p['title'],
											'href' => 'browse?ParentId='.$p['id']);
				}

				$details = $this->DB->RetrieveRecord($this->ParentId);
				$breadcrumps []= array('title' => $details['record']['title']['value'],
										'href' => 'browse?ParentId='.$details['record']['id']['value']);
				
			}
			
			if($this->id > 0)
			{
				$details = $this->DB->RetrieveRecord((int)$this->id);
				$breadcrumps []= array('title' => $details['record']['title']['value'],
										'href' => 'browse?ParentId='.$details['record']['id']['value']);
				
			}
			return $breadcrumps;
		}
	}

?>

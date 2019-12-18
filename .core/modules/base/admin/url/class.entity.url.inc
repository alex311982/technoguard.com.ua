<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class BaseUrl extends Entity
	{
		function BaseUrl()
		{
			parent::__construct();
		}

		function SetBreadcrump()
		{
			$breadcrumps = array();
			$breadcrumps []= array('title' => 'Url',
						'href' => '../../base/url/browse');
			if($this->id > 0)
			{
				$details = $this->DB->RetrieveRecord((int)$this->id);
				$breadcrumps []= array('title' => $details['record']['title']['value'],
							'href' => 'edit?id='.$details['record']['id']['value']);

			}
			return $breadcrumps;
		}
	}

?>

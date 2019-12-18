<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class BaseType extends Entity
	{
		function BaseType()
		{
			parent::__construct();
		}

		function SetBreadcrump()
		{
			$breadcrumps = array();
			$breadcrumps []= array('title' => 'User types',
						'href' => 'browse');
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

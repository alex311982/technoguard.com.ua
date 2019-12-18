<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
 	class BaseMail extends Entity
	{
		function BaseMail()
		{
			parent::__construct();
		}

		function SetBreadcrump()
		{
			$breadcrumps = array();
			$breadcrumps []= array('title' => 'Email templates',
						'href' => 'browse');
			if($this->id > 0)
			{
				$details = $this->DB->RetrieveRecord((int)$this->id);
				$breadcrumps []= array('title' => $details['record']['key']['value'],
							'href' => 'edit?id='.$details['record']['id']['value']);

			}
			return $breadcrumps;
		}
	}

?>

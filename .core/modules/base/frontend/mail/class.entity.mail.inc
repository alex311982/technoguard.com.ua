<?php
/**
 * ============================================================
 * File name: xxx.xxx
 * Date: xx/xx/2009
 * Url:	
 * Purpose: 
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

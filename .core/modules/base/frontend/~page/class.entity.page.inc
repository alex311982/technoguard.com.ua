<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class BasePage extends Entity
	{
		function BasePage()
		{
			parent::__construct();
		}

		function IsPage($url)
		{
			$page = $this->Db->GetPage($url);
			if(empty($page)) return false;
			else return true;
		}

	}

?>

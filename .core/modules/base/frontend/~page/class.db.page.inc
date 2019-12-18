<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBasePage extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_page';
			$this->fields = include('fields.inc');
		}

		function GetPage($url)
		{
      $url = str_replace('"', '', $url);
			if(empty($url)) return;
			
			$sql = '
				SELECT
					*
				FROM
					`'.$this->table.'`
				WHERE
					`url` = "'.$url.'"
				LIMIT 0, 1
				';
			
			$this->query($sql);
			$result = '';
			$result = $this->fetch_row();
			return $result;
		}
	}

?>

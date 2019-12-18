<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBaseContent extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_content';
			$this->fields = include('fields.inc');
		}
		
		function deleteCurrentLangItem($mid, $lang_id)
		{
			$mid = (int)$mid;
			$lang_id = (int)$lang_id;
			
			$sql = "
				DELETE FROM `{$this->table}`
				WHERE
					`mid` = $mid AND
					`lang_id` = {$lang_id}
			";
			$this->query($sql);
		}
		
	}

?>

<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBaseSettings extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_settings';
			$this->fields = include('fields.inc');
		}
	}

?>

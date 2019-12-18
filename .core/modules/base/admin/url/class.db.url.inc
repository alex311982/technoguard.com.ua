<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBaseUrl extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_url';
			$this->fields = include('fields.inc');
		}
	}

?>

<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBaseType extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_type';
			$this->fields = include('fields.inc');
		}
	}

?>

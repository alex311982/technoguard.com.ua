<?php
/**
 * ============================================================
 * File name: xxx.xxx
 * Date: xx/xx/2009
 * Url:	
 * Purpose: 
 * ============================================================
 */
	class DbBaseMail extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_mail';
			$this->fields = include('fields.inc');
		}
	}

?>

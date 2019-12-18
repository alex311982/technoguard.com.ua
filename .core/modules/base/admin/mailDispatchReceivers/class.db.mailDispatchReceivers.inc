<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2008
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */

	class DbbasemailDispatchReceivers  extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_mail_dispatch_receivers';
			$this->fields = $this->IncludeSpecificFields(__FILE__, "dispatch_receivers");
		}		
	

	}

?>

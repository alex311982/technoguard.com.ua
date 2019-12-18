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

		function GetTableType($type)
		{
      $result = $this->RetrieveRecord ($type, 'title');
			if (!empty ($result))
			{
				return $result ['record']['tablename']['value'];
			}
			return "";
		}

		function GetTypeId($type)
		{
			$result = $this->RetrieveRecord($type, 'title');
			if (!empty ($result))
			{
				return $result ['record']['id']['value'];
			}
			return 0;
		}
	}

?>

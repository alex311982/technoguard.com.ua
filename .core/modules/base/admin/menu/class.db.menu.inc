<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	class DbBaseMenu extends DB
	{
		function __construct()
		{
			parent::__construct();
			$this->table = 'base_backend_menu';
			$this->fields = include('fields.inc');
		}
		
		function GetBackendMenu()
		{
            if(isset($_SESSION["user"]) && $_SESSION["user"]->role_id == EkvBackendAdmins::ROLE_SUPER)
            {
                $sql = '
                    SELECT
                        *
                    FROM
                        `'.$this->table.'`
                    WHERE
                        `is_active` = 1
                    ORDER BY
                        `order_no`
                    ';
            }
            else
            {
                $sql = "
                        SELECT
                            *
                        FROM
                            `$this->table`
                  WHERE
                    `super_admin`<>'yes'
                        ORDER BY
                            `order_no`
                ";
            }
			
			$this->query($sql);
			while($row = $this->fetch_row())
			{
				$result[$row['id']] = $row;
			} 
			if(empty($result)) $result = include('menu.inc');
			
			return $result;
		}
		
		function AddNew()
		{
			$result = parent::AddNew();
			return $result;
		}
		

		
	}

?>

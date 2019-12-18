<?php

	class DbBase extends ErrorCustom
	{
		var $obj = array (
			"sql_database"	=> "",
			"sql_user"	=> "root",
			"sql_pass"	=> "",
			"sql_host"	=> "localhost",
			"sql_port"	=> "",
			"persistent"	=> "0",
			"sql_tbl_prefix"=> "",
			"cached_queries"=> array(),
			'debug'		=> 0,
			);

		var $query_id		= "";
		var $connection_id	= null;
		var $query_count	= 0;
		var $record_row		= array();
		var $return_die		= 0;
		var $error		= "";
		var $error_no		= 0;
		var $failed		= 0;

		function __construct($host, $base, $user, $pass, $port, $table_prefix)
		{
			parent::__construct();
			$this->obj['sql_host'] = $host;
			$this->obj['sql_database'] = $base;
			$this->obj['sql_user'] = $user;
			$this->obj['sql_pass'] = $pass;
			$this->obj['sql_port'] = $port;
			$this->obj['sql_tbl_prefix'] = $table_prefix;

			$this->connect();
		}

        function setReturnDie($val)
        {
            $this->return_die = $val;
        }

		/*========================================================================*/
		// Connect to the database
		/*========================================================================*/
		function connect()
		{
			$this->connection_id = mysqli_connect( $this->obj['sql_host'], $this->obj['sql_user'], $this->obj['sql_pass']);
//var_dump($this->connection_id);
			if ( !mysqli_select_db($this->connection_id, $this->obj['sql_database']) )
				trigger_error ("Could not connect to the database: <b>{$this->obj['sql_database']}</b>", E_USER_ERROR);

			$charset = defined('DB_CHARSET')?DB_CHARSET:'utf8';
			// hack for 1251 charset
   			if ($charset == "cp1251"){
				$this->query("set character_set_client='cp1251'");
				$this->query( "set character_set_results='cp1251'");
				$this->query( "set collation_connection='cp1251_general_ci'");
			}else{
				$this->query("SET CHARACTER SET '".$charset."'");
				$this->query("SET character_set_client='".$charset."'");
				$this->query("SET character_set_results='".$charset."'");
				$this->query("SET collation_connection='".$charset."_general_ci'");
			}

   			//var_dump($this->connection_id);exit;
		}

		function connect_outer_db($DB_NAME = "",
			$connection_params =
				array(
					"host" => SCHOOL_DB_HOST,
					"user"	=> SCHOOL_DB_USER,
					"pass"	=> SCHOOL_DB_PASS,
					"charset" => SCHOOL_DB_CHARSET,
				)
			)
		{
			if(empty($DB_NAME)){
				$DB_NAME = SCHOOL_DB_BASE;
			}
			$this->connection_id = mysqli_connect( $connection_params["host"], $connection_params["user"], $connection_params["pass"] );

			if ( !mysqli_select_db($this->connection_id, $DB_NAME) )
				trigger_error ("Could not connect to the OUTER database: <b>{$this->obj['sql_database']}</b>", E_USER_ERROR);

			$charset = defined('DB_CHARSET')?DB_CHARSET:'utf';
			//$this->query("SET CHARACTER SET '".$charset."'");
			if($charset == "cp1251"){
				$this->query( "set character_set_client='cp1251'");
				$this->query( "set character_set_results='cp1251'");
				$this->query( "set collation_connection='cp1251_general_ci'");
			}
		}

		/*========================================================================*/
		// Process a query
		/*========================================================================*/
		function query($the_query, $bypass=0)
		{
			//--------------------------------------
			// Change the table prefix if needed
			//--------------------------------------
//			if(empty($this->connection_id)){
//                $this->connect();
//            }
///*
            //--- hard resolving bug, catch each using of date_mod (only writing - upd or ins) ---//
            if(
                stripos($the_query, "date_mod") !== false
                &&
                (
                    stripos($the_query, "insert") !== false
                    || stripos($the_query, "update") !== false
                )
            ){
                //--- save debug info ---//
                $debug_info = debug_backtrace();
                $debug_info_cut = EkvCommonHelper::cutExessiveDebugInfo($debug_info);
                $debug_info_packed = EkvCommonHelper::packArray2Text($debug_info_cut);
                //pa(EkvCommonHelper::unpackArray2Text($debug_info_packed));

                $fileLogerObj = new EkvFileLogger("DATE_MOD_catch");
                $write_str = $the_query . "\n". $debug_info_packed;
                $fileLogerObj->writeString($write_str);
            }
//*/
			//var_dump($the_query);
			$this->query_id = mysqli_query($this->connection_id, $the_query);
            //var_dump($the_query);
            //var_dump($this->query_id);
			if (! $this->query_id ){
			    $this->fatal_error("mySQL query error: $the_query");
			}

            $this->query_count++;
//            if(EkvRoomHelper::isDebugVersionNow()){
//			    $this->obj['cached_queries'][] = $the_query;
//            }
			@$_SESSION['site_debug']['num_queries']++;

			return $this->query_id;
		}

		function escape($s)
		{
//			if(empty($this->connection_id)){
//				$this->connect();
//			}

			return '\''.mysqli_real_escape_string($this->connection_id, $s).'\'';
		}

		/*========================================================================*/
		// Fetch a row based on the last query
		/*========================================================================*/
		function fetch_row($query_id = "")
		{
			if ($query_id == "")
			{
				$query_id = $this->query_id;
			}
//var_dump($query_id);
			$this->record_row = @mysqli_fetch_array($query_id);

			return $this->record_row;
		}

		function fetch_row_numeric($query_id = "")
		{
			if ($query_id == "")
			{
				$query_id = $this->query_id;
			}
			$this->record_row = mysqli_fetch_row($query_id);
			return $this->record_row;
		}

		/*========================================================================*/
		// Fetch the number of rows affected by the last query
		/*========================================================================*/
		function get_affected_rows()
		{
			return mysqli_affected_rows($this->connection_id);
		}

		/*========================================================================*/
		// Fetch the number of rows in a result set
		/*========================================================================*/
		function get_num_rows()
		{
			return mysqli_num_rows($this->query_id);
		}

		/*========================================================================*/
		// Fetch the last insert id from an sql autoincrement
		/*========================================================================*/
		function get_insert_id()
		{
			return mysqli_insert_id($this->connection_id);
		}

		/*========================================================================*/
		// Return the amount of queries used
		/*========================================================================*/
		function get_query_cnt()
		{
			return $this->query_count;
		}

		/*========================================================================*/
		// Free the result set from mySQLs memory
		/*========================================================================*/
		function free_result($query_id="")
		{
			if ($query_id == "") $query_id = $this->query_id;
			@ mysqli_free_result($query_id);
		}

		/*========================================================================*/
		// Shut down the database
		/*========================================================================*/
		function close_db()
		{
			return mysqli_close($this->connection_id);
		}

		/*========================================================================*/
		// Return an array of tables
		/*========================================================================*/
//		function get_table_names()
//		{
//			$result     = mysql_list_tables($this->obj['sql_database']);
//			$num_tables = @mysqli_numrows($result);
//			for ($i = 0; $i < $num_tables; $i++) $tables[] = mysql_tablename($result, $i);
//			mysql_free_result($result);
//			return $tables;
//		}

		/*========================================================================*/
		// Return an array of fields
		/*========================================================================*/
		function get_result_fields($query_id="")
		{
			if ($query_id == "") $query_id = $this->query_id;
			while ($field = mysqli_fetch_field($query_id)) $Fields[] = $field;
			//mysql_free_result($query_id);
			return $Fields;
		}

		/*========================================================================*/
		// Basic error handler
		/*========================================================================*/
		function fatal_error($the_error)
		{
			global $INFO;
			// Are we simply returning the error?
			if ($this->return_die == 1)
			{
				$this->error    = mysqli_error($this->connection_id);
				$this->error_no = mysqli_errno($this->connection_id);
				$this->failed   = 1;
				return;
			}
			$the_error.= "\n\nmySQL error: ".mysqli_error($this->connection_id)."\n";
			$the_error.= "mySQL error code: ".$this->error_no."\n";
			$the_error.= "Date: ".date("l dS of F Y h:i:s A");

			$out = "<html><head><title>Database Error</title>
				<style>P,BODY{ font-family:verdana,sans-serif; font-size:11px; } div{font:normal 10px Courier;width:100%;height:300px;overflow:auto;border:1px solid #ccc;}</style></head><body>
				&nbsp;<br><br><blockquote><b>There appears to be an error with the database.</b><br>
				You can try to refresh the page by clicking <a href=\"javascript:window.location=window.location;\">here</a>
				<br><br>
				<div>".nl2br(htmlspecialchars($the_error))."</div><br>We apologise for any inconvenience</blockquote></body></html>";

			echo($out);
			die();
		}

		/*========================================================================*/
		// Create an array from a multidimensional array returning formatted
		// strings ready to use in an INSERT query, saves having to manually format
		// the (INSERT INTO table) ('field', 'field', 'field') VALUES ('val', 'val')
		/*========================================================================*/
		function compile_db_insert_string($data)
		{
			$field_names  = "";
			$field_values = "";
			foreach ($data as $k => $v)
			{
				$v = preg_replace( "/'/", "\\'", $v );
				//$v = preg_replace( "/#/", "\\#", $v );
				$field_names  .= "$k,";
				$field_values .= "'$v',";
			}
			$field_names  = preg_replace( "/,$/" , "" , $field_names  );
			$field_values = preg_replace( "/,$/" , "" , $field_values );
			return array(	'FIELD_NAMES'  => $field_names,
					'FIELD_VALUES' => $field_values,
					);
		}

		/*========================================================================*/
		// Create an array from a multidimensional array returning a formatted
		// string ready to use in an UPDATE query, saves having to manually format
		// the FIELD='val', FIELD='val', FIELD='val'
		/*========================================================================*/
		function compile_db_update_string($data)
		{
			$return_string = "";
			foreach ($data as $k => $v)
			{
				$v = preg_replace( "/'/", "\\'", $v );
				$return_string .= $k . "='".$v."',";
			}
			$return_string = preg_replace( "/,$/" , "" , $return_string );
			return $return_string;
		}

		/*========================================================================*/
		// Test to see if a field exists by forcing and trapping an error.
		// It ain't pretty, but it do the job don't it, eh?
		// Posh my ass.
		// Return 1 for exists, 0 for not exists and jello for the naked guy
		// Fun fact: The number of times I spelt 'field' as 'feild'in this part: 104
		/*========================================================================*/
		function field_exists($field, $table)
		{
			$this->return_die = 1;
			$this->error = "";
			$this->query("SELECT COUNT($field) as count FROM $table");
			$return = 1;
			if($this->failed) $return = 0;
			$this->error = "";
			$this->return_die = 0;
			$this->error_no   = 0;
			$this->failed     = 0;

			return $return;
		}
/*
		function _pagination($total, $per_page, $per_pagination, $page)
		{
			$pages = ceil($total / $per_page);
			if(!isset($page)) $page = 1;
			if($page > $pages) $page = $pages;
			if($page < 1) $page = 1;

			$start = ($page - 1) * $per_page;

			$res['row_from'] = $start;
			$res['row_num'] = $per_page;

			($page > 1) ? $res['first_page'] = 1 : $res['first_page'] = false;
			(($page > 0) && ($page != $pages)) ? $res['next_page'] = $page + 1 : $res['next_page'] = false;
			(($page > 0) && ($page != $pages)) ? $res['last_page'] = $pages : $res['last_page'] = false;
			($page > 1) ? $res['prev_page'] = $page - 1 : $res['prev_page'] = false;

			$fr = $page - floor($per_pagination / 2);
			$tl = $page + floor($per_pagination / 2);

			if ($pages <= $per_pagination)
			{
				$fr = 1;
				$tl = $pages;
			}
			elseif ($fr < 1)
			{
				$fr = 1;
				$tl = $per_pagination;
			}
			elseif ($tl > $pages)
			{
				$fr = $pages - $per_pagination + 1;
				$tl = $pages;
			}
			$visible_pages = array();
			for ($i = 0, $p = $fr; $p <= $tl; $i++, $p++)
			{
				if ($page == $p) $visible_pages[$i] = array("number" => $p, "current" => true);
				else $visible_pages[$i] = array("number" => $p, "current" => false);
			}

			$res['page'] = $page;
			$res['pages'] = $visible_pages;

			return($res);
		}
*/
	function _pagination($total, $per_page, $per_pagination, $page)
	{
		$pages = ceil($total/$per_page);
		if(!isset($page)){$page=1;}
		if($page > $pages){$page=$pages;}
		if($page<1){$page=1;}
		$start=($page-1)*$per_page;
		$res['total']=$total;
		$res['row_from']=$start;
		$res['row_num']=$per_page;
		$res['total_pages']=$pages;
		($page>1)?$res['first_page']=1:$res['first_page']=false;
		(($page>0)&&($page!=$pages))?$res['next_page']=$page+1:$res['next_page']=false;
		(($page > 0)&&($page!=$pages))?$res['last_page']=$pages:$res['last_page']=false;
		($page>1)?$res['prev_page']=$page-1:$res['prev_page']=false;
		$fr=$page-floor($per_pagination/2);
		$tl=$page+floor($per_pagination/2);
		if($pages<=$per_pagination){
			$fr=1;
			$tl=$pages;
		}elseif($fr<1){
			$fr=1;
			$tl=$per_pagination;
		}elseif($tl>$pages){
			$fr=$pages-$per_pagination+1;
			$tl=$pages;
		}
		$visible_pages=array();
		for($i=0,$p=$fr;$p<=$tl;$i++,$p++){
			if($page==$p){$visible_pages[$i] = array("number" => $p, "current" => true);}
			else{$visible_pages[$i]=array("number"=>$p,"current"=>false);}
		}
		if($per_page>$total){$res['current_visible']=$total;}
		else{$res['current_visible']=$per_page;}
		$res['page']=$page;
		$res['pages']=$visible_pages;
		$res['per_page']=$per_page;
		$res['per_pagination']=$per_pagination;
		$res['total_visible_pages']=count($res['pages']);
		return($res);
	}

	} // end class

?>

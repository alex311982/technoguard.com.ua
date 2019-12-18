<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	/**
	 * Type :	class
	 * Name :	VARParser
	 * Purpose:	Clean warning symbol from keys and vals in
	 *		given arrays and global arrays
	 * @funcs:	clean_array($arr)
	 *		parse_incoming()
	 *		clean_key($key)
	 *		clean_value($val)
	 */
	class VARParser
	{
		/**
		 * Clean warning symbols from keys and vals in array
		 * nesting of array: 2
		 * @param:	(array)$arr => array to parse
		 * @return:	(array) => parsed result
		 */
		function clean_array($arr)
		{
			$res=array();
			if(is_array($arr))
			{
				foreach($arr as $k=>$v)
				{
					if( is_array($arr[$k]) )
						foreach($arr[$k] as $k2=>$v2)
							$res[$k][ $this->clean_key($k2) ] = $this->clean_value($v2);
					else $res[$k] = $this->clean_value($v);
				}
			}
			return $res;
		}

		/**
		 * Clean warning symbols in incoming arrays
		 * @return:	(array) => result massive, all values overwrites
		 */
		// safe mode - each var is clean by reg exprs
		// safe_mode - false - is old version compatibility
		function parse_incoming($safe_mode)
		{
			global $_GET, $_POST;
//				$HTTP_CLIENT_IP,
//				$REQUEST_METHOD,
//				$REMOTE_ADDR,
//				$HTTP_PROXY_USER,
//				$HTTP_X_FORWARDED_FOR

			$res = array();
			if ($safe_mode == true){ 
				$res = array_merge($res, $this->clean_array($_GET));
				$res = array_merge($res, $this->clean_array($_POST));
			}else{
				$res = array_merge($res, $_GET);
				$res = array_merge($res, $_POST);
			}
			//$res = array_merge($res, $this->clean_array($HTTP_CLIENT_IP));
			//$res = array_merge($res, $this->clean_array($REQUEST_METHOD));
			//$res = array_merge($res, $this->clean_array($REMOTE_ADDR));
			//$res = array_merge($res, $this->clean_array($HTTP_PROXY_USER));
			//$res = array_merge($res, $this->clean_array($HTTP_X_FORWARDED_FOR));
			return $res;
		}

		/**
		 * Clean warning symbols in key(taken from array)
		 * @param:	(string)$key => key to parsing
		 * @return:	(string) => parsed key
		 */
		function clean_key($key)
		{
			if ($key == "")return "";
			$key = preg_replace( "/\.\./"		, ""	, $key );
			$key = preg_replace( "/\_\_(.+?)\_\_/"	, ""	, $key );
			$key = preg_replace( "/^([\w\.\-\_]+)$/", "$1"	, $key );
			return $key;
		}

		/**
		 * Clean warning symbols in value(taken from array)
		 * @param:	(string)$val => value to parsing
		 * @return:	(string) => parsed value
		 */
		function clean_value($val)
		{
			if ($val == "")return "";
			$val = preg_replace( "/&/"		, "&amp;"		, $val );
			$val = preg_replace( "/<!--/"		, "&#60;&#33;--"	, $val );
			$val = preg_replace( "/-->/"		, "--&#62;"		, $val );
			$val = preg_replace( "/<script/i"	, "&#60;script"		, $val );
			$val = preg_replace( "/>/"		, "&gt;"		, $val );
			$val = preg_replace( "/</"		, "&lt;"		, $val );
			$val = preg_replace( "/\"/"		, "&quot;"		, $val );
			$val = preg_replace( "/\|/"		, "&#124;"		, $val );
			$val = preg_replace( "/\n/"		, "<br>"		, $val ); // Convert literal newlines
			$val = preg_replace( "/\\\$/"		, "&#036;"		, $val );
			$val = preg_replace( "/\r/"		, ""			, $val ); // Remove literal carriage returns
			$val = preg_replace( "/!/"		, "&#33;"		, $val );
			$val = preg_replace( "/'/"		, "&#39;"		, $val ); // IMPORTANT: It helps to increase sql query safety.
			$val = @stripslashes($val);						  // Swop PHP added backslashes
			$val = preg_replace( "/\\\/"		, "&#092;"		, $val ); //??? Swop user inputted backslashes
			//echo "<h1>clean_value = $val</h1>";   
			return $val;
		}

	} // end of VARParser class

?>

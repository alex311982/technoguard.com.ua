<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	function load_config($file = '')
	{
		if(!file_exists($file)) return false;

		$content = parse_ini_file($file);

		foreach($content as $key => $value)
		{
			@ define(strtoupper($key), $value);
		}
	}

?>

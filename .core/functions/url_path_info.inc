<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */

	function url_path_info($url = 'unnamed')
	{
		if(empty($url)) return array();
		
		$url_parts = parse_url($url);
		$path = @explode('/',$url_parts['path']);

		if(isset($url_parts['hostname']) && $url_parts['hostname'] !== SITE_HOST)
			$url_parts['view'] = 'other';
		elseif(@$path[1] == SITE_BACKEND)
			$url_parts['view'] = SITE_BACKEND;
		else
			$url_parts['view'] = 'frontend';
		
		return $url_parts;
	}
?>

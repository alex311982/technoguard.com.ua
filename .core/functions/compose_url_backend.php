<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	function compose_url_backend($url)
	{
		$parts = parse_url($url);
		$count = strlen(PAGE_EXTENSION);
		
		if(!empty($parts['path']) && substr($parts['path'], -1, 1) != '/')
		{
			if(substr($parts['path'], -$count) != PAGE_EXTENSION)
			{
				$parts['path'] .= PAGE_EXTENSION;
				$url = $parts['path'];
				if(isset($parts['port']))
				{
					$url .= ':'.$parts['port'];
				}
				if(isset($parts['query']))
				{
					$url .= '?'.$parts['query'];
				}
			}
		}
			
		return $url;
	}

?>
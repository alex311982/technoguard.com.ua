<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	function compose_url_frontend($url)
	{
		$url = (empty($url) || '/' == $url)?'index':$url;
		$url_parts = parse_url($url);
		$uri = $url_parts['path'];
		$uri = str_replace(PAGE_EXTENSION, '', $uri);
		$uri = str_replace('"', '', $uri);
		$uri = ($uri{0} == '/')?substr($uri, 1):$uri;
		$uri = preg_split('/\//i', $uri);
		if(!class_exists('DB')) err404();		
		$DB = new DB();
		$sql = '
				SELECT
					`url`
				FROM
					`base_url`
				WHERE
					`module` = "'.$uri[0].'"
					AND `entity` = "'.$uri[1].'"
					AND `action` = "'.$uri[2].'"
				LIMIT 0,1
			';
		$DB->query($sql);
		$res = $DB->fetch_row();
		$url = '/'.$res['url'];
		$url .= PAGE_EXTENSION;
		if(isset($url_parts['port']))
		{
			$url .= ':'.$url_parts['port'];
		}
		if(isset($url_parts['query']))
		{
			$url .= '?'.$url_parts['query'];
		}
		return $url;
	}

?>

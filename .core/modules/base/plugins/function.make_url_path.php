<?php

function smarty_function_make_url_path($params, &$smarty)
{
	$url = stristr ($params["url"], "http://");
	if ($url)
		return $url;
	else
		return $params['domain']."/".$params["url"];
}
?>
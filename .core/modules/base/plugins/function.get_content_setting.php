<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */

	function smarty_function_get_content_setting($params, &$smarty)
	{
		//$en = load_entity_class('index', 'category');
		//if(empty($en)) return;
		$key = @mysql_real_escape_string($params["key"]);
		$assign = (isset($params["assign"])) ? $params["assign"] : "plugin_content_setting"; 
		
		$db = new DB();
		$res = $db->bGetInfoById("base_content", $key, "key");
		$smarty->assign($assign, @$res["text"]);
	}

?>

<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	function smarty_function_page($params, &$smarty)
	{
		if(empty($params['n']))
		{
			$params['n']=0;
		}
		else
		{
			$params['n']=intval($params['n']);
		}
		$QUERY_CONNECTOR = "&";
		if (isset ($params ['query_connector']) ){
			$QUERY_CONNECTOR = $params['query_connector'];
		}
//pa($params);
		if(!empty($params['type']) && $params['type']=='simply')
		{
			return $QUERY_CONNECTOR."page=".$params['n'];
		}elseif ( !empty($params['type']) && $params['type']=='4pu' )
		{
			return $QUERY_CONNECTOR."page".$params['n'];
		}
		if(!empty($params['type']) && $params['type']=='num')
		{
			return $params['n'];
		}
		else
		{
			return 'page'.$params['n'];
		}
	}

?>

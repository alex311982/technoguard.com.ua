<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */

	function smarty_function_retrieve_backend_menu($params, &$smarty)
	{
		$entity = load_entity_class('base', 'menu');
		if(empty($entity)) return;

		$r = $entity->DB->GetBackendMenu();
		$result = $entity->DB->Linear2Tree($r);
		$smarty->assign('menu', $result);
	}

?>

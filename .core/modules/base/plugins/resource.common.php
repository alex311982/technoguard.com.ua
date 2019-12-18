<?php

	/*
	* Smarty plugin
	* -------------------------------------------------------------
	* File: resource.user.php
	* Type: resource
	* Purpose: Fetches templates from the database
	* -------------------------------------------------------------
	*/

	// common:page.main.tpl will point to: .core/templates/SITE_VIEW/common/page.main.tpl
	// common:base/user/page.login.tpl will point to: .core/templates/SITE_VIEW/base/user/page.login.tpl

	function get_tpl_filename($tpl_name)
	{
	    $parts = preg_split('/\//', $tpl_name);
        $site_core_view = SITE_CORE_VIEW;
        $site_core_view_fake = EkvCommonHelper::getSiteCoreViewFake();
        /**
         * sometimes it's required to process frontend tpls from backend environment so we need to make fake core
         * to make proper path for including
         */
        if(is_null($site_core_view_fake) == false){
            $site_core_view = $site_core_view_fake;
        }

        $use_theme = EkvCommonHelper::getUseTplTheme();

        $_templates_path = "";
        //#------------------- no theme or backend view -> use common usual templates -------------------#//
        if($use_theme == false || $site_core_view == SITE_BACKEND){
            $_templates_path = PATH_TEMPLATES . DIRECTORY_SEPARATOR . $site_core_view;
        }
        //#------------------- use_theme = true and view = frontend -> use old theme -------------------#//
        else if($use_theme && $site_core_view == SITE_FRONTEND){
            $_templates_path = PATH_TEMPLATES . DIR_SEP . 'theme_old';
        }
//        $_templates_path = PATH_TEMPLATES . DIRECTORY_SEPARATOR . $site_core_view;
        
	    $_tpl_filename = '';
	    if(1 == count($parts))
	    {
	    	if(file_exists($_templates_path . DIRECTORY_SEPARATOR . 'common' . DIRECTORY_SEPARATOR . $tpl_name))
	    	{
	    		$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . 'common' . DIRECTORY_SEPARATOR . $tpl_name;
	    	}
	    }
	    elseif(3 == count($parts))
	    {
	    	$_module = $parts[0];
	    	$_entity = $parts[1];
	    	$_tpl = $parts[2];

			$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . $_module . DIRECTORY_SEPARATOR . $_entity . DIRECTORY_SEPARATOR . $_tpl;
	    	if(!file_exists($_tpl_filename))
	    	{
	    		$_tpl_filename = '';
	    	}
	    }
	    elseif(count($parts) > 3)
	    {
	    	$_module = $parts[0];
	    	$_entity = $parts[1];
	    	$_tpl = $parts[2].DIRECTORY_SEPARATOR.$parts[3];

			$_tpl_filename = $_templates_path . DIRECTORY_SEPARATOR . $_module . DIRECTORY_SEPARATOR . $_entity . DIRECTORY_SEPARATOR . $_tpl;
	    	if(!file_exists($_tpl_filename))
	    	{
	    		$_tpl_filename = '';
	    	}
	    }
    	return $_tpl_filename;
	}

	function smarty_resource_common_source($tpl_name, &$tpl_source, &$smarty)
	{
		$filename = get_tpl_filename($tpl_name);
		if(!empty($filename))
		{
			$tpl_source = file_get_contents($filename);
			return true;
		}

		return false;
	}

	function smarty_resource_common_timestamp($tpl_name, &$tpl_timestamp, &$smarty)
	{
		$filename = get_tpl_filename($tpl_name);
		$tpl_timestamp = filemtime($filename);
		$smarty->assign('tpl', $tpl_name);

		if (!$tpl_timestamp)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	function smarty_resource_common_secure($tpl_name, &$smarty)
	{
	    // assume all templates are secure
	    return true;
	}

	function smarty_resource_common_trusted($tpl_name, &$smarty)
	{
	    // not used for templates
	}


?>

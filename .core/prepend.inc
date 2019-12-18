<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$core_functions = array(
		'pa',
		'load_config',
		'load_functions',
		);

	foreach($core_functions as $func){
		include_once(PATH_CORE.DIR_SEP.'functions'.DIR_SEP.$func.'.inc');
	}

	load_config(PATH_CONF.DIR_SEP. 'main.inc');
	load_config(PATH_CONF.DIR_SEP. 'site.inc');
	load_config(PATH_CONF.DIR_SEP. 'lang.inc');
	load_config(PATH_CONF.DIR_SEP. 'errors.inc');

	///**************** backend conf
	load_config(PATH_CONF . DIRECTORY_SEPARATOR . 'backend.inc');	
	///**********sizes
	load_config(PATH_CONF . DIRECTORY_SEPARATOR . 'sizes.inc');
	$__SITE_BACKEND = SITE_BACKEND;
 


	$core_classes = array(
		'basic' => array (
			'error',
			'base',
			'entity',
			'dbbase',
			'db',
			'user',
			'varparser',
			'simage',
			),

		'smarty' => array(
			'smarty',
			),

		'captcha' => array(
			'kcaptcha',
			),
		);

    //#------------------- smarty3 -------------------#//
    $use_smarty3 = false;
    if(defined("EKV_SMARTY_USE_VERSION3")){
        $use_smarty3 = (bool)EKV_SMARTY_USE_VERSION3;
    }
    if($use_smarty3){
        unset($core_classes["smarty"]); //don't include smarty 2 classes
        require_once PATH_CORE. "/classes/smarty3/class.Smarty3.inc"; // include local smarty3 child class
    }

	foreach($core_classes as $section => $classes)
		foreach($classes as $class)
			require_once(PATH_CORE.DIR_SEP.'classes'.DIR_SEP.$section.DIR_SEP.'class.'.$class.'.inc');

	load_functions();

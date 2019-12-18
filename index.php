<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

//ini_set('display_errors', 0);

//header("Access-Control-Allow-Origin: *");

/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose:
 * ============================================================
 */


define('DIR_SEP', DIRECTORY_SEPARATOR);
define('PATH_ROOT', $_SERVER['DOCUMENT_ROOT']);
define('PATH_CONF', PATH_ROOT.DIRECTORY_SEPARATOR.'.conf');
define('PATH_CORE', PATH_ROOT.DIRECTORY_SEPARATOR.'.core');
define('PATH_TEMP', PATH_ROOT.DIRECTORY_SEPARATOR.'.temp');
define('PATH_IMAGE', PATH_ROOT.DIRECTORY_SEPARATOR.'images');
define('PATH_FILES', PATH_ROOT.DIRECTORY_SEPARATOR.'files');
define('PATH_TEMPLATES', PATH_CORE.DIRECTORY_SEPARATOR.'templates');
define('PATH_MODULES', PATH_CORE.DIRECTORY_SEPARATOR.'modules');
//	if (isset($_GET['aaa'])) die(PATH_CORE.DIRECTORY_SEPARATOR.'index.php');
include(PATH_CORE.DIRECTORY_SEPARATOR.'index.php');


?>

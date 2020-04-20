<?php

use Ekv\Helpers\System\FrontendUrlParser;
use Ekv\System\Misc\DeviceDetector;

//error_reporting(E_ALL);
//error_reporting(E_ALL & ~E_STRICT); 
//error_reporting(E_ALL^E_STRICT);
ini_set('display_errors', 0);
error_reporting(E_ALL ^ E_DEPRECATED);
//setlocale(LC_ALL, 'uk_UA.UTF-8');
if(isset($_REQUEST["friendly_path"])){
    $_SERVER["REAL_MY_PATH"] = $_SERVER["REQUEST_URI"];
    $_SERVER["REQUEST_URI"] = $_REQUEST["friendly_path"];
}

ini_set("url_rewriter.tags", "");
require_once "autoload.inc";

$start = microtime();
require_once(PATH_CORE.DIRECTORY_SEPARATOR.'prepend.inc');
require_once(PATH_CORE.DIRECTORY_SEPARATOR.'prepear.inc');
require_once(PATH_CORE.DIRECTORY_SEPARATOR.'globals.inc');
//ini_set('session.cookie_domain', COOKIE_DOMAIN );
ini_set('session.cookie_domain', EkvCommonHelper::getCookieDomain());
ini_set('display_errors', 0);
ini_set('date.timezone', 'Europe/Kiev');

require_once(PATH_CORE.DIRECTORY_SEPARATOR.'/functions/utf8/UTF8.php');
require_once(PATH_CORE.DIRECTORY_SEPARATOR.'/functions/utf8/ReflectionTypehint.php');

require_once(PATH_CORE.DIRECTORY_SEPARATOR.'/functions/strip_tags_smart.php');

$url_parts = parse_url($_SERVER['REQUEST_URI']);
if(
    !isset($url_parts["path"])
    || empty($url_parts["path"])
    || isset($url_parts["host"])
){
    echo "<h2>Error</h2>\n"; exit;
}
$url = ($url_parts['path']{0} == '/')?substr($url_parts['path'], 1):$url_parts['path'];
$path_parts = preg_split('/\//i', $url);

// Get the session Id passed from SWFUpload. We have to do this to work-around the Flash Player Cookie Bug
if(@$path_parts[3] == "photos_multi_upload_new.html"
    && isset($_REQUEST["do_uploading"])
    && isset($_POST["PHPSESSID"])
)
{
    session_id($_POST["PHPSESSID"]);
}

//--- set session max lifetime for backend ---//
if(isset($path_parts[0]) && $path_parts[0] == SITE_BACKEND){
    ini_set("session.gc_maxlifetime", 10800);// 3 hours
}

session_start();
//--- make session single for all subdomains ---//
//setcookie("PHPSESSID", session_id(), 0, "/", COOKIE_DOMAIN);
/* Debug */
define('DEBUG_TIME_START', microtime_float($start));
add_debug_point('end_prepear');
unset($_SESSION['site_debug']);
$_SESSION['site_debug']['start'] = "0.000";
$_SESSION['site_debug']['num_queries'] = 0;

if(in_array(SITE_BACKEND, $path_parts))
{
    $_REQUEST['view'] = SITE_BACKEND;
    if(count($path_parts) > 2)
    {
        $_REQUEST['module'] = $path_parts[1];
        $_REQUEST['entity'] = $path_parts[2];
        $_REQUEST['action'] = substr($path_parts[3], 0, -strlen(PAGE_EXTENSION));
    }
}
if(empty($_REQUEST['view']))
{
    $_REQUEST['view'] = SITE_FRONTEND;
}

//	if(isset($_SESSION[$session_user_key]['id'])){
//		define ('__UID', $_SESSION[$session_user_key]['id']);
//	}else{
//		define ('__UID', -1);
//	}
define ('__UID', -1);

if($_REQUEST['view'] == SITE_BACKEND)
{
    if($_SERVER['HTTP_HOST'] != SITE_HOST)
    {
        session_destroy();
        $url = SITE_DOMAIN.'/'.SITE_BACKEND;
        header('Location: '.$url);
    }

    if (empty($_REQUEST['module']) || empty($_REQUEST['entity']) || empty($_REQUEST['action']))
    {
        $backend_url = compose_url_backend('/'.SITE_BACKEND.'/'.DEFAULT_BACKEND_MODULE.'/'.DEFAULT_BACKEND_ENTITY.'/'.DEFAULT_BACKEND_ACTION);
        //echo "<h1>backend_url = $backend_url</h1>";
        force_location($backend_url);
    }

    $_REQUEST['module'] = (!isset($_REQUEST['module']))?BACKEND_LOGIN_MODULE:$_REQUEST['module'];
    $_REQUEST['entity'] = (!isset($_REQUEST['entity']))?BACKEND_LOGIN_ENTITY:$_REQUEST['entity'];
    $_REQUEST['action'] = (!isset($_REQUEST['action']))?BACKEND_LOGIN_ACTION:$_REQUEST['action'];
}
else
{
    $frUrlParserObj = new FrontendUrlParser();

    //$path = frontend_get_path($_SERVER['REQUEST_URI']);
    $path = $frUrlParserObj->mainParse($_SERVER['REQUEST_URI']);

//    if (!empty($path['translation_id'])){
//        $_REQUEST['page_translation_id'] = $path['translation_id'];
//    }
//
//    //--- parse device type ---//
//    $deviceDetector = new DeviceDetector();
//    $deviceDetector->mainProcess();
//    $deviceDetector->maintainDevicePages($path["module"], $path["entity"], $path['action']);

    $_REQUEST['module'] = $path['module'];
    $_REQUEST['entity'] = $path['entity'];
    $_REQUEST['action'] = $path['action'];
}
define("SITE_CORE_VIEW", $_REQUEST['view']);
define("SITE_CORE_MODULE", $_REQUEST['module']);
define("SITE_CORE_ENTITY", $_REQUEST['entity']);
define("SITE_CORE_ACTION", $_REQUEST['action']);
add_debug_point('start_load_entity_class');

//$handler = load_entity_class(SITE_CORE_MODULE, SITE_CORE_ENTITY);
$handler = load_entity_class_optional(SITE_CORE_MODULE, SITE_CORE_ENTITY);
$handler->view = SITE_CORE_VIEW;



//if(SITE_CORE_VIEW == "frontend"){}

//#------------------- do logging for unhandled exceptions -------------------#//

try{
    $handler->execute(SITE_CORE_ACTION);
}catch(Exception $e){
    var_dump($e);exit;
    if(EkvCommonHelper::isLiveVersionNow()){

        echo "<h2>Critical error has occured!  </h2>\n";
        $fileLogger = new EkvFileLogger("__exceptions");
        $ex_header_str = PHP_EOL . "-->> " . $e->getMessage() . PHP_EOL . $e->getFile() . " | Line: " . $e->getLine();
        $fileLogger->writeStringPlain($ex_header_str);

        $pack_exception = EkvCommonHelper::packArray2Text(array_slice($e->getTrace(), 0, 5));
        $fileLogger->writeStringPlain($pack_exception);
    }else{
        throw $e;
    }
}
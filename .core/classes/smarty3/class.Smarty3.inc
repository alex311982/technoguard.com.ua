<?php

//const SMARTY_SPL_AUTOLOAD = true;
//--- include smarty core ---//
define('PATH_SMARTY', PATH_CORE . '/classes/smarty3/native/');
require_once(PATH_SMARTY . 'SmartyBC.class.php');


class Smarty3ClassException extends Exception{}

//class SmartyClass extends Smarty
class SmartyClass extends SmartyBC //save backward compatibility
{
//		var $_file_perms = 0777;
//		var $_dir_perms = 0777;

    function __construct($handler = NULL)
    {
        parent::__construct();

        $this->left_delimiter = '<!--{';
        $this->right_delimiter = '}-->';
        $this->compile_check = true;
        $this->force_compile = SMARTY_FORCE_COMPILE;
        $this->debugging = false;
        $this->caching = false;

        $this->error_reporting = 0;
        //$this->error_reporting = E_ALL & ~E_NOTICE;

        //--- set template dir ---//
        $template_dir = array();
        $_template_path_base = PATH_TEMPLATES . DIRECTORY_SEPARATOR . $handler;
        $template_dir[] = $_template_path_base . DIRECTORY_SEPARATOR . 'common' . DIRECTORY_SEPARATOR;
        $this->setTemplateDir($template_dir);

        //--- set plugin dir ---//
        $plugins_dir = array();


        $plugins_dir [] = PATH_SMARTY . 'plugins' . DIRECTORY_SEPARATOR; // smarty3 core plugins
        $old_plugin_dir = PATH_CORE . "/modules/base/plugins/"; // old plugins for backward compatibility
        $smarty3_plugin_dir = PATH_CORE . "/modules/base/plugins3/"; //smarty3 plugins

        $plugins_dir[] = $smarty3_plugin_dir;
        $plugins_dir[] = $old_plugin_dir;
        $this->setPluginsDir($plugins_dir);

        //#------------------- path compiled -------------------#//
        $path_compiled = PATH_TEMP . DIRECTORY_SEPARATOR . "compiled3" . DIRECTORY_SEPARATOR . $handler . DIRECTORY_SEPARATOR;
        $this->setCompileDir($path_compiled);

        //--- path cache IS NOT USED NOW ---//
        $path_cache = PATH_TEMP . DIRECTORY_SEPARATOR . SMARTY_PATH_CACHE . DIRECTORY_SEPARATOR;
        $this->setCacheDir($path_cache);

        //--- global tpl vars ---//
        $LIVE_VERSION = (LIVE_VERSION == "on" || LIVE_VERSION == "yes" || LIVE_VERSION == 1) ? true : false;
        if(
            isset($_COOKIE["ekv_live_version"])
            && $_COOKIE["ekv_live_version"] == 0
        ){
            $LIVE_VERSION = false;
        }
        $this->assign("__LIVE_VERSION", $LIVE_VERSION);

        //$this->assign("__UID", __UID); //just for convenience
    }

    function display($resource_name = NULL, $cache_id = null, $compile_id = null, $parent_NEW = null)
    {
        //parent::fetch($resource_name, $cache_id, $compile_id, true);
        //parent::fetch($resource_name, $cache_id, $compile_id, null, true);
        $this->assign("ekv_resource_name", $resource_name);
        parent::display($resource_name, $cache_id, $compile_id, $parent_NEW);
    }

    static function getFrontendTemplatesAbsolutePath($relative_path = null)
    {
        $base_path = PATH_ROOT . "/.core/templates/frontend/";
        $abs_path = $base_path;
        if (!is_null($relative_path)) {
            $relative_path = EkvCommonHelper::strRemoveFirstSymbol($relative_path, '/');
            $abs_path .= $relative_path;
            if (file_exists($abs_path) == false) {
                throw new Smarty3ClassException("tpl path doesn't exist '{$abs_path}' ");
            }
        }
        return $abs_path;
    }

}

?>

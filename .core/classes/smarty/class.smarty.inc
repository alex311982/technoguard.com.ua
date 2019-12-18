<?php
	define('PATH_SMARTY', PATH_CORE.'/classes/smarty/native/');
	require_once(PATH_SMARTY.'Smarty.class.php');

    class SmartyClassException extends Exception{}

	class SmartyClass extends Smarty
	{
		var $_file_perms = 0777;
		var $_dir_perms = 0777;

		function SmartyClass($handler = NULL)
		{
			$this->Smarty();

			$this->template_dir = array();
			$_template_path =  PATH_TEMPLATES.DIRECTORY_SEPARATOR.$handler;
			$this->template_dir[] = $_template_path.DIRECTORY_SEPARATOR.'common'.DIRECTORY_SEPARATOR;
			//$this->template_dir[] = PATH_TEMPLATES.DIRECTORY_SEPARATOR."frontend".DIRECTORY_SEPARATOR;
			$this->plugins_dir = array();
			$this->plugins_dir []= PATH_SMARTY.'plugins'.DIRECTORY_SEPARATOR;
			foreach($GLOBALS['modules'] as $_module)
			{
				$this->plugins_dir []= PATH_CORE.DIRECTORY_SEPARATOR.'modules'.DIRECTORY_SEPARATOR.$_module.DIRECTORY_SEPARATOR.'plugins'.DIRECTORY_SEPARATOR;
			}

            //#------------------- path compiled -------------------#//
			$path_compiled = PATH_TEMP . DIRECTORY_SEPARATOR . SMARTY_PATH_COMPILE . DIRECTORY_SEPARATOR . $handler . DIRECTORY_SEPARATOR;

			$path_cache = PATH_TEMP.DIRECTORY_SEPARATOR.SMARTY_PATH_CACHE.DIRECTORY_SEPARATOR;
			//mkdirpath($path_compiled);
			//mkdirpath($path_cache);
			$this->compile_dir = $path_compiled;
			$this->cache_dir = $path_cache;

			$this->left_delimiter = '<!--{';
			$this->right_delimiter = '}-->';

			$this->compile_check = true;
			$this->force_compile = SMARTY_FORCE_COMPILE;
			$this->debugging = false;

			$this->caching = false;

			$this->trusted_dir[] = PATH_CORE.DIRECTORY_SEPARATOR.'functions'.DIRECTORY_SEPARATOR;

            $LIVE_VERSION = (LIVE_VERSION == "on" || LIVE_VERSION == "yes" || LIVE_VERSION == 1) ? true : false;
            if(
                isset($_COOKIE["ekv_live_version"])
                && $_COOKIE["ekv_live_version"] == 0
            ){
                $LIVE_VERSION = false;
            }
            $this->assign("__LIVE_VERSION", $LIVE_VERSION);
            $this->assign("__UID", __UID );//just for convenience
		}

		function display($resource_name, $cache_id = null, $compile_id = null)
		{
            $this->assign("ekv_resource_name", $resource_name);
			parent::fetch($resource_name, $cache_id, $compile_id, true);
		}

        static function getFrontendTemplatesAbsolutePath($relative_path = null)
        {
            $base_path = PATH_ROOT . "/.core/templates/frontend/";
            $abs_path = $base_path;
            if(!is_null($relative_path)){
                $relative_path = EkvCommonHelper::strRemoveFirstSymbol($relative_path, '/');
                $abs_path .= $relative_path;
                if(file_exists($abs_path) == false){
                    throw new SmartyClassException("tpl path doesn't exist '{$abs_path}' ");
                }
            }
            return $abs_path;
        }

        /**
         * used to allow old plugin use smarty2 & smarty3 together
         * @param null $varname
         * @return array|string
         */
        function getTemplateVars($varname=null)
        {
            return $this->get_template_vars($varname);
        }

	}

?>

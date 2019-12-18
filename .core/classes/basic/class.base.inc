<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: xxx xxx
 *   Purpose:
 * ============================================================
 */

	class Base extends ErrorCustom
	{
		//var static  $vars_already_parsed = false;
		var $handler = '';
		var $incoming_vars = '';
		var $parsed_vars = ''; /* _input_[name] */
		//cleaned vars
		var $safe_incoming_vars = '';
		var $safe_parsed_vars = ''; /* _input_[name] */

        /**
         * @var SmartyClass
         */
        var $TML = '';
		var $url = '';
		var $last_url = '';
		var $default_url = '';

		var $module = '';
		var $entity = '';
		var $action = '';
		var $view 	= '';

		var $login_url = '';
		var $logout_url = '';

		var $user = '';

		function __construct()
		{
			$this->view = SITE_CORE_VIEW;
			$this->url = parse_url($_SERVER['REQUEST_URI']);
			if($this->view == SITE_BACKEND)
			{
				$this->login_url = compose_url_backend('/'.SITE_BACKEND.'/base/user/login');
				$this->logout_url = compose_url_backend('/'.SITE_BACKEND.'/base/user/logout');
				$this->default_url = compose_url_backend('/'.SITE_BACKEND.'/'.DEFAULT_BACKEND_MODULE.'/'.DEFAULT_BACKEND_ENTITY.'/'.DEFAULT_BACKEND_ACTION);
			}
			else
			{
				$this->logout_url = compose_url_frontend('/base/user/logout');
				$this->login_url = compose_url_frontend(BACKEND_LOGIN_MODULE.'/'.BACKEND_LOGIN_ENTITY.'/'.BACKEND_LOGIN_ACTION);
				$this->default_url = '/';
			}
			if(isset($_SESSION['last_uri'])) $last = url_path_info($_SESSION['last_uri']);
			if(isset($last['view']) && $last['view'] !== SITE_CORE_VIEW) unset($_SESSION['last_uri']);
			if(!isset($_SESSION['last_uri'])) $_SESSION['last_uri'] = '';

			if (empty($_SESSION['last_uri']) && isset($_SERVER['HTTP_REFERER']))
			{
				$_SESSION['last_uri'] = $_SERVER['HTTP_REFERER'];
			}
			if (!empty($_SESSION['current_uri']) &&	$this->url['path'] !== $this->login_url)
			{
				$_SESSION['last_uri'] = $_SESSION['current_uri'];
			}
			$_SESSION['current_uri'] = $_SERVER["REQUEST_URI"];
			$this->last_url = parse_url($_SESSION['last_uri']);

			$modules_path = PATH_CORE.'/modules/';
			$d = dir($modules_path);
			while(false !== ($file = $d->read()))
			{
				if(is_dir($modules_path.$file) && $file != '.' && $file != '..' && $file != '.svn')
				{
					$GLOBALS['modules'] []= $file;
				}
			}

			if(!isset($_SESSION['user']))
			{
				$this->user = new User();
				$_SESSION['user'] = $this->user;
			}
			else $this->user = &$_SESSION['user'];
		}

		function init()
		{
			static  $vars_already_parsed = false;
			$this->TML = new SmartyClass(SITE_CORE_VIEW);
			if ($vars_already_parsed == false){
				$vars_already_parsed = true;
				$this->parse_incoming_vars();
			}
		}
		function vars_strip_tags($vars)
		{
			foreach ($vars as $k=>$val){
				$vars[$k] = strip_tags($val);
			}
			return $vars;
		}
		function get_parsed_vars($strip_tags = false)
		{
			$vars=array();
			foreach ($this->parsed_vars as $pars => $value){
				if (!is_array($value)){
					$vars[$pars] = stripslashes($value);
				}else{
					$vars[$pars] = $value;
				}
			}
            if($strip_tags){
                $vars = $this->vars_strip_tags($vars);
            }
			return $vars;
		}
        function get_parsed_vars_strip()
        {
            return $this->get_parsed_vars(true) ;
        }
        function get_parsed_vars_with_strip_tags()
        {
            return $this->get_parsed_vars(true) ;
        }
		function get_parsed_vars_safe()
		{
			$vars=array();
			foreach ($this->safe_parsed_vars as $pars => $value){
				if (!is_array($value)){
					$vars[$pars] = stripslashes($value);
				}else{
					$vars[$pars] = $value;
				}
			}
			return $vars;
		}
		function get_incoming_vars_safe()
		{
			return $this->safe_incoming_vars;
		}
		function get_incoming_vars()
		{
			return $this->incoming_vars;
		}

		function set_parsed_vars($set = array())
		{
			$_set = array();
			foreach($set as $name => $value)
			{
				$matches = array();
				if(preg_match('/^_input_([\w_\s]+)$/i', $name, $matches) > 0)
				{
					$_set[$matches[1]] = $value;
				}
			}
			$this->parsed_vars = $_set;
			//pa($this->parsed_vars);
			$this->incoming_vars = $set;
		}

		function set_safe_parsed_vars($set = array())
		{
			$_set = array();
			foreach($set as $name => $value)
			{
				$matches = array();
				if(preg_match('/^_input_([\w_\s]+)$/i', $name, $matches) > 0)
				{
					$_set[$matches[1]] = $value;
				}
			}
			$this->safe_parsed_vars = $_set;
			//pa($this->parsed_vars);
			$this->safe_incoming_vars = $set;
		}

		// safe mode - each var is clean by reg exprs
		// safe_mode - false - is old version compatibility
		function parse_incoming_vars($safe_mode = false)
		{
			$vh = new VARParser();
			$vars = $vh->parse_incoming($safe_mode);
			$this->set_parsed_vars($vars);
			// parse safe vars
			$safe_vars = $vh->parse_incoming($safe_mode = true);
			$this->set_safe_parsed_vars($safe_vars);
		}

		function DefaultAssign()
		{
			$errors = array_merge($this->DB->errors, $this->errors, $this->user->errors);
			$this->TML->assign('debug_sql', $this->DB->obj['cached_queries']);
			$this->TML->assign('errors', $errors);
			$this->TML->assign('messages', $this->messages);

            $this->TML->assign("is_backend_edit_new_action", $this->isCurrentActionEditNew() );
		}
        function isCurrentActionEditNew()
        {
            if($this->action == "new" || strpos($this->action, "_new") !== false ){
                return true;
            }
            return false;
        }

		function Display($resource_name, $cache_id = null, $compile_id = null)
		{
			$this->DefaultAssign();
			$this->TML->assign('top_tpl', $resource_name);
			add_debug_point('start_display');
			$this->TML->display($resource_name, $cache_id, $compile_id);
			exit();
		}

        function DisplayAuto($relative_tpl_name)
        {
            $full_tpl_name = "common:{$this->module}/{$this->entity}/{$relative_tpl_name}";
            $this->Display($full_tpl_name);
        }

        function assignFilterAction($filter_action)
        {
            $this->TML->assign("filter_action", $filter_action);
        }

	}
?>
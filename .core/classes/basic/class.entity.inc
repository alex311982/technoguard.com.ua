<?php

class Entity extends Base
{
    var $RedirectVars = array();
    var $id = 0;
    var $ids = array();
    //var $DB = '';
    /** @var $DB DB */
    public $DB;
    var $login_req = false;

    var $DisableFilter = false;
    var $RedirectQuery = '';

    var $path = '';

    /**
     * @var array
     * Example: array("conv" => "conv_")
     */
    protected $_pageTypes = array();

    /**
     * Current page type
     * @var string
     */
    public $pageType;

    public $browseAction = "";
    public $PK;

    /**
     * if not null - don't call SetBreadcrumps() but use this array instead
     * @var array
     */
    public $redefinedBreadcrumps;

    protected $_breadcrumps = array();

    function __construct()
    {
        parent::__construct();
        if(!empty($_GET['id']))
        {
            $this->id = (int)$_GET['id'];
            $this->ids = array((int)$_GET['id']);
        }
        elseif(!empty($_GET['ids']))
        {
            $ids = explode(',',$_GET['ids']);
            foreach($ids as $id)
                $this->ids []= (int)$id;
        }elseif(!empty($this->PK)){
            $input_key = "_input_{$this->PK}";
            if(
                isset($_REQUEST[$input_key])
                && $_REQUEST[$input_key] > 0
            ){
                $this->id = (int)$_REQUEST[$input_key];
            }
        }


        $this->RedirectQuery  = '';
        $query = array();
        if(!empty($this->RedirectVars) && empty($this->RedirectQuery))
        {
            foreach($this->RedirectVars as $var)
            {
                if(!empty($_REQUEST[$var]))
                    $this->$var = $_REQUEST[$var];
                if(isset($_GET[$var]))
                {
                    $query []= $var . '=' . $_GET[$var];
                }
            }
            $this->RedirectQuery = implode('&', $query);
        }
    }

    /**
     * @param $pk_name
     * @param string $browse_action
     * @param array $page_types - array('dr')
     */
    protected function _initNew($pk_name, $browse_action = '', $page_types = array())
    {
        $this->browseAction = $browse_action;
        $this->PK = $pk_name;
        $this->initPageTypes($page_types);
        $this->defineAndSetPageType();
    }

    function Init($module = '', $entity = '')
    {
        parent::init();
        $this->module = $module;
        $this->entity = $entity;
        $this->path = PATH_CORE.DIRECTORY_SEPARATOR.'modules'.DIRECTORY_SEPARATOR.$this->module.DIRECTORY_SEPARATOR.SITE_CORE_VIEW.DIRECTORY_SEPARATOR.$this->entity;
    }

    //--- is not inited copletely here ---//
    function ManualAfterDbInit()
    {
        // can be overriden in descendant
    }
    //--- inited copletely here ---//
    function ManualBeforeExecuteIncludeAction()
    {
        // can be overriden in descendant
    }

    function ProcessRedirectVars()
    {
        if(empty($this->RedirectVars)) return;
        foreach($this->RedirectVars as $var)
        {
            if(isset($_GET[$var]))
            {
                $_REQUEST[$var] = $_GET[$var];
            }
            else
            {
                $_REQUEST[$var] = $this->$var;
//					pa($this->$var);
//					$_GET[$var] = $_SESSION['redirect_vars'][$var];
            }
            if(isset($_SESSION['redirect_vars']))
                unset($_SESSION['redirect_vars'][$var]);
            $_SESSION['redirect_vars'][$var] = $_REQUEST[$var];
        }

    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // EXECUTE
    /////////////////////////////////////////////////////////////////////////////////////////

    function execute($action)
    {
        //pa($entity_path, $action, $this->user);
        add_debug_point('start_execute_action');
        if(
            $this->view == SITE_BACKEND &&
            (!$this->user->IsLogged() || $this->user->view != SITE_BACKEND)
            && $this->login_url != $this->url['path']
            )
        {
            force_location($this->login_url);
        }
        if(
            $this->view == SITE_FRONTEND &&
            (!$this->user->IsLogged() || $this->user->view != 'frontend' || $this->user->role_id == RL_GUEST)
            && $this->login_url != $this->url['path']
            && $this->login_req
            )
        {
            force_location($this->login_url);
        }

        $this->action = $action;

        $this->ProcessRedirectVars();

        $default_action_file = PATH_CORE.DIRECTORY_SEPARATOR.
                                'default'.DIRECTORY_SEPARATOR.
                                SITE_CORE_VIEW.DIRECTORY_SEPARATOR.
                                'actions'.DIRECTORY_SEPARATOR.'action.'.$this->action.'.inc';

        $entity_path = PATH_CORE.DIRECTORY_SEPARATOR.'modules'.DIRECTORY_SEPARATOR.$this->module.DIRECTORY_SEPARATOR.SITE_CORE_VIEW.DIRECTORY_SEPARATOR.$this->entity;
        $action_file = $entity_path.DIRECTORY_SEPARATOR.'actions'.DIRECTORY_SEPARATOR.'action.'.$this->action.'.inc';

        //#------------------- PERMISSIONS FOR BACKEND ADMINS -------------------#//
        if ($this->view == "admin"
            && !in_array($this->action, array("logout", "login", "forbidden")) //this actions are available for all
            && $this->user->role_id != EkvBackendAdmins::ROLE_SUPER // not superadmin
        )
        {
            $user_role_id = $this->user->role_id;

            /**
             * -------------------- MODERATOR access restrictions
             * has access to scheduleMod including schedule templates and grid
             */
            if(
                $user_role_id == EkvBackendAdmins::ROLE_MODER
                && !$this->checkModerModuleEntityPermissions()
            ){
                force_location("/admin/moder/moder/forbidden.html"); // default partners url
            }
        }

        if ( $this->view == "admin" ){
            $role_text_id = EkvBackendAdmins::getRoleTextId($this->user->role_id);
            $this->TML->assign("ADMIN_ROLE", $role_text_id);

//            $userNewsPermHelper = new UserNewsPermission();
//            $news_permissions = $userNewsPermHelper->getSessionNewsPermContainer(false);
//
//            $userSpPermHelper = new UserSpPermission();
//            $sp_perm = $userSpPermHelper->getSessionNewsPermContainer(false);
//
//            $this->TML->assign("newsPermissions", $news_permissions);
//            $this->TML->assign("spPermissions", $sp_perm);
        }
        //#------------------- /PERMISSIONS FOR BACKEND ADMINS/ -------------------#//

        if(file_exists($action_file))
        {
            if($this->view == "admin"){
                //--- this object can be used by getInstance in the every part of backend ---//
                $adminLogObj = new EkvAdminLog($this->user->id);
                EkvAdminLog::setInstance($adminLogObj);
            }
            //echo "<h1>action file = $action_file </h1>";
            $this->ManualBeforeExecuteIncludeAction();
            include $action_file;
        }
        elseif(file_exists($default_action_file))
        {
            include $default_action_file;
        }
        else
        {
            trigger_error('Could not load action <b>'.$this->action.'</b>: <i>'.$action_file.'</i><br />', E_USER_ERROR);
        }

    }

    private function checkModerModuleEntityPermissions()
    {
        /*
         * array('moduleAllowed' => array('entity1Allowed', 'entity2Allowed'))
         */
        $allowed = array(
            'scheduleMod' => array(),
            'bank' => array(),
            'news' => array(),
            'tcontent' => array(),
            'base' => array('menuFrontend')
        );

        $allowedModules = array_keys($allowed);

        $hasAccess = true;
        if(!in_array($this->module, $allowedModules)){
            $hasAccess = false;
        }else{
            //--- module is allowed, check is entity allowed ---//
            $allowedEntities = $allowed[$this->module];
            if(
                !empty($allowedEntities)
                && !in_array($this->entity, $allowedEntities)
            ){
                $hasAccess = false;
            }
        }

        return $hasAccess;
    }

    function SetBreadcrump()
    {
        return array();
    }

    function ChangeLocation($url)
    {
        if($this->view == SITE_BACKEND)
        {
            $redirect = compose_url_backend($url.'?'.$this->RedirectQuery);
        }
        else
        {
            $redirect = compose_url_frontend($url.'?'.$this->RedirectQuery);
        }

        force_location($redirect);
    }

    function force_referer()
    {
        if(!isset($_SERVER['HTTP_REFERER'])) force_location("/");
        if($_SERVER['HTTP_REFERER']==('http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'])) force_location("/");
        else force_location($_SERVER['HTTP_REFERER']);
    }

    function DefaultAssign()
    {
        parent::DefaultAssign();
        $breadcrumps = (is_null($this->redefinedBreadcrumps)) ? $this->SetBreadcrump() : $this->redefinedBreadcrumps;
        $redirect_vars = array();
        $redirect_str = array();
        if(!empty($this->RedirectVars))
        {
            foreach($this->RedirectVars as $var)
            {
                if(isset($_REQUEST[$var])) $redirect_vars []= $var.'='.$_REQUEST[$var];
            }
        }
        //pa ($redirect_vars);
        $redirect_str = implode('&', $redirect_vars);
        $this->TML->assign('breadcrumps', $breadcrumps);

        $this->TML->assign('_view', $this->view);
        $this->TML->assign('_module', $this->module);
        $this->TML->assign('_entity', $this->entity);
        $this->TML->assign('_action', $this->action);
        $this->TML->assign('_disable_filter', $this->DisableFilter);

        $this->TML->assign('redirect_vars', $redirect_str);
        $this->TML->assign('redirect_query', $this->RedirectQuery);
        $this->TML->assign('site_date_format', DEFAULT_DATE_FORMAT);
    }

    function redefineBreadCrumps($breadcrumps)
    {
        $this->redefinedBreadcrumps = $breadcrumps;
    }

    //--- corrects filter for fields in $fields array, and saves filter values to $session name ---//
    function CorrectFilter($session_name, $fields = array())
    {
        if(!empty($fields)){
            foreach ($fields as $k=>$val){
                $post_key = "_filter_{$val}";
                $ses_key = "filter_{$session_name}";
                if(isset($_POST[$post_key])){
                    $_SESSION[$ses_key][$val] = $_POST[$post_key];
                }else{
                    $_POST[$post_key] = @$_SESSION[$ses_key][$val];
                }
            }
        }
    }

    //-------------------------
    // should be invoked from browse action to make proper filter values saving
    // $extra_distinguisher - is used to distinguish common filter and filter for pages like forCityId or forUserId(orders page)
    //-------------------------
    function saveFilterState($extra_distinguisher = array())
    {
        $extra_distinguisher_STR = "";
        if(!empty($extra_distinguisher)){
            foreach($extra_distinguisher as $ek=>$eval){
                $extra_distinguisher_STR .= "{$ek}_{$eval}";
            }
            $extra_distinguisher_STR = "_{$extra_distinguisher_STR}";
        }

        $filter_local_session_name = "_backend_filter_{$this->module}_{$this->entity}_{$this->action}{$extra_distinguisher_STR}";
        $fields_to_save_filter = array();

        foreach($this->DB->fields as $fk=>$fval){
            if(
                array_key_exists("calculable", $fval) == false
                || (isset($fval["manual_processing"]) && $fval["manual_processing"] == true)
            ){
                $fields_to_save_filter[] = $fk;
            }
        }
        //pa($fields_to_save_filter, $filter_local_session_name);
        if(!empty($fields_to_save_filter)){
            $this->CorrectFilter($filter_local_session_name, $fields_to_save_filter);
        }
    }

    static function prependOptionsWithDefaultValue(&$options, $default_value = null)
    {
        if(is_null($default_value)){
            $default_value = DEFAULT_SELECT_NAME;
        }
        $options = array('' => $default_value) + $options;
    }

    function setSelectFieldOptions($options, $field_name, $browse_action = "browse")
    {
        if(is_null($browse_action)){
            Entity::prependOptionsWithDefaultValue($options);
        }
        else if($this->action == $browse_action){
            Entity::prependOptionsWithDefaultValue($options);
        }
        $this->DB->fields[$field_name]["options"] = $options;
    }

    function setSelectFieldOptionsUnconditional($options, $field_name)
    {
        $this->setSelectFieldOptions($options, $field_name, null);
    }

    function denyRobotAccess()
    {
        $this->TML->assign("__PAGE_NO_ROBOTS", true );
    }
    function addBreadcrumpItem(&$breadcrumps, $title, $url)
    {
        $breadcrumps []=
            array(
                'title' => EkvCommonHelper::smartyEscapeEmulator($title),
                'href' => $url
            );
    }

    function addCurrentModuleToAutoload()
    {
        EkvAutoload::addModuleClassPath($this->module, $this->entity, $this->view);
    }

    function bDisableAllFilterButtons($dis_filter = 1, $dis_create = 1, $dis_edit  = 1, $dis_delete = 1)
    {
        if($dis_create == 1){
            $this->TML->assign("toolbar_create_disable", $dis_create);
        }
        $this->TML->assign("toolbar_edit_disable", $dis_edit);
        $this->TML->assign("toolbar_delete_disable", $dis_delete);
        //$this->TML->assign("_disable_filter", 1);
        $this->DisableFilter = $dis_filter;
    }

    function bDisableToolbar()
    {
        $this->TML->assign("__DISABLE_TOOLBAR", true);
    }

    /**
     * common:/undertoolbar/orders/extra_print.tpl
     * @param $tpl_relative
     */
    function bUnderToolbarBlock($tpl_relative)
    {
        $base_path = "common:/undertoolbar/";
        $full_path = $base_path . $tpl_relative;
        $this->TML->assign("tpl_undertoolbar", $full_path);
    }

    /**
     * must be called on edit and create pages
     * @param null $url
     */
    function bSetReferer($url = null)
    {
        if(is_null($url)){
            $url = isset($_SERVER["HTTP_REFERER"]) ? $_SERVER["HTTP_REFERER"] : "";
        }
        $this->TML->assign("ekv_http_referer", $url);
    }

    /**
     * used at save page after successul operation
     * @return bool
     */
    function bRedirectToReferer()
    {
        if(
            isset($_REQUEST["ekv_http_referer"])
            && !empty($_REQUEST["ekv_http_referer"])
        ){
            force_location($_REQUEST["ekv_http_referer"]);
        }

        return false;
    }

    /**
     * defines page type from action for backend urls
     * @return string
     */
    protected function definePageType()
    {
        $page_type = "basic";
        if(!empty($this->_pageTypes)){
            foreach($this->_pageTypes as $page_type_iter => $page_type_action_prefix){
                if(preg_match("#^{$page_type_action_prefix}#i", $this->action)){
                    $page_type = $page_type_iter;
                }
            }
        }

        $this->TML->assign("toolbar_action_prefix", "{$page_type}_");
        return $page_type;
    }

    protected function initMainParamsByOtherEntity(Entity $entityObj)
    {
        $this->DB = $entityObj->DB;
        $this->TML = $entityObj->TML;
        $this->user = $entityObj->user;
    }

    function loadModuleConfig($__FILE__, $conf_name = "conf.inc")
    {
        load_config ( dirname($__FILE__) . DIRECTORY_SEPARATOR . $conf_name );
    }

    /**
     * assigns tpl flag which then allows to include neccessary js/css files in backend header
     */
    function jsChosenUse()
    {
        $this->TML->assign("JS_CHOSEN_USE", true);
    }

    function setFrontError($err_msg)
    {
        $this->TML->assign('err_front', $err_msg);
    }

    protected function defineAndSetPageType()
    {
        $this->pageType = $this->definePageType();
    }

    protected function initPageTypes($plain_list)
    {
        foreach($plain_list as $lval){
            $this->_pageTypes[$lval] = "{$lval}_";
        }
    }

    function accessDeniedMsgStop($msg = "Access denied")
    {
        echo "<h2>{$msg}</h2>\n";
        exit;
    }

    function addBcItem($title, $url)
    {
        $this->_breadcrumps[] = array(
            'title' => EkvCommonHelper::smartyEscapeEmulator($title),
            'href' => $url
        );
    }

    function removeRedirectVars()
    {
        $this->RedirectQuery = "";
    }

    function inputName($plain_name)
    {
        return "_input_{$plain_name}";
    }
}
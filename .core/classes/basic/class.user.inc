<?php
use Ekv\Helpers\Backend\BackendUserGlobal;
use \Ekv\Moderator\Moderator;
use Ekv\News\Permissions\UserNewsPermission;
use Ekv\StatPage\Permissions\UserSpPermission;

class User extends DB
{
    public $view	= '';
    public $id		= 0;
    public $login	= '';
    public $role_id	= 0;
    public $enabled	= '';
    public $name	= 'guest';
    public $globalUid;

    function __construct()
    {
        parent::__construct();
        $this->_init_guest_user(false);
    }

    function _init_guest_user($save = true)
    {
        $this->view		= 'frontend';
        $this->id		= 0;
        $this->login		= 'guest';
        //$this->role_id		= RL_GUEST;
        $this->enabled		= 'yes';
        $this->name		= 'guest';
        if($save) $_SESSION['user'] = $this;
    }

    /**
     * encript string
     * @param	(string)$str -> string to encript
     * @return	(string) -> encripted string
     */
    function _encript($str)
    {
        return md5($str);
    }

    /**
     * decript string
     * @param	(string)$str -> string to decript
     * @return	(string) -> decripted string
     */
    function _decript($str)
    {
        return null;
    }


    function Login($login, $password)
    {
        $pass = $this->_encript($password);
        $user_info = $this->GetUserInfo($login);

        //--- super admin ---//
        if(
            $user_info['password'] === $pass &&
            $user_info['login'] == $login&&
            $user_info['enabled'] == 'yes' &&
            $user_info['view'] == SITE_CORE_VIEW
            && EkvBackendAdmins::isRoleIdCorrect($user_info["type_id"])
        ){
            $this->view	= $user_info['view'];
            $this->id	= $user_info['id'];
            $this->login	= $user_info['login'];
            $this->view	= $user_info['view'];
            $this->enabled	= $user_info['enabled'];
            $this->role_id = $user_info["type_id"];
            if($this->role_id == EkvBackendAdmins::ROLE_SUPER){
                $_SESSION['user'] = $this;// back compatibility
                $this->_setGlobalUid($this->id, EkvBackendAdmins::ROLE_SUPER);

                $this->_setSessionTcPermContainers($this->id, EkvBackendAdmins::ROLE_SUPER);
            }else{
                EkvBackendAdmins::setSessionAdminInfo($user_info);
            }
        }else{
            if($this->_tryModerAuth($login, $password)){
            }
        }
    }

    private function _setSessionTcPermContainers($uid, $role)
    {
        $newsHelperObj = new UserNewsPermission();
        $newsHelperObj->setTcPermToSession($uid, $role);

        $spHelperObj = new UserSpPermission();
        $spHelperObj->setTcPermToSession($uid, $role);
    }

    private function _setGlobalUid($uid, $role)
    {
        $helperObj = new BackendUserGlobal();
        $global_uid = $helperObj->getGlobalUserID($uid, $role);

        EkvException::ensure($global_uid > 0, "Retrieving globalUID failed!");
        $this->globalUid = $global_uid;
        return $global_uid;
    }

    private function _tryModerAuth($login, $password)
    {
        $auth_ok = false;
        $moderObj = new Moderator();
        $moder_info = $moderObj->getModerInfoByLogin($login);
        $crypt_pwd = $moderObj->cryptPassword($password);

        if(
            !empty($moder_info)
            && $moder_info["moder_pwd"] == $crypt_pwd
            && $moder_info["moder_enabled"]
        ){
            $moder_id = $moder_info["id_moder"];
            $this->_initUserPropertiesOnSuccessAuth($login, $moder_id, EkvBackendAdmins::ROLE_MODER);

            EkvBackendAdmins::setSessionAdminInfo($moder_info);

            $auth_ok = true;

            $this->_setGlobalUid($moder_id, EkvBackendAdmins::ROLE_MODER);

            $this->_setSessionTcPermContainers($moder_id, EkvBackendAdmins::ROLE_MODER);
        }

        return $auth_ok;
    }

    private function _initUserPropertiesOnSuccessAuth($login, $user_id, $role_id)
    {
        $this->role_id = $role_id;
        $this->view	= "admin";
        $this->id	= $user_id;
        $this->login	= $login;
        $this->enabled	= true;
    }

    function GetUserInfo($login)
    {
        $this->table	= 'base_user';
        $this->connect();
        $login = mysqli_real_escape_string($this->connection_id, $login);

        $sql = "
            SELECT *, 'admin' as `view`
            FROM `base_user`
            WHERE
                `login` = '{$login}'
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $this->query($sql);
        $result = $this->fetch_row();
        return $result;
    }

    function IsLogged()
    {
        if($this->id > 0) return true;
        return false;
    }

    function Logout()
    {
        $this->_init_guest_user();
        $_SESSION['user'] = $this;

        if(isset($_SESSION["backend_admin_info"])){
            unset($_SESSION["backend_admin_info"]);
        }
    }

    function GetRestrictions()
    {
    }

}
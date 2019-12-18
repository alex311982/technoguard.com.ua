<?php
/**
 * EkvBackendAdmins - logic for operating with backend admins
 */

class EkvBackendAdmins
{
    const TABLE_BASE_USER = "base_user";
    private $DB;
    // this const are used in old engine class user, required for back compatibility mode
    const ROLE_SUPER = 1,
        ROLE_MODER = 4
    ;

    const SES_NAME_ADMIN_INFO = "backend_admin_info";

    function __construct()
    {
        $this->DB = EkvCommonHelper::getDB();
    }

    static function getSignedInAdminId()
    {
        $admin_id = 0;
        if(isset($_SESSION["user"]->id)){
            $admin_id = intval($_SESSION["user"]->id);
        }
        return $admin_id;
    }

    static function getSignedInAdminRoleId()
    {
        $role_id = 0;
        if(isset($_SESSION["user"]->role_id)){
            $role_id = intval($_SESSION["user"]->role_id);
        }
        return $role_id;
    }

    static function getSignedInGlobalUid()
    {
        $global_uid = 0;
        if(isset($_SESSION["user"]->globalUid)){
            $global_uid = intval($_SESSION["user"]->globalUid);
        }
        return $global_uid;
    }

    static function isSignedInAdminSuper()
    {
        $res = false;
        if(self::getSignedInAdminRoleId() == self::ROLE_SUPER){
            $res = true;
        }
        return $res;
    }

    static function isSignedInAdminModer()
    {
        $res = false;
        if(self::getSignedInAdminRoleId() == self::ROLE_MODER){
            $res = true;
        }
        return $res;
    }

    static function getSignedInModerID()
    {
        $moder_id = 0;
        if(self::assertSignedInUserRoleId(self::ROLE_MODER)){
            $moder_id = self::getSignedInAdminId();
        }
        return $moder_id;
    }

    static function assertSignedInUserRoleId($role_id)
    {
        $res = false;
        if(self::getSignedInAdminRoleId() == $role_id){
            $res = true;
        }
        return $res;
    }

    static function setSessionAdminInfo($data)
    {
        $_SESSION[self::SES_NAME_ADMIN_INFO] = $data;
    }

    static function getSessionAdminInfo()
    {
        return @$_SESSION[self::SES_NAME_ADMIN_INFO]; 
    }

    static function getRoleOptions()
    {
        $options =
            array(
                self::ROLE_SUPER => "ROLE_SUPER",
                self::ROLE_MODER => "ROLE_MODER",
            );
        return $options;
    }

    static function isRoleIdCorrect($role_id)
    {
        $roles = self::getRoleOptions();
        if(array_key_exists($role_id, $roles)){
            return true;
        }else{
            return false;
        }
    }

    static function getRoleTextId($role_int_id)
    {
        $text_id = "";
        $roles = self::getRoleOptions();
        if(array_key_exists($role_int_id, $roles)){
            $text_id = $roles[$role_int_id];
        }
        return $text_id;
    }

    function getAllAdminOptions()
    {
        $sql = "
            SELECT id, login
            FROM `".self::TABLE_BASE_USER."`
            WHERE
                1
            ORDER BY
                login ASC
        ";
        $options = array();
        $this->DB->query($sql);
        while($row = $this->DB->fetch_row()){
            $options[$row["id"]] = $row["login"];
        }
        return $options;
    }

    function getUserInfoBackend($uid)
    {
        $sql = "
            SELECT *
            FROM `".self::TABLE_BASE_USER."`
            WHERE
                id = '$uid'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

}



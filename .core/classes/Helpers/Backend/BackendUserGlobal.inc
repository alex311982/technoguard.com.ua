<?php
namespace Ekv\Helpers\Backend;
use EkvGlobalHelper, EkvBackendAdmins;

class BackendUserGlobal
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function insertConnection($outer_uid, $user_type)
    {
        $outer_uid = intval($outer_uid);
        $user_type = intval($user_type);

        $sql = "
            INSERT INTO  `".EkvGlobalHelper::TABLE_USER_BACKEND."`
            (`outer_user_id`, `user_type`)
            VALUES
            (
                '{$outer_uid}',
                '{$user_type}'
            )
        ";

        $this->DB->query($sql);
        return $this->DB->get_insert_id();
    }

    function getGlobalUserID($outer_uid, $user_type)
    {
        $outer_uid = intval($outer_uid);
        $user_type = intval($user_type);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_USER_BACKEND."`
            WHERE
                `outer_user_id` = '{$outer_uid}'
                AND `user_type` = '{$user_type}'
        ";
        $res = $this->DB->RunSql($sql);

        return !empty($res) ? intval($res["uid_global"]) : 0;
    }

    function getGlobalUserOptions()
    {
        $admin_options = $this->_getAdminOptions();
        $moder_options = $this->_getModerOptions();

        $options = $admin_options + $moder_options;
        return $options;
    }

    private function _getAdminOptions()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_BASE_USER."` bu
                INNER JOIN `".EkvGlobalHelper::TABLE_USER_BACKEND."` ug
                    ON bu.id = ug.outer_user_id
                    AND ug.user_type = ".EkvBackendAdmins::ROLE_SUPER."
            WHERE
                1
            ORDER BY
                `login` ASC
        ";
        $this->DB->query($sql);

        $options = array();
        while($row = $this->DB->fetch_row()){
            $global_uid = $row["uid_global"];
            $options[$global_uid] = "{$row["login"]} [{$global_uid}] [админ]";
        }

        return $options;
    }

    private function _getModerOptions()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_MODER."` m
                INNER JOIN `".EkvGlobalHelper::TABLE_USER_BACKEND."` ug
                    ON m.id_moder = ug.outer_user_id
                    AND ug.user_type = ".EkvBackendAdmins::ROLE_MODER."
            WHERE
                1
            ORDER BY
                `moder_login` ASC
        ";
        $this->DB->query($sql);

        $options = array();
        while($row = $this->DB->fetch_row()){
            $global_uid = $row["uid_global"];
            $options[$global_uid] = "{$row["moder_login"]} [{$global_uid}]";
        }

        return $options;
    }
}
 
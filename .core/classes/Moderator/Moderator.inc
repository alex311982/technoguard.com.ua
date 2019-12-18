<?php
namespace Ekv\Moderator;
use EkvGlobalHelper, DB;

class Moderator
{
    private $DB;
    private $_pwdSalt = 'sched-moder-pwd-salt';

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    function getModerInfoById($moder_id)
    {
        $moder_id = intval($moder_id);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_MODER."`
            WHERE
                id_moder = {$moder_id}
        ";

        $res = $this->DB->RunSql($sql);
        return $res;
    }

    /**
     * get moder info and channels allowed
     * @param $login
     * @return array
     */
    function getModerInfoByLogin($login)
    {
        $login_safe = $this->DB->escape($login);
        $sql = "
            SELECT *,
                (
                    SELECT GROUP_CONCAT(mca.channel_id)
                    FROM `".EkvGlobalHelper::TABLE_MODER_CHANNEL_ACCESS."` mca
                    WHERE
                        mca.moder_id = m.id_moder
                ) as `channels_allowed`
            FROM `".EkvGlobalHelper::TABLE_MODER."` m
            WHERE
                moder_login = {$login_safe}
        ";

        $res = $this->DB->RunSql($sql);

        //--- make channels allowed as array ---//
        if(
            !empty($res)
            && !empty($res["channels_allowed"])
        ){
            $res["channels_allowed"] = explode(',', $res["channels_allowed"]);
        }
        return $res;
    }

    function isLoginValid($login)
    {
        $is_valid = false;
        $pattern = '#^[0-9a-zA-Z_\-]+$#';

        if(preg_match($pattern, $login)){
            $is_valid = true;
        }

        return $is_valid;
    }

    function cryptPassword($pwd)
    {
        $pwd .= $this->_pwdSalt;
        return md5($pwd);
    }

    function updateModerPassword($moder_id, $pwd_clear)
    {
        $pwd_crypted = $this->cryptPassword($pwd_clear);
        $moder_id = intval($moder_id);

        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_MODER."`
            SET
                `moder_pwd` = '{$pwd_crypted}'
            WHERE
                id_moder = '{$moder_id}'
        ";
        $this->DB->query($sql);
    }

    function setEnabled($moder_ids)
    {
        $moder_ids = is_array($moder_ids) ? implode(',', $moder_ids) : $moder_ids;

        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_MODER."`
            SET
                `moder_enabled` = 0
            WHERE
                id_moder IN ({$moder_ids})
        ";

        $this->DB->query($sql);
    }

    function getModer2ChannelsConn($moder_id)
    {
        $moder_id = intval($moder_id);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_MODER_CHANNEL_ACCESS."`
            WHERE
                `moder_id` = '{$moder_id}'
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }
}

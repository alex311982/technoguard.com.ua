<?php
class EkvAdminLogException extends Exception{}

class EkvAdminLog
{
    const TABLE_ADMIN_MODULES = "ekv_admin_modules",
        TABLE_ADMIN_LOG_ACTIONS = "ekv_admin_log_actions",
        TABLE_ADMIN_LOG = "ekv_admin_log",
        TABLE_ADMIN_LOG_EXTRA = "ekv_admin_log_extra"
    ;
    static private $_instance;
    private $_admin_id, $DB;
    function __construct($admin_id = null)
    {
        $this->_admin_id = $admin_id;
        $this->DB = EkvCommonHelper::getDB();
    }

    static function setInstance(EkvAdminLog $inst)
    {
        self::$_instance = $inst;
    }
    /**
     * @static
     * @return EkvAdminLog
     */
    static function getInstance()
    {
        return self::$_instance;        
    }

    //-------------------------
    // $module_item_id - id of news, gallery, video etc
    // $item_name - name of news item, gallery item
    // $action_key - key of self::$actionsList like 'delete', 'login'
    //-------------------------
    function addLogRecord($module_text_id, $action_key, $module_item_id, $item_name = '')
    {
        $admin_id = $this->_admin_id;
        $module_id = $this->getModuleIdByTextId($module_text_id);
        $ip = EkvCommonHelper::getCurrentIp();
        $action_id = $this->getActionIdByTextId($action_key);
        
        $item_name_safe = mysql_real_escape_string(strip_tags($item_name));

        //$save_data = array("request" => $_REQUEST, "server" => $_SERVER);
        $server = array(
            "HTTP_REFERER" => @$_SERVER["HTTP_REFERER"],
            "REQUEST_URI" => $_SERVER["REQUEST_URI"]
        );
        $save_data = array("request" => $_REQUEST, "server" => $server);
        $data_packed = EkvCommonHelper::packArray2Text($save_data);
        $data_packed_safe = mysql_real_escape_string($data_packed);

        $sql = "
            INSERT INTO `".self::TABLE_ADMIN_LOG."`
                (
                    `admin_id`,
                    `ip`,
                    `module_id`,
                    `module_item_id`,
                    `module_item_name`,
                    `action_id`,
                    `date`
                )
            VALUES
                (
                    '$admin_id',
                    '$ip',
                    '$module_id',
                    '$module_item_id',
                    '$item_name_safe',
                    '$action_id',
                    NOW()
                )
        ";

        $this->DB->query($sql);
        $log_id = $this->DB->get_insert_id();
        if($log_id > 0){
            $sql = "
                INSERT INTO `".self::TABLE_ADMIN_LOG_EXTRA."`
                (log_id, data_packed)
                VALUES
                    ({$log_id}, '{$data_packed_safe}')
            ";
            $this->DB->query($sql);
        }
    }

    function getModuleInfo($module_text_id)
    {
        $text_id_safe = mysql_real_escape_string($module_text_id);
        $sql = "
            SELECT *
            FROM `".self::TABLE_ADMIN_MODULES."`
            WHERE
                `module_text_id` = '{$text_id_safe}'
        ";
        $res = $this->DB->RunSql($sql);
        if(empty($res)){
           throw new EkvAdminLogException(); 
        }
        return $res;
    }
    function getModuleIdByTextId($module_text_id)
    {
        $res = $this->getModuleInfo($module_text_id);
        return $res["id_module"];
    }
    function getActionInfo($action_key)
    {
        $action_key_safe = mysql_real_escape_string($action_key);
        $res = $this->DB->bGetInfoById(self::TABLE_ADMIN_LOG_ACTIONS, $action_key_safe, "action_text_id");
        if(empty($res)){
           throw new EkvAdminLogException();
        }
        return $res;
    }
    function getActionIdByTextId($action_key)
    {
        $res = $this->getActionInfo($action_key);
        return $res["id_action"];
    }

    function getModuleOptions()
    {
        $sql = "
            SELECT *
            FROM `".self::TABLE_ADMIN_MODULES."`
            WHERE
                1
            ORDER BY
                `module_name`
        ";
        $this->DB->query($sql);
        $res = array();
        while($row = $this->DB->fetch_row()){
            $res[$row["id_module"]] = $row["module_name"];
        }
        return $res;
    }
    function getActionOptions()
    {
        $sql = "
            SELECT *
            FROM `".self::TABLE_ADMIN_LOG_ACTIONS."`
            WHERE
                1
            ORDER BY
                `action_name`
        ";
        $this->DB->query($sql);
        $res = array();
        while($row = $this->DB->fetch_row()){
            $res[$row["id_action"]] = $row["action_name"];
        }
        return $res;
    }
}

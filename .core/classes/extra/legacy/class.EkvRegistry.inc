<?php

class EkvRegistry
{
    const TABLE_REGISTRY = "ekv_registry";
    private $DB;
    static private $_instance;
    private function __construct()
    {
        $this->DB = EkvCommonHelper::getDB();    
    }

    /**
     * @static
     * @return EkvRegistry
     */
    static function getInstance()
    {
        if(is_null(self::$_instance)){
            self::$_instance = new self();
        }
        return self::$_instance;
    }

    function setValue($key, $value)
    {
        $key_safe = mysql_real_escape_string($key);
        $value_safe = mysql_real_escape_string($value);
        $sql = "
            INSERT INTO `".self::TABLE_REGISTRY."`
            SET
                `key` = '{$key_safe}',
                `value` = '{$value_safe}'
            ON DUPLICATE KEY
            UPDATE
                `value` = '{$value_safe}'
        ";
        $this->DB->query($sql);
    }

    static function setValueStatic($key, $value)
    {
        $registryObj = self::getInstance();
        $registryObj->setValue($key, $value);
    }

    static function getValueStatic($key)
    {
        $registryObj = self::getInstance();
        return $registryObj->getValue($key);
    }

    function getValue($key)
    {
        $key_safe = mysql_real_escape_string($key);
        $sql = "
            SELECT *
            FROM `".self::TABLE_REGISTRY."`
            WHERE
                `key` = '{$key_safe}'
        ";
        $res = $this->DB->RunSql($sql);
        return !empty($res) ? $res["value"] : null;
    }

}

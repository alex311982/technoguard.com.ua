<?php
/**
 * User: sunsey
 * Date: 10.11.2010
 */

class EkvContentModule
{
    static private $DB = null;
    static public $contentModuleTable = "base_content";
    static function initDb($db = null)
    {
        if(is_null($db) == false){
            self::$DB = $db;
        }else{
            self::$DB = EkvCommonHelper::getDB();
        }
    }

    static function getMultipleValues(array $keys)
    {
        self::initDb();
        foreach($keys as &$kval){
            $kval = "'{$kval}'";
        }
        $keys_str = implode(",", $keys);
        $sql = "
            SELECT *
            FROM `".self::$contentModuleTable."`
            WHERE
                `key` IN ($keys_str)
        ";
        //$res = self::$DB->RunSql($sql, true);
        self::$DB->query($sql);
        $res = array();
        while($row = self::$DB->fetch_row()){
            $res[$row["key"]] = $row["text"];
        }
        return $res;
    }

    static function getValue($key)
    {
        self::initDb();
        $key_safe = mysql_real_escape_string($key);
        $sql = "
            SELECT *
            FROM `".self::$contentModuleTable."`
            WHERE
                `key` = '{$key_safe}'
        ";
        $res = self::$DB->RunSql($sql, false);
        return $res["text"];        
    }
}

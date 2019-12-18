<?php
/**
 * User: sunsey
 * Date: 15.11.2010
 */
 
class JsonHelper
{
    static function  escapeSingleQuotes($src_string)
    {
        $res_str = str_replace("'", '&#039;', $src_string);
        //src_string.replace(/'/g, '&#039;');
        return $res_str;
    }
    static function  escapeDoubleQuotes($src_string)
    {
        $res_str = str_replace('"', '&quot;', $src_string);
        return $res_str;
    }

    static function  escapeBothQuotes($src_string)
    {
        $res_str = self::escapeSingleQuotes($src_string);
        $res_str = self::escapeDoubleQuotes($res_str);
        return $res_str;
    }

}

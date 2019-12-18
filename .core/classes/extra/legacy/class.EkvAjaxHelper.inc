<?php
/**
 * User: sunsey
 * Date: 22.02.2011
 */

class EkvAjaxHelperException extends Exception{}

 
class EkvAjaxHelper
{
    static public $DEBUG = true;

    static function ensureWithException($expression, $fail_msg = "Default EkvAjaxHelperException msg.")
    {
        if(!$expression){
            throw new EkvAjaxHelperException($fail_msg);
        }
    }
    static function ensureWithExit($expression, $fail_msg = "ensureWithExit")
    {
        if(!$expression){
            EkvDebug::debugAjaxExceptions();
            //self::ensureWithException($expression);
            echo "<h3>{$fail_msg}</h3>";
            exit;
        }
    }

    static function getCmd($cmd_key = "cmd")
    {
        $cmd = "";
        if(isset($_REQUEST[$cmd_key])){
            $cmd = trim($_REQUEST[$cmd_key]);
        }
        return $cmd;
    }

    static function getRequestInt($key)
    {
        $res = 0;
        if(isset($_REQUEST[$key])){
            $res = intval($_REQUEST[$key]);
        }
        return $res;
    }
}

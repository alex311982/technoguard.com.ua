<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: xxx xxx
 *   Purpose: 
 * ============================================================
 */

	function add_debug_point($str = 'unnamed')
	{
//		pa(microtime(), DEBUG_TIME_START);
		$_SESSION['site_debug'][$str] = substr(microtime_float(microtime()) - microtime_float(microtime()), 0, 5);
	}
    function ekv_show_debug_data($show_always = false)
    {
        if(
            $show_always
            || EkvCommonHelper::isLiveVersionNow() == false
            || EkvCommonHelper::isDebugVersionNow()
        ){
            pa($_SESSION["site_debug"]);
        }
    }
	
	function microtime_float($time = 0)
	{
		list($usec, $sec) = explode(" ", $time);
		return ((float)$usec + (float)$sec);
	}
?>

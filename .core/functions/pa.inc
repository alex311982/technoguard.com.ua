<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */	
	function pa()
	{
		$backtrace = debug_backtrace();
		$args = func_get_args();
		$matches = array();
		preg_match('|.*[\/\\\](.+)$|', $backtrace[0]['file'], $matches);
		$res = array($matches[1].': '.$backtrace[0]['line'], $args);
		echo "<pre>";
		print_r($res);
		echo "</pre>";
	}	


	/**
	 * PA Write to file
	 */
	function paw()
	{
		$backtrace = debug_backtrace();
		$args = func_get_args();
		$matches = array();
		preg_match('|.*[\/\\\](.+)$|', $backtrace[0]['file'], $matches);
		$res = array(date('Y-m-d H:i:s'), $matches[1].': '.$backtrace[0]['line'], $args);

		$s="\n".str_repeat("-",80)."\n";
		ob_start();
		print_r($res);
		$s.=ob_get_contents();
		ob_end_clean();
		$s.="\n".str_repeat("-",80)."\n";

		$f=@ fopen(PATH_TEMP.DIR_SEP.'paw.log', "a");
		if(!empty($f))
		{
			fwrite($f, $s);
			fclose($f);
		}
		else
		{
			echo '<b>FATAL ERROR:</b>&nbsp;<br/><pre>'.$s.'</pre>';
		}
	}

?>

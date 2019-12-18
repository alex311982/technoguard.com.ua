<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose:
 * ============================================================
 */

	function load_functions($dir = '')
	{
		if($dir == '') $dir = PATH_CORE.'/functions/';
		if(is_dir($dir))
		{
			$dh = dir($dir);

			while(false !== ($file = $dh->read()))
			{
				if(file_exists($dir.$file) && $file != '.' && $file != '..' && $file != '.svn' && $file != 'utf8')
				{
					include_once($dir.$file);
				}
			}
			$dh->close();
		}
	}

?>

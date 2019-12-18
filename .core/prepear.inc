<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
	$DB = new DB();
    EkvCommonHelper::setDB($DB); // to use the same db obj in smarty plugins plugins
	// Define settings from database
	$DB->query('SELECT `title`, `key`, `value` FROM `base_settings`');
	while($row = $DB->fetch_row()){
		define(strtoupper($row['key']), $row['value']);
	}


?>
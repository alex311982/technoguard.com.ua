<?php

	/*
	* Smarty plugin
	* -------------------------------------------------------------
	* File: resource.user.php
	* Type: resource
	* Purpose: Fetches templates from the database
	* -------------------------------------------------------------
	*/
	
	function smarty_resource_database_source($tpl_name, &$tpl_source, &$smarty) 
	{ 
	    // do database call here to fetch your template, 
	    // populating $tpl_source 
//	    $sql = new SQL; 
//	    $sql->query("select tpl_source 
//	                   from my_table 
//	                  where tpl_name='$tpl_name'"); 
//	    if ($sql->num_rows) { 
//	        $tpl_source = $sql->record['tpl_source']; 
//	        return true; 
//	    } else { 
//	        return false; 
//	    } 
	} 
	
	function smarty_resource_database_timestamp($tpl_name, &$tpl_timestamp, &$smarty) 
	{ 
	    // do database call here to populate $tpl_timestamp. 
//	    $sql = new SQL; 
//	    $sql->query("select tpl_timestamp 
//	                   from my_table 
//	                  where tpl_name='$tpl_name'"); 
//	    if ($sql->num_rows) { 
//	        $tpl_timestamp = $sql->record['tpl_timestamp']; 
//	        return true; 
//	    } else { 
//	        return false; 
//	    } 
	} 
	
	function smarty_resource_database_secure($tpl_name, &$smarty) 
	{ 
	    // assume all templates are secure 
	    return true; 
	} 
	
	function smarty_resource_database_trusted($tpl_name, &$smarty) 
	{ 
	    // not used for templates 
	} 


?>

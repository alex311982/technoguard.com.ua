<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */
 
 	//$this->DB->Delete($this->ids);
 	foreach ($this->ids as $k=>$val){
		$this->DB->CorrectDeliveryDelete($val);
	}

 	$this->ChangeLocation('browse');

?>

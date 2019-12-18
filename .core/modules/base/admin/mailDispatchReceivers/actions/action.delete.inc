<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *    Author: Egor Konoval
 *   Purpose: 
 * ============================================================
 */

    foreach($this->ids as $val){
        $item_name = EkvAdminLog::getItemNameForLog($this->DB->table,
            "email",
            $val,
            $item_id_key = "id");
        
        EkvAdminLog::addLogRecord($val, $item_name, "delete");
    }
 
 	$this->DB->Delete($this->ids);
 	$this->ChangeLocation('browse');

?>

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
    $mdTaskObj = new EkvMailDispatch_Tasks();
    foreach ($this->ids as $k=>$task_id){             
        $mdTaskObj->deleteTaskCompletely($task_id);
    }

    $this->ChangeLocation('queue_browse');

?>

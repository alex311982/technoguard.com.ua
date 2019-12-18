<?php

$mdTaskObj = new EkvMailDispatch_Tasks();
//#------------------- fetch new portions of processed email -------------------#//
if(isset($_REQUEST["doAjaxAction"])){
    $is_task_finished = 0;
    $task_id = @intval($_REQUEST["task_id"]);
    $begin_from_id = @intval($_REQUEST["beginFrom"]);
    EkvAjaxHelper::ensureWithExit($task_id > 0);

    $extra_items = $mdTaskObj->getFinishedExtraItemes($task_id, $begin_from_id);
    $beginFetchFromId = defineNextBeginFetchPoint($extra_items);

    $task_info = $mdTaskObj->getTaskInfoById($task_id);
    if(!empty($task_info) && $task_info["is_finished"] == 1){
        $is_task_finished = 1;
    }

    $final_ar =
        array(
            "extra_items" => $extra_items,
            "beginFetchFromId" => $beginFetchFromId,
            "is_task_finished" => $is_task_finished
        );
    $final_ar_JSON = json_encode($final_ar);
    echo $final_ar_JSON;
    
}
//#------------------- first time preview  -------------------#//
else{
    $task_id = $this->id;
    $extra_items = $mdTaskObj->getFinishedExtraItemes($task_id);
    if(!empty($extra_items)){
        $date_formated = date(DATE_FORMAT_PLAIN, time());
        foreach($extra_items as &$ei_val ){
            $ei_val["date_formated"] = $date_formated;
        }
    }
    $this->TML->assign("extra_items", $extra_items);

    //--- get nex run begin point ---//
    $beginFetchFromId = 0;
    $beginFetchFromId = defineNextBeginFetchPoint($extra_items);

    $this->TML->assign("beginFetchFromId", $beginFetchFromId);
    $this->TML->assign("task_id", $task_id);    
    $this->Display('common:custom_templates/mail_dispatch/queue_progress.tpl');
}


/**
 * next ajax call will fetch data below this id
 * @param  $extra_items
 * @return int
 */
function defineNextBeginFetchPoint($extra_items)
{
    $beginFetchFromId = 0;
    if(!empty($extra_items)){
        $tmp = end($extra_items);
        $beginFetchFromId = $tmp["id"];
    }
    return $beginFetchFromId;
}
?>

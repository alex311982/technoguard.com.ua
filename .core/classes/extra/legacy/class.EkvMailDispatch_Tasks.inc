<?php
//#------------------- EkvMailDispatch_Tasks -------------------#//
class EkvMailDispatch_Tasks
{
    const PROCESSED_ITEMS_PER_RUN = 100;
    private $DB, $_dispatch_obj;
    function __construct(EkvMailDispatch $dispatch_obj = null)
    {
        if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }
        if(is_null($dispatch_obj) == false){
            $this->_dispatch_obj = $dispatch_obj;
        }
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }

    /**
     * creates queue and adds all receiveres to it,
     * cron script will process emails in background
     * @param  $dispatch_id from backend
     * @return void
     */
    function createTask($dispatch_id)
    {
        $RETURN = false;
        $dispatch_info = $this->_dispatch_obj->getDispatchInfo($dispatch_id);
        if(!empty($dispatch_info)){
            $actual_tasks = $this->_getNonfinishedTaskForDispatch($dispatch_id);

            //--- if there is already an actual task - don't add any more till it would be finished ---//
            if(!empty($actual_tasks)){
                return false;
            }
            $receivers = $this->_dispatch_obj->getReceivers($dispatch_info["event_id"],
                $dispatch_info["shipment_type"],
                $dispatch_info["test_mode"]
            );
            //pa($receivers);
            if(!empty($receivers)){

                //--- TRANS START ---//
                $this->DB->transactionStart();

                //--- create task base ---//
                $sql = "
                    INSERT INTO `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
                    SET
                        `dispatch_id` = $dispatch_id,
                        `date_creation` = NOW(),
                        `in_progress` = 0,
                        `is_active` = 0,
                        `is_finished` = 0
                ";
                $this->DB->query($sql);
                $task_id = $this->DB->get_insert_id();

                //--- create task extra rows ---//
                if($task_id > 0){
                    $sql_parts = array();
                    $sql_tpl = "
                        INSERT INTO `".EkvMailDispatch::TABLE_DISPATCH_TASK_EXTRA."`
                        (`task_id`, `email`, `is_processed`) VALUES
                    ";
                    $sql_row_tpl = '('.$task_id.', "%1s", 0)';
                    foreach($receivers as $rec_val){
                        $email_safe = mysql_real_escape_string($rec_val["email"]);
                        $sql_row = sprintf($sql_row_tpl, $email_safe);
                        array_push($sql_parts, $sql_row);
                    }
                    //$sql_parts = array_merge($sql_parts, $sql_parts, $sql_parts,$sql_parts,$sql_parts,$sql_parts,$sql_parts);

                    $sql_suffix = implode(', ', $sql_parts);
                    $sql_task_extra = $sql_tpl ."\n". $sql_suffix;
                    //echo "\n <pre> sql = $sql_task_extra </pre> <br/>\n";
                    $this->DB->query($sql_task_extra);
                    $RETURN = true;
                }

                //--- TRANS END ---//
                $this->DB->transactionCommit();
            }
        }
        return $RETURN;
    }

    function getFileLoggerObj($fname = null)
    {
        if(is_null($fname)){
            $fname = "mail_task_process";
        }
        $fLoggerObj = new EkvFileLogger($fname);
        return $fLoggerObj;
    }

    function processTaskQueue()
    {
        $fLoggerObj = $this->getFileLoggerObj();
        $task_id_in_progress = 0;
        
        //--- any task in progress ? ---//
        if($this->_checkIsAnyTaskInProgress($task_id_in_progress) == false){

            $active_task_id = $this->_getActiveTask();
            //--- there is no active task ---//
            if($active_task_id > 0 == false){
                                                        pa("calculate active task");
                $this->DB->transactionStart(); //TS

                $tmp_active_task_id = $this->_defineActiveTask();
                if($tmp_active_task_id > 0){
                    $this->_setActiveTaskId($tmp_active_task_id);
                    $active_task_id = $tmp_active_task_id;
                    $fLoggerObj->writeString("Task #{$active_task_id} is marked as active");
                }

                $this->DB->transactionCommit(); // TC

            }

            //--- recheck active task (defined automatically from task pool) ---//
            if($active_task_id > 0){
                //#------------------- process task here -------------------#//
                $this->_processTaskQueue($active_task_id);
            }else{
                echo "<h2>NO tasks to process!  </h2>\n";
            }

        }else{
            echo "<h2>still in progress $task_id_in_progress </h2>\n";
            $fLoggerObj->writeString("some task is still in progress - {$task_id_in_progress}");
        }
    }

    private function _processTaskQueue($task_id)
    {
        pa($task_id);
        //--- set task in progress ---//
        $this->_markTaskInProgress($task_id, 1);

        $fLoggerObj = $this->getFileLoggerObj();
        $task_items = $this->_getTaskExtraItemsSlice($task_id);
        //--- $max_cached_items are updated in db at once ---//
        $MAX_CACHED_ITEMS = 18;

        //--- there are extra items ot be processed ---//
        if(!empty($task_items)){
            $dispatch_info = $this->_getDispatchInfoByTaskId($task_id);
            if(!empty($dispatch_info)){
                $DISPATCH_PARAMS = array();
                $this->_dispatch_obj->performDispatch($dispatch_info, $DISPATCH_PARAMS);

                if(!empty($DISPATCH_PARAMS)){
                    $mailObj = $DISPATCH_PARAMS["mail_obj"];
                    $cached_task_ids = array();
                    $mails_sent = 0;

                    $body_original = $mailObj->Body; // !!!!
                    foreach($task_items as $tval){
                        $receiver_email = $tval["email"];
                        $current_fio = isset($tval["fio"]) ? $tval["fio"] : '';
                        $body_current = str_replace(array('%FIO%'), $current_fio, $body_original);
                        $mailObj->Body = $body_current;

                        $mailObj->ClearAddresses();
                        $mailObj->AddAddress($receiver_email);
//            sleep(2);
                        echo "email: {$tval["email"]} <br />";
                        if($mails_sent % 5 == 0){
                            @flush();
                        }
                        
                        $mailObj->Send();
                        $mails_sent++;
                        array_push($cached_task_ids, $tval["id"]);
                        if($mails_sent % $MAX_CACHED_ITEMS == 0){
                            echo "\n\n<h2>SENT $mails_sent  </h2>\n\n";
                            $this->_markTaskExtraItemsProcessed($cached_task_ids);
                            $cached_task_ids = array();
                            //exit;
                        }
                    }
                    //--- remain ids ---//
                    $this->_markTaskExtraItemsProcessed($cached_task_ids);

                    //--- check are there any unsent elements for futher script run ---//
                    $furhter_elements = $this->_getTaskExtraItemsSlice($task_id, true);
                    //--- no elements stayed to be sent - so we can mark task as finished ---//
                    if(empty($furhter_elements)){
                        $this->_markTaskAsFinished($task_id);
                    }
                }
            }

        }else{
            $this->_markTaskAsFinished($task_id);
        }

        //--- release task in progress ---//
        $this->_markTaskInProgress($task_id, 0);
        
    }

    private function _markTaskInProgress($task_id, $val_p = 0)
    {
        $sql = "
            UPDATE `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            SET
                `in_progress` = $val_p
            WHERE
                `id_task` = $task_id
        ";
        $this->DB->query($sql);
    }

    private function _markTaskExtraItemsProcessed($task_ids)
    {
        pa($task_ids);
        $task_ids_str = "";
        if(is_array($task_ids)){
            $task_ids_str = implode(',', $task_ids);
        }elseif(is_string($task_ids)){
            $task_ids_str = $task_ids;
        }

        if(!empty($task_ids_str)){
            $sql = "
                UPDATE `".EkvMailDispatch::TABLE_DISPATCH_TASK_EXTRA."`
                SET
                    `is_processed` = 1
                WHERE
                    `id` IN ({$task_ids_str})
            ";
            $this->DB->query($sql);
        }
    }


    private function _getDispatchInfoByTaskId($task_id)
    {
        $res = array();
        $task_info = $this->DB->bGetInfoById(EkvMailDispatch::TABLE_DISPATCH_TASK, $task_id, 'id_task');
        if(!empty($task_info)){
            $res = $this->_dispatch_obj->getDispatchInfo($task_info["dispatch_id"]);
        }
        return $res;
    }

    /**
     * get some portions of task extra elements
     * @param  $task_id
     * @param bool $check_only - if there is a need to check are there any unsent items pass TRUE here
     * @return array
     */
    private function _getTaskExtraItemsSlice($task_id, $check_only = false)
    {
        $limit = self::PROCESSED_ITEMS_PER_RUN;
        if($check_only == true){
            $limit = 1;
        }
        $sql = "
            SELECT dte.id, dte.email, u.fio
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK_EXTRA."` dte
                LEFT JOIN `".EkvRoomHelper::TABLE_USERS."` u
                    ON u.email = dte.email
            WHERE
                `task_id` = $task_id
                AND `is_processed` = 0
            ORDER BY
                id ASC
            LIMIT $limit
        ";
        $res = array();
        $this->DB->query($sql);
        while($row = $this->DB->fetch_row()){
            $res[$row["id"]] = $row;
        }
        return $res;
    }

    private function _markTaskAsFinished($task_id, $write_log = true)
    {
        $sql = "
            UPDATE `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            SET
                `in_progress` = 0,
                `is_active` = 0,
                `is_finished` = 1
            WHERE
                `id_task` = $task_id
        ";
        $this->DB->query($sql);

        if($write_log == true){
            $fLoggerObj = $this->getFileLoggerObj();
            $fLoggerObj->writeString("task #{$task_id} is finished");
        }

        $task_info = $this->getTaskInfoById($task_id);
        if(!empty($task_info)){
            $this->_dispatch_obj->setDispatchStatusSent($task_info["dispatch_id"]);
        }
    }

    private function _setActiveTaskId($task_id)
    {
        $sql = "
            UPDATE `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            SET
                `is_active` = 1
            WHERE
                `id_task` = $task_id
        ";
        $this->DB->query($sql);
    }
    private function _defineActiveTask()
    {
        $sql = "
            SELECT id_task
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            WHERE
                `in_progress` = 0
                AND `is_finished` = 0
                AND `is_active` = 0
            ORDER BY
                `id_task` ASC
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);
        $task_id = 0;
        if(!empty($res)){
            $task_id = $res["id_task"];
        }
        return $task_id;
    }

    private function _getActiveTask()
    {
        $sql = "
            SELECT id_task
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            WHERE
                `in_progress` = 0
                AND `is_finished` = 0
                AND `is_active` = 1
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);

        $task_id = 0;
        if(!empty($res)){
            $task_id = $res["id_task"];
        }
        return $task_id;
    }

    private function _checkIsAnyTaskInProgress(&$task_id_ref = null)
    {
        $sql = "
            SELECT id_task
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            WHERE
                `in_progress` = 1
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);

        if( !empty($res) && is_null($task_id_ref) == false ){
            $task_id_ref = $res["id_task"];
        }
        return !empty($res);
    }

    private function _getNonfinishedTaskForDispatch($dispatch_id)
    {
        $sql = "
            SELECT id_task
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            WHERE
                `dispatch_id` = $dispatch_id
                AND `is_finished` = 0
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    function deleteTaskCompletely($task_id)
    {
        $this->DB->transactionStart();

        //--- delete extra rows ---//
        $sql = "
            DELETE
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK_EXTRA."`
            WHERE
                `task_id` = $task_id
        ";
        $this->DB->query($sql);

        //--- delete base ---//
        $sql = "
            DELETE
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK."`
            WHERE
                id_task = $task_id
        ";
        $this->DB->query($sql);

        $this->DB->transactionCommit();
    }

    function getFinishedExtraItemes($task_id, $begin_from = 0)
    {
        $SQL_BEGIN_FROM = "";
        if($begin_from > 0){
            $SQL_BEGIN_FROM = " AND `id` > $begin_from ";
        }
        $sql = "
            SELECT id, email
            FROM `".EkvMailDispatch::TABLE_DISPATCH_TASK_EXTRA."`
            WHERE
                `task_id` = $task_id
                AND `is_processed` = 1
                $SQL_BEGIN_FROM
            ORDER BY
                id ASC
        ";
        $res = array();
        $this->DB->query($sql);

        while($row = $this->DB->fetch_row()){
            $res[$row["id"]] = $row;
        }
        return $res;
    }

    function getTaskInfoById($task_id)
    {
        $res = $this->DB->bGetInfoById(EkvMailDispatch::TABLE_DISPATCH_TASK, $task_id, 'id_task');
        return $res;
    }
}
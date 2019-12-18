<?php
namespace Ekv\Schedule;
use EkvGlobalHelper, DB, EkvException, EkvFileLogger;
use \Ekv\AudioParser\CmdComposer;


/**
 * is called via cron to check reparse queue and reparse(reslice mp3 files) when needed
 * script must be called each 10-15 minutes
 * only one period per time is processed
 */
class ReparseQueue
{
    private $DB;

    const STATUS_ADDED = 1;
    const STATUS_IN_PROGRESS = 2;
    const STATUS_COMPLETED = 3;

    private $_fLoggerObj;

    function __construct()
    {
        $this->DB = DB::getInstance();
        $this->_fLoggerObj = new EkvFileLogger("cron_reparse_period");
    }

    function checkPeriodAlreadyInQueue($period_id)
    {
        $period_id = intval($period_id);
        $statuses_accountable = array(self::STATUS_ADDED, self::STATUS_IN_PROGRESS);
        $statuses_accountable_STR = implode(",", $statuses_accountable);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE."`
            WHERE
                period_id = '{$period_id}'
                AND status IN ({$statuses_accountable_STR})
        ";
        $res = $this->DB->RunSql($sql, true);

        return !empty($res);
    }

    function addToQueue($period_id)
    {
        $period_id = intval($period_id);
        $sql = "
            INSERT INTO `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE."`
                (period_id, status, date_add)
            VALUES
                ('{$period_id}', '".self::STATUS_ADDED."', NOW())
        ";
        $this->DB->query($sql);

        $insert_id = $this->DB->get_insert_id();

        return $insert_id;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //  CRON REPARSE PERIODs
    /////////////////////////////////////////////////////////////////////////////////////////

    function mainCronReparse()
    {
        $this->_fLoggerObj->writeStringPlainDense(PHP_EOL . "------>>> STARTED");

        $queue_item = $this->_getPeriodIdForProcessing();

        if(!empty($queue_item)){
            $period_id = $queue_item["period_id"];
            $q_id = $queue_item["id"];

            //--- set queue item status in progress ---//
            $this->_setQueueItemStatus(self::STATUS_IN_PROGRESS, $q_id);

            $this->_fLoggerObj->writeStringPlainDense("Processing qID:{$q_id} periodID:{$period_id}");

            //--- perform reparse ---//
            $cmdComposerObj = new CmdComposer($period_id);
            $cmdComposerObj->main();

            //--- update status as completed ---//
            $this->_setQueueItemStatus(self::STATUS_COMPLETED, $q_id);
            $this->_fLoggerObj->writeStringPlainDense("Processing qID:{$q_id} periodID:{$period_id} COMPLETED <<<<-----");
        }
    }

    private function _getPeriodIdForProcessing()
    {
        $sql = "
            SELECT id
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE."`
            WHERE
                status = '".self::STATUS_IN_PROGRESS."'
        ";
        $in_progress_item = $this->DB->RunSql($sql);


        $this->_fLoggerObj->writeStringPlainDense("Check in progress item is_any: " . !empty($in_progress_item));

        //--- check maybe some reparsing process is already running ---//
        EkvException::ensure(empty($in_progress_item),
            "Queue item #{$in_progress_item["id"]} is already in progress while trying to get the new one");

        //--- get the most old still not processed item form the queue ---//
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE."`
            WHERE
                status = '".self::STATUS_ADDED."'
            ORDER BY
                date_add ASC
            LIMIT 1
        ";
        $item_for_processing = $this->DB->RunSql($sql);
        return $item_for_processing;
    }

    private function _setQueueItemStatus($status, $q_id)
    {
        $status = intval($status);
        $q_id = intval($q_id);

        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_REPARSE_QUEUE."`
            SET
                status = '{$status}'
            WHERE
                id = '{$q_id}'
        ";
        $this->DB->query($sql);
    }

    

}

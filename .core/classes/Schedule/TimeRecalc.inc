<?php
namespace Ekv\Schedule;
use EkvGlobalHelper, DB, EkvCommonHelper;

/**
 * calculates end time and duration for period items and updates with new values
 */
class TimeRecalc
{
    private $DB;
    private $_periodID;
    private $_periodInfo;

    function __construct($period_id)
    {
        $this->DB = DB::getInstance();
        $this->_periodID = intval($period_id);

        //--- period info ---//
        $schedRootObj = new ScheduleRoot();
        $this->_periodInfo = $schedRootObj->getPeriodInfo($this->_periodID);
    }

    function mainRecalcTimeFields()
    {
        //$this->_periodItems = $this->_getPeriodItems();
        $period_items_prepared = array();
        $period_items_src = $this->_getPeriodItems();

        /**
         * calculate end time and duration for period items
         */
        if(!empty($period_items_src)){
            $current = null;
            $next = null;
            foreach($period_items_src as $item_val){
                $next = $item_val; //important position for algorithm

                //--- first iteration ---//
                if($current === null){
                    $current = $item_val;
                    continue;
                }

                $period_items_prepared[] = $this->_calcTimeEnd($current, $next);

                $current = $item_val; //important position for algorithm
            }
            $period_items_prepared[] = $this->_calcTimeEnd($current, null);

            $this->_updatePeriodItemsPrepared($period_items_prepared);
        }
        //pa($period_items_prepared);
    }

    /**
     * construct single query for updating all items
     * http://stackoverflow.com/a/3466/1101589
     * 
     * @param $period_items_prepared
     */
    private function _updatePeriodItemsPrepared($period_items_prepared)
    {
        $sql_tpl = "
            INSERT INTO `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
                (`id_item`, `item_time_end`, `duration`, `dt_full`)
            VALUES
                %1\$s
            ON DUPLICATE KEY UPDATE
                `item_time_end` = VALUES(`item_time_end`),
                `duration` = VALUES(`duration`),
                `dt_full` = VALUES(`dt_full`)
        ";

        $values = array();
        foreach($period_items_prepared as $val){
            $values[] = "({$val["id_item"]}, '{$val["item_time_end"]}', '{$val["duration"]}', '{$val["dt_full"]}')";
        }
        $values_str = EkvCommonHelper::implodeWithComma($values);

        $sql = sprintf($sql_tpl, $values_str);
        $this->DB->query($sql);
    }

    private function _calcTimeEnd($current, $next)
    {
        //--- for the last item in result set $next item is null - correct it manually ---//
        if(is_null($next)){
            $next["item_time"] = ScheduleHelper::MAX_DAY_TIME;
            $next["item_time_seconds"] = 86399; //seconds in a day
        }

        $res = $current;
        $res["item_time_end"] = $next["item_time"];
        $res["duration"] = $next["item_time_seconds"] - $current["item_time_seconds"];

        $res["dt_full"] = "{$this->_periodInfo["per_date"]} {$current["item_time"]}";

        return $res;
    }

    private function _getPeriodItems()
    {
        $sql = "
            SELECT id_item, item_time, TIME_TO_SEC(`item_time`) as `item_time_seconds`
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                period_id = '{$this->_periodID}'
                AND item_enabled = 1
            ORDER BY
                item_time ASC
        ";
        $res = $this->DB->RunSql($sql, true, "id_item");
        return $res;
    }

}

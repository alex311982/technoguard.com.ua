<?php
namespace Ekv\Schedule;
use Ekv\Helpers\System\Lang;
use EkvGlobalHelper, DB, EkvException, EkvDateHelper;

/**
 * live schedule (period + items), not tpl
 */
class ScheduleRoot
{
    private $DB;


    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    static function isDateValid($date_mysql)
    {
        $is_valid = false;
        $parts = explode('-', $date_mysql);

        if(sizeof($parts) == 3){
            $is_valid = checkdate($parts["1"], $parts["2"], $parts["0"]);
        }

        return $is_valid;
    }

    /**
     * preiod like '03-01-2012' may be only one for channel, it's a container for schedule items
     * @param $channel_id
     * @param $period_date_mysql
     * @return array
     */
    function checkPeriodExists($channel_id, $period_date_mysql)
    {
        $channel_id = intval($channel_id);
        $period_date_mysql = mysqli_real_escape_string($this->DB->connection_id, $period_date_mysql);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                channel_id = '{$channel_id}'
                AND per_date = '{$period_date_mysql}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getPeriodNameFromDate($date)
    {
        $date_jq = EkvDateHelper::getJqDatePickerFormatedDate($date, false);
        return $date_jq;
    }

    function createPeriod($channel_id, $date)
    {
        EkvException::ensure($this->isDateValid($date), "Invalid date");
        $per_name = $this->getPeriodNameFromDate($date);
        $per_name = $this->DB->escape($per_name);

        $date = $this->DB->escape($date);

        $sql = "
            INSERT INTO `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            (channel_id, per_name, per_date, per_enabled)
            VALUES
                ('{$channel_id}', {$per_name}, {$date}, 1)
        ";
        $this->DB->query($sql);
        $insert_id = $this->DB->get_insert_id();
        EkvException::ensure($insert_id > 0, "Period creation error");

        return $insert_id;
    }

    function getPeriodInfo($period_id)
    {
        $period_id = intval($period_id);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                id_period = '{$period_id}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getPeriodInfoExtended($period_id)
    {
        $period_id = intval($period_id);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                INNER JOIN `".EkvGlobalHelper::TABLE_CHANNEL."` c
                    ON c.id_channel = p.channel_id
            WHERE
                id_period = '{$period_id}'
        ";
        $res = $this->DB->RunSql($sql);

        if(!empty($res)){
            $php_date = EkvDateHelper::mysqlDate2PhpDate($res["per_date"]);
            $res["date_human"] = EkvDateHelper::getDateWithTranslatedMonth_DateOnly($php_date);
        }

        return $res;
    }

    function getPeriodInfoByDate($channel_id, $mysql_date)
    {
        $channel_id = intval($channel_id);
        $mysql_date = $this->DB->escape($mysql_date);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                channel_id = '{$channel_id}'
                AND per_date = {$mysql_date}
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

//    function periodItemHumanDate($date_mysql)
//    {
//        $res = EkvDateHelper::getDateWithTranslatedMonth_DateOnly(
//            EkvDateHelper::mysqlDate2PhpDate($date_mysql)
//        );
//
//        return $res;
//    }

    //#------------------- require separate classes -------------------#//

    function getChannels()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                ch_enabled = 1
            ORDER BY
                id_channel ASC
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function getPeriodList($channel_id)
    {
        $channel_id = intval($channel_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                channel_id = '{$channel_id}'
                AND per_enabled = 1
            ORDER BY
                per_date ASC
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    /**
     * @param $period_id
     * @param null $lang_id
     * @param bool $exclude_in_progress
     * @return array
     */
    function getPeriodItems($period_id, $lang_id = null, $exclude_in_progress = true)
    {      
        $period_id = intval($period_id);

        //--- playback type ---//
        $SQL_PLAYBACK_TYPE = "";
        if($exclude_in_progress){
            $SQL_PLAYBACK_TYPE = " AND playback_type != '".EkvGlobalHelper::PLAYBACK_TYPE_IN_EDITION."'  ";
        }

        //--- lang ---//
        $SQL_LANG = "";
        /*
         * Retrieve specific lang progs exclusively only if current lang is not main.
         * For main lang show all progs
         */
        if(Lang::isLangIDMain($lang_id) == false){
            $lang_id = intval($lang_id);
            $SQL_LANG = " AND `prog_lang` = '{$lang_id}' ";
        }

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                period_id = '{$period_id}'
                AND item_enabled = 1
                {$SQL_LANG}
                {$SQL_PLAYBACK_TYPE}
            ORDER BY
                item_time ASC
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true, 'id_item');
        return $res;
    }

    //#------------------- complete delete of grid period -------------------#//

    function deletePeriodCompletely($period_id)
    {
        $period_id = intval($period_id);

        $this->deletePeriodItemPresenterConnForTheWholePeriod($period_id);

        //--- delete period items ---//
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                period_id = '{$period_id}'
        ";
        $this->DB->query($sql);

        //--- delete period itself ---//
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                id_period = '{$period_id}'
        ";
        $this->DB->query($sql);
    }

    function deletePeriodItem($period_item_id)
    {
        $this->deletePeriodItemPresenterConn($period_item_id);

        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                id_item = '{$period_item_id}'
        ";
        $this->DB->query($sql);
    }


    private function deletePeriodItemPresenterConn($periodItemID)
    {
        $periodItemID = @intval($periodItemID);
        $sql = "
            DELETE FROM `".\EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            WHERE
                period_item_id = '{$periodItemID}'
        ";

        $this->DB->query($sql);
    }

    private function deletePeriodItemPresenterConnForTheWholePeriod($periodId)
    {
        $periodId = @intval($periodId);

        $sql = "
            DELETE p2pi FROM `".\EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."` p2pi
                INNER JOIN `".\EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                    ON p2pi.period_item_id = pi.id_item
            WHERE
                pi.period_id = '{$periodId}'
        ";
        $this->DB->query($sql);
    }

    /**
     * @param $period_id
     * @param null $lang_id
     * @param bool $exclude_in_progress
     * @return array
     */
    function getPeriodItemById($itemId)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                id_item = '{$itemId}'
                AND item_enabled = 1
            LIMIT 1
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql);
        return $res;
    }
}

<?php
namespace Ekv\Schedule;
use Ekv\Schedule\Formatting\PeriodItemFormatter;
use EkvGlobalHelper, DB, EkvException, EkvCommonHelper;

class PeriodItem
{
    private $DB;

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    function getPeriodItemInfo($period_item_id)
    {
        $period_item_id = intval($period_item_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                id_item = '{$period_item_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    function getPeriodItemInfoFull($period_item_id)
    {
        $period_item_id = intval($period_item_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    ON p.id_period = pi.period_id
                INNER JOIN `".EkvGlobalHelper::TABLE_CHANNEL."` c
                    ON c.id_channel = p.channel_id
            WHERE
                id_item = '{$period_item_id}'
        ";

        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getPeriodItemInfoFullAndFormatted($period_item_id)
    {
        $formatted = array();
        $base_info = $this->getPeriodItemInfoFull($period_item_id);
        if(!empty($base_info)){
            $piFromatterObj = new PeriodItemFormatter();
            $formatted = $piFromatterObj->mainFormatSinglePeriodItemWithBankFields($base_info);
        }

        return $formatted;
    }

    function getPeriodItemChannel($period_item_id)
    {
        $period_item_id = intval($period_item_id);
        $sql = "
            SELECT p.channel_id
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    ON pi.period_id = p.id_period
            WHERE
                id_item = '{$period_item_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return @$res["channel_id"];
    }

    /**
     * gets current item (taken from the page load) and defines is it still playing
     * if the prog has finished - define next item
     * doesn't intend for anything except LIVE PROG (for others calculations would be inappropriate)
     * @param $period_item_current
     * @return array|null
     */
    function getNextPeriodItemPlaying($period_item_current)
    {
        $period_item_current = intval($period_item_current);

        //--- define is current item still playing ---//
        $sql = "
            SELECT id_item, period_id, item_time, item_time_end,
                (TIME(NOW()) < `item_time_end`) as `is_still_playing`
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                id_item = '{$period_item_current}'
        ";
        $current_item = $this->DB->RunSql($sql);

        $next_item = null;
        //--- if already finished - try to fetch next item ---//
        if(
            !empty($current_item)
            && $current_item["is_still_playing"] == 0
        ){
            $next_item = $this->_nextPeriodItem($current_item);
        }

        return $next_item;
    }

    /**
     * takes next item of the same period excluding playback type editing
     * @param $current_item
     * @return array|null
     */
    private function _nextPeriodItem($current_item)
    {
        $sql = "
            SELECT id_item, period_id, item_time, item_time_end, playback_type
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                `period_id` = '{$current_item["period_id"]}'
                AND `item_time` >= '{$current_item["item_time_end"]}'
                AND `playback_type` != ".EkvGlobalHelper::PLAYBACK_TYPE_IN_EDITION."
            ORDER BY
                item_time ASC
            LIMIT 1
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";

        $next_item = $this->DB->RunSql($sql);

        $res = null;
        //--- update next item only if it is not in edition now otherwise hold current item ---//
        if(
            !empty($next_item)
            //&& $next_item["playback_type"] != EkvGlobalHelper::PLAYBACK_TYPE_IN_EDITION
        ){
            $res = $next_item;
        }

        return $res;
    }

    static function convertDurationSecondsToHumanFormat($duration_sec)
    {
        // http://stackoverflow.com/a/3172358/1101589
        return gmdate("H:i:s", $duration_sec);
    }


}

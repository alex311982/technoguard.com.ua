<?php
namespace Ekv\Schedule\EmailReport;
use Ekv\Helpers\System\Lang;
use Ekv\Schedule\Formatting\PeriodItemFormatter;
use EkvGlobalHelper, DB;
use \Ekv\Schedule\PeriodItem;
use \Ekv\Schedule\ScheduleRoot;

class PrepareData
{
    private $DB;

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    private function _getChannels()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                ch_enabled = 1
        ";
        $res = $this->DB->RunSql($sql, true, "id_channel");
        return $res;
    }

    static function nextDayDate()
    {
        $dtObj = new \DateTime();
        //$dtObj->setDate(2013, 3, 26); //!!!!!!!!!!!!!!!!!!!!!!
        $dtObj->modify("+1 day");
        $next_day_date = $dtObj->format(\EkvDateHelper::MYSQL_DATE_ONLY);
        return $next_day_date;
    }

    private function _getNextDayPeriodIds()
    {
//        $next_day_date = '';
//        $dtObj = new \DateTime();
//        //$dtObj->setDate(2013, 3, 26); //!!!!!!!!!!!!!!!!!!!!!!
//        $dtObj->modify("+1 day");
//        $next_day_date = $dtObj->format(\EkvDateHelper::MYSQL_DATE_ONLY);
        $next_day_date = self::nextDayDate();

        $next_day_date = $this->DB->escape($next_day_date);

        $sql = "
            SELECT p.*
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
            WHERE
                p.per_date = {$next_day_date}
        ";
        $res = $this->DB->RunSql($sql, true, "id_period");
        return $res;
    }

    /**
     * period items groupped by period id
     * @param $next_day_period_ids
     * @return array
     */
    private function _getPeriodItems($next_day_period_ids)
    {
        $period_items = array();
        if(!empty($next_day_period_ids)){
            $next_day_period_ids_str = \EkvCommonHelper::implodeWithComma($next_day_period_ids);
            $next_day_period_ids_str = \EkvCommonHelper::validateIntValuesInImplodedStr($next_day_period_ids_str);

            $sql = "
                SELECT p.*, l.lang_name, pt.pb_name as `playback_type_name`
                FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` p
                    LEFT JOIN `".EkvGlobalHelper::TABLE_LANG."` l
                        ON l.id_lang = p.prog_lang
                    LEFT JOIN `".EkvGlobalHelper::TABLE_PROG_PLAYBACK_TYPE."` pt
                        ON pt.id = p.playback_type
                WHERE
                    period_id IN({$next_day_period_ids_str})
                ORDER BY
                    period_id ASC,
                    item_time ASC
            ";
            $this->DB->query($sql);
            $period_items_raw = $this->DB->RunSql($sql, true);

            //--- append bank fields ---//
            $piFormatterObj = new PeriodItemFormatter(Lang::LANG_ID_MAIN);
            $period_items_raw = $piFormatterObj->mainFormatPeriodItemsWithBankFields($period_items_raw);

            foreach($period_items_raw as $row){
                $row["duration_human"] = PeriodItem::convertDurationSecondsToHumanFormat($row["duration"]);
                $period_id = $row["period_id"];
                $period_items[$period_id][] = $row;
            }
        }

        return $period_items;
    }

    function mainPrepareStructuredData()
    {
        $channels = $this->_getChannels();
        $next_day_periods = $this->_getNextDayPeriodIds();

        if(!empty($next_day_periods)){
            $next_day_period_ids = array();
            array_walk($next_day_periods, function($item)use(&$next_day_period_ids){
                $next_day_period_ids[] = $item["id_period"];
            });

            //--- gropped by period ids ---//
            $period_items = $this->_getPeriodItems($next_day_period_ids);

            //--- append period items to periods list ---//
            if(!empty($period_items)){
                foreach($period_items as $period_id => $period_items_val){
                    $next_day_periods[$period_id]["items"] = $period_items_val;
                }
            }

            //--- append periods to channels ---//
            foreach($next_day_periods as $period_val){
                $channel_id = $period_val["channel_id"];
                if(isset($channels[$channel_id])){
                    $channels[$channel_id]["period"] = $period_val;
                }
            }
        }

        return $channels;
    }


    //#------------------- single  -------------------#//

    /**
     * @see mainPrepareStructuredData
     * emulate return result to be the same as of mainPrepareStructuredData method
     * it would be used in sending
     * @param $period_id
     * @return array
     */
    function mainPrepareSingleChannelStructuredData($period_id)
    {
        $channels = array();
        $period_info = $this->_getPeriodInfo($period_id);
        if(!empty($period_info)){
            $channel_id = $period_info["channel_id"];
            $channelObj = new \EkvChannel();
            $channel_info = $channelObj->getChannelInfo($channel_id);
            \EkvException::ensure(!empty($channel_info), "Channel info is empty");

            //--- the same structure as in mainPrepareStructuredData ---//
            $period_items = $this->_getPeriodItems(array($period_id));
            $period_info["items"] = isset($period_items[$period_id]) ? $period_items[$period_id] : array();
            $channel_info["period"] = $period_info;
            $channels[$channel_id] = $channel_info;
        }

        return $channels;
    }

    private function _getPeriodInfo($period_id)
    {
        $sRootObj = new ScheduleRoot();
        $period_info = $sRootObj->getPeriodInfo($period_id);
        return $period_info;
    }

}

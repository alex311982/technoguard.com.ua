<?php
namespace Ekv\AudioParser\Diff;
use EkvGlobalHelper, DB, EkvDateHelper, DateTime, EkvFileLogger;
use \Ekv\AudioParser\CmdComposer;

/**
 * must be run hourly at about xx:30 xx:40
 * reparses mp3 files for current day for each active channel
 */
class DiffParser_ch1
{
    private $DB;

    /**
     * @var EkvFileLogger
     */
    private $_fLoggerObj;

    /**
     * @var int php ts of current time
     * as relative point for calculations
     */
    private $_parseTimestamp;

    function __construct()
    {
        $this->DB = DB::getInstance();
        $this->_fLoggerObj = new EkvFileLogger("mp3_diff");
        $this->_parseTimestamp = time();
    }

    private function _log($msg)
    {
        $this->_fLoggerObj->writeStringPlainDense($msg);
    }

    /**
     * reparse for specific day
     * @param $human_readable
     */
    function setParseTimeHuman($human_readable)
    {
        //strtotime("2013-01-09 02:40:03");
        $this->_parseTimestamp = strtotime($human_readable);;
    }

    function mainAllChannels()
    {
        $this->_log(PHP_EOL . "[START] define periods");
        $calc_date = $this->_getCalculationDate();
        $periods = $this->_getChannelsWithDayPeriods($calc_date["date"]);

        //--- reparse files for each channel-period for the day ---//
        if(!empty($periods)){
            foreach($periods as $period_val){
                $period_id = $period_val["id_period"];
                $this->_log("---->>> started periodID {$period_id}");

                $cmdComposerObj = new CmdComposer($period_id);
                $cmdComposerObj->main();
                unset($cmdComposerObj);

                $this->_log("<<<--- ended periodID {$period_id}");
            }
        }
    }

    private function _getCalculationDate()
    {
        /**
         * parse request comes with a delay
         * for example hour 10 to 11 we parse at about 11:30, so we need to define which was the previous hour
         * at 00:30 we parse 23:00 hour of the previous day
         */
        $dtObj = new DateTime();
        $dtObj->setTimestamp($this->_parseTimestamp); // for test
        $dtObj->modify("-1 hour");

        //--- set mins and secs as zero (bound of period) ---//
        $hour = $dtObj->format("G");
        $dtObj->setTime($hour, 0, 0);
        //pa($dtObj);exit;

        $date = $dtObj->format(EkvDateHelper::MYSQL_DATE_ONLY);
        $time_begin = $dtObj->format(EkvDateHelper::MYSQL_TIME_ONLY);
        //--- bound of hour ---//
        $dtObj->setTime($hour, 59, 59);
        $time_end = $dtObj->format(EkvDateHelper::MYSQL_TIME_ONLY);
        //$date_full = $dtObj->format(EkvDateHelper::MYSQL_DATE_TIME);

        $res = array(
            //"datetime" => $date_full,
            "date" => $date,
            "time_begin" => $time_begin,
            "time_end" => $time_end,
        );
        //pa($res);
        return $res;
    }

    private function _getChannelsWithDayPeriods($date_period)
    {
        $date_period = $this->DB->escape($date_period);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."`
            WHERE
                per_date = {$date_period}
                AND per_enabled = 1
                AND channel_id = 1
        ";

        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

}

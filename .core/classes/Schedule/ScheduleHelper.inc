<?php
namespace Ekv\Schedule;
use EkvGlobalHelper, DB, EkvCommonHelper;

class ScheduleHelper
{
    private $DB;

    const MAX_DAY_TIME = "23:59:59";

//    const PLAYBACK_TYPE_LIVE = 2,
//        PLAYBACK_TYPE_RECORD = 3,
//        PLAYBACK_TYPE_TRANSMISSION = 4
//    ;


    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    function getPlaybackTypeOptions()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PLAYBACK_TYPE."`
            WHERE
                pb_enabled = 1
        ";

        $options = EkvCommonHelper::createOptionsSimple($sql, 'id', 'pb_name');
        return $options;
    }

    function getProgLangOptions()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_LANG."`
            WHERE
                lang_enabled = 1
        ";

        $options = EkvCommonHelper::createOptionsSimple($sql, 'id_lang', 'lang_name');
        return $options;
    }

    /**
     * allowed format '00:00:00' 24h
     * @param $time_str
     * @return bool
     */
    static function isTimeValid($time_str)
    {
        $res_raw = preg_match("/^(2[0-3]|[01][0-9]):([0-5][0-9]):([0-5][0-9])$/", $time_str);

        $res_bool = (bool)$res_raw;
        return $res_bool;
    }

}

<?php
namespace Ekv\Schedule\Ether;
use EkvGlobalHelper, EkvDateHelper;

class Ether
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    /**
     * Commonly used filter condition, which doesn't take into account "in edition" items
     * @param $field_name
     * @return string
     */
    function conditionInEdition($field_name = 'pi.playback_type')
    {
        $sql_cond = " AND {$field_name} != '".\EkvGlobalHelper::PLAYBACK_TYPE_IN_EDITION."' ";
        return $sql_cond;
    }

    static function conditionInEditionSt($field_name = 'pi.playback_type')
    {
        $obj = new self();
        return $obj->conditionInEdition($field_name);
    }

    /**
     * Basic info of period item which is on air currently,
     * or if "in edition" prog is on air now - show last "non-edition" item
     * TODO for new day when the first prog is IN_EDITION empty array will be returned, that's a bug and we should watch in perv day
     * @param $channel_id
     * @return array
     */
    function getLivePeriodItemBasic($channel_id)
    {
        $date_time_now = $this->_getDateTimeNow();

        $sql = "
            SELECT pi.id_item, pi.item_time
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD."` p
                    ON pi.period_id = p.id_period
            WHERE
                p.channel_id = '{$channel_id}'
                AND pi.item_enabled = 1
                ".$this->conditionInEdition('pi.playback_type')."
                AND p.per_date = '{$date_time_now["date"]}'
                AND pi.item_time <= '{$date_time_now["time"]}'
            ORDER BY
                item_time DESC
            LIMIT 1
        ";
        //$sql = "SELECT pi.id_item FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."` pi order by rand() limit 1";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    private function _getDateTimeNow()
    {
        $ts = time();
        //$ts = strtotime("2013-02-22 13:00:06");

        $res = array(
            'time' => date(EkvDateHelper::MYSQL_TIME_ONLY, $ts),
            'date' => date(EkvDateHelper::MYSQL_DATE_ONLY, $ts),
        );
        return $res;
    }
}

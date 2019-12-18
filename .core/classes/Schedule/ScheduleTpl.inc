<?php
namespace Ekv\Schedule;
use EkvGlobalHelper, DB, EkvCommonHelper;

class ScheduleTpl
{
    private $DB;

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    function getTplInfo($tpl_id, $fields = "*")
    {
        $tpl_id = EkvCommonHelper::getIntOrZero($tpl_id);

        $sql = "
            SELECT {$fields}
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ROOT."`
            WHERE
                id_tpl = '{$tpl_id}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getTplPlusChannelInfo($tpl_id)
    {
        $tpl_id = intval($tpl_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ROOT."` t
                INNER JOIN `".EkvGlobalHelper::TABLE_CHANNEL."` c
                    ON t.channel_id = c.id_channel
            WHERE
                id_tpl = '{$tpl_id}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getTplItemChannel($tpl_item_id)
    {
        $sql = "
            SELECT t.channel_id
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ROOT."` t
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."` ti
                    ON ti.tpl_id = t.id_tpl
            WHERE
                id_tpl_item = '{$tpl_item_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return @$res["channel_id"];
    }

    /**
     * allowed format '00:00:00' 24h
     * @param $time_str
     * @return bool
     */
    function isTimeValid($time_str)
    {
        return ScheduleHelper::isTimeValid($time_str);
    }

    function deleteTplItemCompletely($tpl_item_id)
    {
        $tpl_item_id = intval($tpl_item_id);
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."`
            WHERE
                id_tpl_item = '{$tpl_item_id}'
        ";

        $this->DB->query($sql);
    }

    function getTplItems($tpl_id)
    {
        $tpl_id = intval($tpl_id);
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."`
            WHERE
                tpl_id = '{$tpl_id}'
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function deleteTplRootCompletely($tpl_id)
    {
        $tpl_id = intval($tpl_id);
        //--- delete tpl items ---//
        $tpl_items = $this->getTplItems($tpl_id);

        foreach($tpl_items as $tpl_item_val){
            $this->deleteTplItemCompletely($tpl_item_val["id_tpl_item"]);
        }

        //--- delete tpl root itself ---//
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ROOT."`
            WHERE
                id_tpl = '{$tpl_id}'
        ";
        $this->DB->query($sql);
    }


}

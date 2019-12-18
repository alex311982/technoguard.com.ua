<?php
namespace Ekv\Schedule;
use Ekv\Bank\ProgPresenter;
use EkvGlobalHelper, EkvCommonHelper, DB, EkvException;

/**
 * copies schedule contents to corresponding live schedule by creating period item
 */
class ScheduleTplCopy
{
    private $DB;
    private $_tplID;
    private $_tplInfo;
    private $_periodDate;

    function __construct($tpl_id, $period_date)
    {
        $this->DB = DB::getInstance();

        $this->_tplID = intval($tpl_id);
        $this->_periodDate = $period_date;

        EkvException::ensure(ScheduleRoot::isDateValid($this->_periodDate), "Date is invalid");

        $schedTplObj = new ScheduleTpl();
        $this->_tplInfo = $schedTplObj->getTplPlusChannelInfo($tpl_id);
        EkvException::ensure(!empty($this->_tplInfo), "TplID error");
    }

    private function _getChannelID()
    {
        return $this->_tplInfo["channel_id"];
    }

    function mainCopy()
    {
        $schedRoot = new ScheduleRoot();
        $period_exists = $schedRoot->checkPeriodExists($this->_getChannelID(), $this->_periodDate);

        EkvException::ensure(!$period_exists, "Period already exists");

        $period_id = $schedRoot->createPeriod($this->_getChannelID(), $this->_periodDate);
        $this->_copyInnerItems($period_id);

        $timeRecalc = new TimeRecalc($period_id);
        $timeRecalc->mainRecalcTimeFields();
    }

    private function _copyInnerItems($period_id)
    {
        //--- copy direct fields ---//
        $common_fields_ar = array(
            "prog_id",
            "prog_name",
            "prog_presenter",
            "audio_link",
            "playback_type",
            "duration",
            "prog_lang",
            "prog_photo",
            "prog_responsible_person",
            "prog_note",
            "show_on_main",
            "is_prog_repeatable",
            "is_archivable",
            "prog_descr"
        );
        $common_fields_str = EkvCommonHelper::implodeWithComma($common_fields_ar);

        /*
         * tpl_item_id_tmp is used to have direct correspondence of id_tpl_item <<-->> tpl_item_id_tmp
         * which is used to facilitate copying of presenter connections.
         * If we wouldn't created and properly save this fields we would have to avoid this single query and do it
         * in the loop.
         */
        $sql = "
            INSERT INTO `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            (
                {$common_fields_str},
                `period_id`,
                `tpl_item_id_tmp`,
                `item_time`,
                `item_enabled`
            )
            SELECT
                {$common_fields_str},
                {$period_id},
                `id_tpl_item`,
                `tpl_item_time`,
                1
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."`
            WHERE
                tpl_id = '{$this->_tplID}'
                AND tpl_item_enabled = 1
        ";
        $this->DB->query($sql);

        //--- copy presenter connections ---//
        $this->copyPresenterConnections($period_id);
    }

    /**
     * Main method to copy presenter connections while copying all contents of tpl items to period items
     * Implemented in optimized way to avoid hundreds of mini queries but to put all thing into single query
     * @param $period_id_created
     */
    function copyPresenterConnections($period_id_created)
    {
        $tpl2period_items_mapping = $this->_createPeriodItemIdToTplItemIdMapping($period_id_created);
        $presenters2tpl_items_conn = $this->_getPresenters2TplItemConnections();
        $insert_values = $this->_createInsertValues($presenters2tpl_items_conn, $tpl2period_items_mapping);

        $this->_createPeriodItemConnSql($insert_values);
    }

    /**
     * Using tpl_item_id_tmp reconstruct which period_item_id inserted corresponds to tpl_item_id to have proper
     * mapping for creating insert statement later
     * @param $period_id
     * @return array
     */
    private function _createPeriodItemIdToTplItemIdMapping($period_id)
    {
        $sql = "
            SELECT
                `id_item` as `period_item_id`,
                `tpl_item_id_tmp` as `tpl_item_id`
            FROM `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            WHERE
                period_id = '{$period_id}'
                AND tpl_item_id_tmp > 0
        ";

        $res = array();
        $this->DB->query($sql);

        while($row = $this->DB->fetch_row()){
            $res[$row["tpl_item_id"]] = $row["period_item_id"];
        }

        return $res;
    }

    private function _getPresenters2TplItemConnections()
    {
        $sql = "
            SELECT p2ti.*
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2TPL_ITEM."` p2ti
                INNER JOIN `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."` ti
                    ON p2ti.tpl_item_id = ti.id_tpl_item
                    AND ti.tpl_id = '{$this->_tplID}'
            WHERE
                1
        ";
        return $this->DB->RunSql($sql, true);
    }

    /**
     * iterate through connections and format prepared insert values structure for presenter to appropriate period_item
     * connection depending on current tpl_item to presenter connections
     * @param $presenters2tpl_items_conn
     * @param $tpl2period_items_mapping
     * @return array
     */
    private function _createInsertValues($presenters2tpl_items_conn, $tpl2period_items_mapping)
    {
        $insert_values = array();
        if(!empty($presenters2tpl_items_conn)){
            foreach($presenters2tpl_items_conn as $tpli_conn_val){
                $tpl_item_id_connected = $tpli_conn_val["tpl_item_id"];
                EkvException::ensure(isset($tpl2period_items_mapping[$tpl_item_id_connected]),
                    "No appropriate period_item_id found for tpl_item_id");

                $period_item_id = $tpl2period_items_mapping[$tpl_item_id_connected];

                $insert_values[] = array(
                    "presenter_id" => $tpli_conn_val["presenter_id"],
                    "period_item_id" => $period_item_id
                );
            }
        }

        return $insert_values;
    }

    /**
     * create final insert query for copy connections and perform it
     * @param $insert_values
     * @return bool
     */
    private function _createPeriodItemConnSql($insert_values)
    {
        $res = false;
        $sql_base = "
            INSERT INTO `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            (`presenter_id`, `period_item_id`)
            VALUES
                %1\$s
        ";

        $sql_values_array = array();
        foreach($insert_values as $ins_val){
            $presenter_id = intval($ins_val["presenter_id"]);
            $period_item_id = intval($ins_val["period_item_id"]);

            $sql_values_array[] = "({$presenter_id}, {$period_item_id})";
        }

        //--- there are connections, which should be copied ---//
        if(!empty($sql_values_array)){
            $sql_values_str = EkvCommonHelper::implodeWithComma($sql_values_array);
            $sql = sprintf($sql_base, $sql_values_str);
            $this->DB->query($sql);
            $res = true;
        }

        return $res;
    }
}

<?php
namespace Ekv\Bank;

use EkvGlobalHelper, DB, imgResizeSettings;

class Prog
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    /**
     * prog options with cyrillic / latin names
     * @return array
     */
    function getProgsOptions()
    {
        $sql = "
            SELECT id_prog, prog_name, prog_latin_name
            FROM `".EkvGlobalHelper::TABLE_PROG."`
            WHERE
                `prog_enabled` = 1
            ORDER BY
                id_prog ASC
        ";

        $res = $this->DB->RunSql($sql, true);

        $options = array();
        foreach($res as $row){
            $prog_id = $row["id_prog"];
            $opt_name = "{$row["prog_name"]}";

            //--- append latin name if provided ---//
            if(!empty($row["prog_latin_name"])){
                $opt_name .= " / {$row["prog_latin_name"]}";
            }

            $opt_name .= " [{$prog_id}]";

            $options[$prog_id] = $opt_name;
        }

        return $options;
    }

    function getProgInfo($prog_id, $fields = null)
    {
        $prog_id = intval($prog_id);
        if(is_null($fields)){
            $fields = "id_prog, prog_name, prog_enabled, prog_latin_name";
        }

        $sql = "
            SELECT {$fields}
            FROM `".EkvGlobalHelper::TABLE_PROG."`
            WHERE
                id_prog = '{$prog_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    /**
     * Deletes prog record, all connections in tpl_items and period_items, maybe photos in future
     * @param $prog_id
     */
    function deleteProgCorrectly($prog_id)
    {
        $prog_info = $this->getProgInfo($prog_id);

        if(!empty($prog_info)){
            $this->_deleteProgConnections($prog_id);

//            if(!empty($prog_info["photo_file"])){
//                $photo_size_setting = imgResizeSettings::getSettingsItem("progPhotos");
//                $obj_upload = new \ManageUpload($photo_size_setting["base_photo_path"], "", "");
//                $obj_upload->size_settings = $photo_size_setting["size_settings"];
//                $obj_upload->removeAllSizesPhoto($prog_info["photo_file"]);
//            }

            $sql = "
                DELETE
                FROM `".EkvGlobalHelper::TABLE_PROG."`
                WHERE
                    `id_prog` = '$prog_id'
            ";
            $this->DB->query($sql);
        }
    }

    private function _deleteProgConnections($prog_id)
    {
        $prog_id = intval($prog_id);

        //--- from tpl items ---//
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_TPL_ITEM."`
            SET
                `prog_id` = 0
            WHERE
                `prog_id` = '{$prog_id}'
        ";
        $this->DB->query($sql);

        //--- from period items ---//
        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_SCHEDULE_PERIOD_ITEM."`
            SET
                `prog_id` = 0
            WHERE
                `prog_id` = '{$prog_id}'
        ";
        $this->DB->query($sql);
    }
}

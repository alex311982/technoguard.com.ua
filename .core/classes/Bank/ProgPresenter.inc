<?php
namespace Ekv\Bank;

use Ekv\Helpers\System\Lang;
use EkvGlobalHelper, DB;

class ProgPresenter
{
    private $DB;

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    function getPresenterOptions()
    {
        $sql = "
            SELECT id_presenter, fio, latin_fio
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER."`
            WHERE
                `presenter_enabled` = 1
            ORDER BY
                id_presenter ASC
        ";
        $res = $this->DB->RunSql($sql, true);

        $options = array();
        foreach($res as $row){
            $presenter_id = $row["id_presenter"];
            $opt_name = "{$row["fio"]}";

            //--- append latin name if provided ---//
            if(!empty($row["latin_fio"])){
                $opt_name .= " / {$row["latin_fio"]}";
            }

            $opt_name .= " [{$presenter_id}]";

            $options[$presenter_id] = $opt_name;
        }

        return $options;
    }

    function getPresenterToTplItemConnections($tpl_item_id)
    {
        $tpl_item_id = intval($tpl_item_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2TPL_ITEM."`
            WHERE
                tpl_item_id = '{$tpl_item_id}'
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    function getPresenterToPeriodItemConnections($period_item_id)
    {
        $period_item_id = intval($period_item_id);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            WHERE
                period_item_id = '{$period_item_id}'
        ";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    /**
     * @param $presenter_id
     * @param null $fields
     * @return array
     */
    function getPresenterInfo($presenter_id, $fields = null)
    {
        $presenter_id = intval($presenter_id);
        if(is_null($fields)){
            $fields = "id_presenter, fio, photo_file, presenter_enabled, latin_fio";
        }

        $sql = "
            SELECT {$fields}
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER."`
            WHERE
                id_presenter = '{$presenter_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }

    /**
     * Deletes presenter record, all connections in tpl_items and period_items, photos
     * @param $presenter_id
     */
    function deletePresenterCorrectly($presenter_id)
    {
        $presenter_info = $this->getPresenterInfo($presenter_id);

        if(!empty($presenter_info)){
            $this->_deletePresenterConnections($presenter_id);

            if(!empty($presenter_info["photo_file"])){
                $photo_size_setting = \imgResizeSettings::getSettingsItem("presenterPhotos");
                $obj_upload = new \ManageUpload($photo_size_setting["base_photo_path"], "", "");
                $obj_upload->size_settings = $photo_size_setting["size_settings"];
                $obj_upload->removeAllSizesPhoto($presenter_info["photo_file"]);
            }

            $sql = "
                DELETE
                FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER."`
                WHERE
                    `id_presenter` = '$presenter_id'
            ";
            $this->DB->query($sql);
        }
    }

    private function _deletePresenterConnections($presenter_id)
    {
        $presenter_id = intval($presenter_id);

        //--- from tpl_items ---//
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2TPL_ITEM."`
            WHERE
                `presenter_id` = '{$presenter_id}'
        ";
        $this->DB->query($sql);

        //--- from period items ---//
        $sql = "
            DELETE
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER2PERIOD_ITEM."`
            WHERE
                `presenter_id` = '{$presenter_id}'
        ";
        $this->DB->query($sql);
    }

    function mainPageRotation($limit = 20)
    {

        //echo(LANG::getLangCode());
//                        AND in_rotation = 1

        //                 

        if (Lang::getInstance()->getLangCode(true) == '') $l_ = 'uk'; else $l_ = Lang::getInstance()->getLangCode(true);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_PROG_PRESENTER."`
            WHERE
                presenter_enabled = 1
                AND `".$l_."` = 1
            ORDER BY
                id_presenter ASC
            LIMIT {$limit}
        ";

        $pres_raw = $this->DB->RunSql($sql, true);

        $pres_prepared = array();
        foreach($pres_raw as $pval){
            $presFormatterObj = new PresenterFormatter(Lang::isCurrentLangIDMain(), $pval);
            $pres_prepared[] = $presFormatterObj->mainFormat(true);
        }

        return $pres_prepared;
    }
}

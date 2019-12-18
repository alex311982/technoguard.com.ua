<?php
namespace Ekv\StatPage;
use Ekv\Helpers\System\FileUpload\EntityImageUpload;
use Ekv\StatPage\Tags\SpTagsRoot;
use EkvGlobalHelper;

class SpRoot
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function getTranslationInfo($spID, $langID)
    {
        $spID = intval($spID);
        $langID = intval($langID);

        $sql = "
            SELECT l.lang_code, l.lang_name, tl.*
            FROM `".EkvGlobalHelper::TABLE_STAT_PAGE_TRANSL."` tl
                INNER JOIN `".EkvGlobalHelper::TABLE_LANG."` l
                    ON l.id_lang = tl.lang_id
            WHERE
                sp_id = '{$spID}'
                AND lang_id = '{$langID}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getTranslID($spID, $langID)
    {
        $spID = intval($spID);
        $langID = intval($langID);

        $sql = "
            SELECT tl.id_transl
            FROM `".EkvGlobalHelper::TABLE_STAT_PAGE_TRANSL."` tl
            WHERE
                sp_id = '{$spID}'
                AND lang_id = '{$langID}'
        ";

        $res = $this->DB->RunSql($sql);

        return !empty($res) ? $res["id_transl"] : 0;
    }

    function getTranslationInfoByID($translID, $fields = "lang_id, sp_id")
    {
        $translID = intval($translID);

        $sql = "
            SELECT {$fields}
            FROM `".EkvGlobalHelper::TABLE_STAT_PAGE_TRANSL."`
            WHERE
                id_transl = '{$translID}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    static function sqlMainCond()
    {
        $sql_part = " sp.enabled = 1 AND sp.is_published = 1 ";
        return $sql_part;
    }

//    static function sqlMainCondDate($mysql_now = null)
//    {
//        if(is_null($mysql_now)){
//            $mysql_now = \EkvDateHelper::mysqlNow();
//        }
//
//        $sql_part = self::sqlMainCond() . " AND sp.date_pub <= '{$mysql_now}' ";
//        return $sql_part;
//    }

    static function getNewsFields($table_alias = "sp.")
    {
        $res = array(
            'id_transl',
            'sp_id',
            'lang_id',
            'sp_title',
//            'is_top',
//            'is_main',
            'last_editor',
            'is_published',
            'enabled',
            'date_created',
//            'date_pub',
            'photo_file',

            //'text_short',
            //'text_long'
        );

        foreach($res as &$rval){
            $rval = "{$table_alias}{$rval}";
        }

        return implode(', ', $res);
    }

    function deleteNewsTranslationCompletely($spTranslID)
    {
        $spTranslID = @intval($spTranslID);

        $translInfo = $this->getTranslationInfoByID($spTranslID, '*');
        if(empty($translInfo)){
            return false;
        }

        //--- delete news photo ---//
        $photoFile = $translInfo["photo_file"];
        if(!empty($photoFile)){
            $mockVars = array();
            $entityImgUploadObj = new EntityImageUpload(new \Entity(), "statPagePhotos", $mockVars, false);
            $entityImgUploadObj->removeAllSizePhotos($photoFile);
        }

        //--- delete tag connections ---//
        $newsTagsRoot = new SpTagsRoot();
        $newsTagsRoot->removeTcTagConnections($spTranslID);


        //--- delete transl row ---//
        $sql = "
            DELETE
            FROM `".\EkvGlobalHelper::TABLE_STAT_PAGE_TRANSL."`
            WHERE
                id_transl = '{$spTranslID}'
        ";
        $this->DB->query($sql);

        return true;
    }
}
 
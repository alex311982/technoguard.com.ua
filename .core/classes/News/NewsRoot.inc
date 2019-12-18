<?php
namespace Ekv\News;
use Ekv\Helpers\System\FileUpload\EntityImageUpload;
use Ekv\Helpers\System\Lang;
use Ekv\News\Tags\NewsTagsRoot;
use EkvGlobalHelper;

class NewsRoot
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function getTranslationInfo($newsID, $langID)
    {
        $newsID = intval($newsID);
        $langID = intval($langID);
 
        $sql = "
            SELECT l.lang_code, l.lang_name, tl.*
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
                INNER JOIN `".EkvGlobalHelper::TABLE_LANG."` l
                    ON l.id_lang = tl.lang_id
            WHERE
                news_id = '{$newsID}'
                AND lang_id = '{$langID}'
        ";
        $res = $this->DB->RunSql($sql);

        return $res;
    }

    function getTranslID($newsID, $langID)
    {
        $newsID = intval($newsID);
        $langID = intval($langID);

        $sql = "
            SELECT tl.id_transl
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
            WHERE
                news_id = '{$newsID}'
                AND lang_id = '{$langID}'
        ";

        $res = $this->DB->RunSql($sql);

        return !empty($res) ? $res["id_transl"] : 0;
    }

    function getTranslatedNewsByIds($newsIDs, $langID)
    {
        $langID = intval($langID);

        $newsIDs = implode(',', $newsIDs);

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
            WHERE
                news_id IN ({$newsIDs})
                AND lang_id = '{$langID}'
        ";

        return $this->DB->RunSql($sql, true);
    }

    function getPaginatedTranslatedNewsByIds($newsIDs, $langID)
    {
        $langID = intval($langID);

        $newsIDs = implode(',', $newsIDs);

        $this->DB->per_page = 10;

        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
            WHERE
                news_id IN ({$newsIDs})
                AND lang_id = '{$langID}'
        ";

        $result = $this->DB->RetrieveRecordsSqlOptimized($sql, true, false);

        if(isset($result["records"])){
            $newsFromatter = new NewsFormatter();
            $result["records"] = $newsFromatter->prepareMultipleItems($result["records"]);
        }

        return $result;
    }

    function getTranslationInfoByID($translID, $fields = "lang_id, news_id")
    {
        $sql = "
            SELECT tl.*, moder.news_lang_uk AS moder_news_lang_uk, moder.stuff_lang_uk AS moder_stuff_lang_uk, backend_user.uid_global
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
            LEFT JOIN `".EkvGlobalHelper::TABLE_USER_BACKEND."` backend_user
                ON backend_user.uid_global = tl.last_editor
            LEFT JOIN `".EkvGlobalHelper::TABLE_MODER."` moder 
                ON backend_user.outer_user_id = moder.id_moder
            WHERE
                    tl.id_transl = '{$translID}'
            LIMIT 1
        ";

        $res = $this->DB->RunSql($sql, true);

        return current($res);
    }

    static function sqlMainCond()
    {
        $sql_part = " n.enabled = 1 AND n.is_published = 1 ";
        return $sql_part;
    }

    static function sqlMainCondDate($mysql_now = null)
    {
        if(is_null($mysql_now)){
            $mysql_now = \EkvDateHelper::mysqlNow();
        }

        $sql_part = self::sqlMainCond() . " AND n.date_pub <= '{$mysql_now}' ";
        return $sql_part;
    }

    static function getNewsFields($table_alias = "n.")
    {
        $res = array(
            'id_transl',
            'news_id',
            'lang_id',
            'news_title',
            'is_top',
            'is_main',
            'last_editor',
            'is_published',
            'enabled',
            'date_created',
            'date_pub',
            'photo_file',

            //'text_short',
            //'text_long'
        );

        foreach($res as &$rval){
            $rval = "{$table_alias}{$rval}";
        }

        return implode(', ', $res);
    }

    function deleteNewsTranslationCompletely($newsTranslID, $user = null, $permContainer = null)
    {
        $newsTranslID = @intval($newsTranslID);

        $translInfo = $this->getTranslationInfoByID($newsTranslID, '*');
        if(empty($translInfo)){
            return false;
        }

        $canBeDeleted = true;

        if ($user && $permContainer && (int)$translInfo["lang_id"] == 1) {

            //can be news or material viewd by current logged moder
            $sql = "
                        SELECT nb.type_document FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
                        LEFT JOIN `".\EkvGlobalHelper::TABLE_NEWS_BASE."` nb
                        ON ABS(tl.news_id) = nb.id_news
                        WHERE
                            tl.id_transl = '{$newsTranslID}'
                        LIMIT 1
                    ";

            $news = current($this->DB->RunSql($sql));
            $newsTypeDocument = explode(',', $news['type_document']);

            //get admins
            $sql = "SELECT * FROM `".\EkvGlobalHelper::TABLE_BASE_USER."`";
            $admins = $this->DB->query($sql);

            $adminLogins = [];
            foreach ($admins as $admin) {
                $adminLogins[$admin['login']] = $admin;
            }

            if (in_array($user->login, array_keys($adminLogins))) {
                $adminTypes = explode(',', $adminLogins[$user->login]['type_document_after_create']);

                array_intersect($adminTypes, $newsTypeDocument) ? $canBeDeleted = true : $canBeDeleted = false;
            } else {
                $stuffLangUkCurrentModer = $permContainer->checkStuffAccess('stuff_lang_uk');
                $langUkCurrentModer = $permContainer->checkLangAccess(LANG::LANG_ID_UK);

                $moderTypes = [];
                $stuffLangUkCurrentModer ? $moderTypes[] = 'R' : null;
                $langUkCurrentModer ? $moderTypes[] = 'N' : null;

                array_intersect($moderTypes, $newsTypeDocument) ? $canBeDeleted = true : $canBeDeleted = false;
            }
        }

        if ($canBeDeleted) {
            //--- delete news photo ---//
            $photoFile = $translInfo["photo_file"];
            if (!empty($photoFile)) {
                $mockVars = array();
                $entityImgUploadObj = new EntityImageUpload(new \Entity(), "newsPhotos", $mockVars, false);
                $entityImgUploadObj->removeAllSizePhotos($photoFile);
            }

            //--- delete tag connections ---//
            $newsTagsRoot = new NewsTagsRoot();
            $newsTagsRoot->removeTcTagConnections($newsTranslID);

            //--- delete transl row ---//
            $sql = "
                    DELETE
                    FROM `" . \EkvGlobalHelper::TABLE_NEWS_TRANSL . "`
                    WHERE
                        id_transl = '{$newsTranslID}'
                ";

            $this->DB->query($sql);
        }

        return true;
    }

}

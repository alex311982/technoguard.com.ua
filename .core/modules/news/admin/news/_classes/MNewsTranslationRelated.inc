<?php
namespace Ekv\B\News\News;
use Ekv\Helpers\System\Lang;
use EkvGlobalHelper;

class MNewsTranslationRelated
{
    private $DB;

    private $_newsID;
    private $_exceptLangID;

    function __construct($newsID, $exceptLangID = 0)
    {
        $this->DB = \DB::getInstance();
        $this->_newsID = intval($newsID);
        $this->_exceptLangID = intval($exceptLangID);
    }

    function mainGetTranslations()
    {
        $SQL_EXCEPT_LANG = "";
        /*
         * When exceptLangID is passed then ignore this specific translation
         */
        if($this->_exceptLangID > 0){
            $SQL_EXCEPT_LANG = " AND lang_id != '{$this->_exceptLangID}' ";
        }

        $sql = "
            SELECT l.lang_name, l.lang_code,
                tl.news_title as `tc_title`,
                tl.*
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
                INNER JOIN `".EkvGlobalHelper::TABLE_LANG."` l
                    ON l.id_lang = tl.lang_id
            WHERE
                news_id = '{$this->_newsID}'
                {$SQL_EXCEPT_LANG}
            ORDER BY
                lang_id ASC
        ";

        $res = $this->DB->RunSql($sql, true, 'id_transl');

        $langObj = Lang::getInstance();
        foreach($res as &$rval){
            $rval["lang_default"] = $langObj->isLangIDMain($rval["lang_id"]);
        }

        return $res;
    }

    function mainGetTranslation()
    {
        $SQL_EXCEPT_LANG = "";
        /*
         * When exceptLangID is passed then ignore this specific translation
         */
        if($this->_exceptLangID > 0){
            $SQL_EXCEPT_LANG = " AND lang_id != '{$this->_exceptLangID}' ";
        }

        $sql = "
            SELECT nb.type_document, tl.*, moder.news_lang_uk AS moder_news_lang_uk, moder.stuff_lang_uk AS moder_stuff_lang_uk, backend_user.uid_global
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl
            LEFT JOIN `".\EkvGlobalHelper::TABLE_NEWS_BASE."` nb
                        ON tl.news_id = nb.id_news
            LEFT JOIN `".EkvGlobalHelper::TABLE_USER_BACKEND."` backend_user
                ON backend_user.uid_global = tl.last_editor
            LEFT JOIN `".EkvGlobalHelper::TABLE_MODER."` moder 
                ON backend_user.outer_user_id = moder.id_moder
            WHERE
                news_id = '{$this->_newsID}'
                AND lang_id = '{$this->_exceptLangID}'
            LIMIT 1
        ";

        $res = $this->DB->RunSql($sql, true, 'id_transl');

        return current($res);
    }

    function mainGetAllTranslation()
    {
        $SQL_EXCEPT_LANG = "";
        /*
         * When exceptLangID is passed then ignore this specific translation
         */
        if($this->_exceptLangID > 0){
            $SQL_EXCEPT_LANG = " AND lang_id != '{$this->_exceptLangID}' ";
        }

        $sql = "
            SELECT tl.id_transl, tl.is_locked_in_admin, tl.period_item_track_link
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl        
            WHERE
                lang_id = '1'
        ";

        $res = $this->DB->RunSql($sql, true, 'id_transl');

        return $res;
    }

    function setLockStatusAllLangs($status = 0, $newsID = null)
    {
        if (!$newsID) {
            $sql = "
            SELECT tl.news_id
            FROM `" . EkvGlobalHelper::TABLE_NEWS_TRANSL . "` tl        
            WHERE
                id_transl = '{$this->_newsID}'
            LIMIT 1
        ";

            $res = $this->DB->RunSql($sql, true);

            $temp = current($res);

            $newsID = $temp['news_id'];
        }

        $sql = "
            UPDATE `".EkvGlobalHelper::TABLE_NEWS_TRANSL."` tl  
            SET is_locked_in_admin='{$status}'      
            WHERE news_id = '{$newsID}'
        ";

        $res = $this->DB->RunSql($sql, true);
    }

    function addEditorSaveCountAllLangs($globalUid)
    {
        $sql = "
            SELECT id_save, `count`
            FROM `" . EkvGlobalHelper::TABLE_NEWS_EDITOR_SAVED_COUNT . "`      
            WHERE
                news_id = '{$this->_newsID}'
                AND last_editor = '{$globalUid}'
            LIMIT 1
        ";

        $res = $this->DB->RunSql($sql, true);

        if ($res) {
            $temp = current($res);
            $newsSaveID = $temp['id_save'];
            $newsSaveCount = $temp['count'] + 1;
            $sql = "
                UPDATE `" . EkvGlobalHelper::TABLE_NEWS_EDITOR_SAVED_COUNT . "`
                SET `count`='{$newsSaveCount}'      
                WHERE id_save = '{$newsSaveID}'
            ";
        } else {
            $sql = "
                INSERT INTO `" . EkvGlobalHelper::TABLE_NEWS_EDITOR_SAVED_COUNT . "`  
                SET `count`='1', last_editor='{$globalUid}', news_id='{$this->_newsID}'
            ";
        }

        $res = $this->DB->RunSql($sql, true);
    }
}

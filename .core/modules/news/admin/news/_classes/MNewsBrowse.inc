<?php
namespace Ekv\B\News\News;
use Ekv\Helpers\System\Lang;
use Ekv\News\Permissions\NewsPermissionContainer;
use EkvGlobalHelper, DB;

/**
 * @deprecated
 */
class MNewsBrowse
{

    private $DB;
    /**
     * @var NewsPermissionContainer
     */
    private $_newsPermContainer;

    function __construct(DB $dbObj, NewsPermissionContainer $newsPermContainer)
    {
        $this->DB = $dbObj;
        $this->_newsPermContainer = $newsPermContainer;
    }

    function main()
    {
        $result = $this->_raw();
        $result = $this->_prepareRecords($result);
//pa($result);
        return $result;
    }

    private function _prepareRecords($result)
    {
        $res_records = isset($result["records"]) ? $result["records"] : array();
        if(!empty($res_records)){
            $news_ids = $this->_getNewsIds($res_records);
            $groupped_extra = $this->_getTranslationsGroupped($news_ids);

            foreach($res_records as &$rval){
                $news_id = $rval["news_id"]["value"];
                if(isset($groupped_extra[$news_id])){
                    $rval["man_translations"]["data"] = $groupped_extra[$news_id];
                }
            }

            $result["records"] = $res_records;
        }

        return $result;
    }

    private function _getNewsIds($res_records)
    {
        $news_ids = array();
        foreach($res_records as $val){
            $news_id = $val["news_id"]["value"];
            $news_ids[$news_id] = $news_id;
        }

        return $news_ids;
    }

    private function _getTranslationsGroupped($news_ids)
    {
        $groupped = array();
        $news_ids_str = \EkvCommonHelper::implodeWithComma($news_ids);

        $sql = "
            SELECT id_transl, news_id, lang_id, news_title
            FROM `".EkvGlobalHelper::TABLE_NEWS_TRANSL."`
            WHERE
                news_id IN ({$news_ids_str})
            ORDER BY
                news_id DESC,
                lang_id ASC
        ";
        //echo "\n <pre>$sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);

        foreach($res as $rval){
            $groupped[$rval["news_id"]][$rval["lang_id"]] = $rval;
        }

        $groupped = $this->_mergeGrouppedWithLang($groupped);

        return $groupped;
    }

    /**
     * $groupped_raw contains translations groupped by newsID
     * @param $groupped_raw
     * @return array
     */
    private function _mergeGrouppedWithLang($groupped_raw)
    {
        $formatted = array();
        $langs_all = Lang::getInstance()->getLangsAvailable();

        $allowed_lang_ids = $this->_newsPermContainer->getLangIdsActive();
        //pa($allowed_lang_ids);

        foreach($groupped_raw as $news_id => $lang_news){

            /*
             * Walk through the available langs and either fill spcific lang value with existing translation
             * or with default values (like creation links).
             * This allows to create proper edit/create links for specific lang
             */
            foreach($langs_all as $lang_id => $lang_val){
                $has_translation = false;
                $has_access = in_array($lang_id, $allowed_lang_ids);
                //$has_access = true;// todo remove
                $current_item = array();

                if(isset($lang_news[$lang_id])){
                    $current_item = $lang_news[$lang_id];
                    $has_translation = true;
                }

                $lang_vars = array(
                    "lang_id" => $lang_id,
                    'lang_code' => $lang_val["lang_code"],
                    'lang_name' => $lang_val["lang_name"]
                );

                $extra = $this->_getExtra($has_translation, $lang_id, $news_id);

                $current_item = array_merge($current_item, $lang_vars, $extra);

                $current_item["has_access"] = $has_access;

                $formatted[$news_id][$lang_id] = $current_item;
            }

        }

        return $formatted;
    }

    private function _getExtra($has_translation, $lang_id, $newsID)
    {
        $url = "#";
        if($has_translation){
            //$url = "/admin/news/news/news_edit.html?langID={$lang_id}&newsID={$newsID}";
            $url = MNewsHelper::getUrlEdit($lang_id, $newsID);
        }else{
            //$url = "/admin/news/news/news_new.html?langID={$lang_id}&newsID={$newsID}";
            $url = MNewsHelper::getUrlCreate($lang_id, $newsID);
        }

        $extra = array(
            'has_translation' => $has_translation,
            'edit_url' => $url
        );

        return $extra;
    }

    private function _raw()
    {
        $sql = "
            SELECT *, `id_news` as `news_id`
            FROM `" . EkvGlobalHelper::TABLE_NEWS_BASE . "` n
            WHERE
                1
                -- n.enabled = 1
            ORDER BY
                n.id_news DESC
        ";

        $result = $this->DB->RetrieveRecordsSqlOptimized($sql);
        return $result;
    }
}

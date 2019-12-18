<?php
namespace Ekv\TContent\Backend;

use Ekv\Helpers\Backend\BackendUserGlobal;
use Ekv\Helpers\System\Lang;
use Ekv\News\Permissions\NewsPermissionContainer;
use Ekv\TContent\Permissions\TcPermissionContainer;
use EkvGlobalHelper, DB, NewsNews, EkvDateHelper, Entity;

abstract class TranslBrowse
{
    /**
     * @var NewsNews
     */
    private $en;

    protected $DB;

    /**
     * @var NewsPermissionContainer
     */
    protected $_tcPermContainer;

    private $_tcLangAll;

    protected $_rootIDKey;// = "news_id";
    protected $_tcTitleKey;
    protected $_translTable;
    protected $_user;

    function __construct(Entity $entityObj, TcPermissionContainer $tcPermContainer)
    {
        $this->DB = $entityObj->DB;
        $this->en = $entityObj;
        $this->_init();

        $this->_tcPermContainer = $tcPermContainer;
    }

    abstract protected function _init();

    abstract protected function _getUrlEdit($langID, $tcID);
    abstract protected function _getUrlCreate($langID, $tcID);

    function main($user)
    {
        $this->_user = $user;

        $result = $this->_raw();

        $result = $this->_prepareRecords($result);

        $result = $this->_postFormatResult($result);

        return $result;
    }

    private function _postFormatResult($result)
    {
        if(isset($result["records"])){
            foreach($result["records"] as &$rval){
                $rval = $this->_postFormatResultRow($rval);
            }
        }

        return $result;
    }

    protected function _postFormatResultRow($row)
    {
        $row = $this->_applyTransTitleUrl($row);
        //$rval = $this->_markTopImportanceNews($rval);
        //$rval = $this->_prepareManDate($rval);
        return $row;
    }

    /**
     * Title url have to be linked to ukrainian translation always!
     * If moder  has no access to ukr - don't set url at all
     * @param $row
     * @return mixed
     */
    protected function _applyTransTitleUrl($row)
    {
        $news_id = $row[$this->_rootIDKey]["value"];
        $lang_id = Lang::LANG_ID_MAIN;

        $has_access = $this->_tcLangAll[$news_id][$lang_id]["has_access"];
        $has_translation = $this->_tcLangAll[$news_id][$lang_id]["has_translation"];

        if(!$has_access){
            return $row;
        }

        $url = "";
        if($has_translation){
            $url =  $this->_getUrlEdit($lang_id, $news_id);
        }else{
            $url = $this->_getUrlCreate($lang_id, $news_id);
        }

        $row[$this->_tcTitleKey]["href"] = $url;

        return $row;
    }

    protected function _raw()
    {
        $where = array();

        $lang_options = Lang::getInstance()->getLangAvailableOptions();
        $this->en->setSelectFieldOptionsUnconditional($lang_options, 'lang_id');

        $backUserGlobalObj = new BackendUserGlobal();
        $this->en->setSelectFieldOptionsUnconditional($backUserGlobalObj->getGlobalUserOptions(), 'last_editor');
        $result = $this->DB->RetrieveRecords("", $where);

        return $result;
    }

    private function _prepareRecords($result)
    {
        if(!isset($result["records"])){
            return $result;
        }

        $root_ids = $this->_getRootIds($result["records"]);
        //pa($news_ids);exit;
 
        /*
         * Preformatted news langs groupped by newsID
         */


        $tc_langs_all = $this->_getTranslationsGroupped($root_ids);
        $this->_tcLangAll = $tc_langs_all;
        
        foreach($result["records"] as $key => &$rval){

            $news_id_tmp = intval($rval[$this->_rootIDKey]["value"]);
            $transl_id = intval($rval["id_transl"]["value"]);
            
            $transl_langs = isset($tc_langs_all[$news_id_tmp]) ? $tc_langs_all[$news_id_tmp] : array();

            foreach($transl_langs as &$tl_val){
                $is_current = false;
                if(
                    isset($tl_val["id_transl"])
                    && $tl_val["id_transl"] == $transl_id
                ){
                    $is_current = true;
                }
                $tl_val["lang_code"] = strtoupper($tl_val["lang_code"]);
                $tl_val["is_current"] = $is_current;

		if ($is_current) {

			$current_lang_id = $tl_val['lang_id'];
		}

            }
//echo($is_current.' ');

            $rval["man_langs"]["data"]["langs"] = $transl_langs;
//print_r($rval["man_langs"]["data"]["langs"]);
//print_r($rval);
 
	    if ($rval['lang_id']['value'] != $current_lang_id) unset($result['records'][$key]);
        }
//print_r($result);
//print_r($rval["man_langs"]["data"]["langs"]);

        return $result;

    }

    private function _getRootIds($records)
    {
        $news_ids = array();
        foreach($records as $rval){
            $news_id_tmp = intval($rval[$this->_rootIDKey]["value"]);
            $news_ids[$news_id_tmp] = $news_id_tmp;
        }
        return $news_ids;
    }
 
    private function _getTranslationsGroupped($news_ids)
    {
        $groupped = array();
        $news_ids_str = \EkvCommonHelper::implodeWithComma($news_ids);

        $sql = "
            SELECT id_transl, lang_id, {$this->_rootIDKey}, {$this->_tcTitleKey}
            FROM `{$this->_translTable}`
            WHERE
                {$this->_rootIDKey} IN ({$news_ids_str})
            ORDER BY
                {$this->_rootIDKey} DESC,
                lang_id ASC
        ";
/*        echo "\n <pre>$sql </pre> <br/>\n";*/

        $res = $this->DB->RunSql($sql, true);

        foreach($res as $rval){
            $groupped[$rval[$this->_rootIDKey]][$rval["lang_id"]] = $rval;
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

        $allowed_lang_ids = $this->_tcPermContainer->getLangIdsActive();
        $stuffLangUkCurrentModer = $this->_tcPermContainer->checkStuffAccess('stuff_lang_uk');

        if ($stuffLangUkCurrentModer) {
            $allowed_lang_ids[LANG::LANG_ID_UK] = LANG::LANG_ID_UK;
        }

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
            $url = $this->_getUrlEdit($lang_id, $newsID);
        }else{
            $url = $this->_getUrlCreate($lang_id, $newsID);
        }

        $extra = array(
            'has_translation' => $has_translation,
            'edit_url' => $url
        );

        return $extra;
    }

}
 
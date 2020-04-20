<?php
namespace Ekv\Helpers\System;

/**
 * refactoring of frontend_get_path() legacy function
 */
class FrontendUrlParser
{
    private $_urlParts;
    private $_urlInput;

    private $_urlReady;

    private $DB;

    /**
     * lang from url
     * @var
     */
    private $_langCode;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function mainParse($url)
    {
        $this->_urlInput = $url;
        //$this->_parseLang();

        $this->_urlParts = parse_url($this->_urlInput);

        $this->_urlPartsPreprocess();

        $res = $this->_lookInModules();

        //Lang::getInstance()->setCurrentLangInfoByCode($this->_langCode);

//        if(empty($res)){
//            $res = $this->_lookInStaticPages();
//            if(empty($res)){
//                err404();
//            }
//        }

        return $res;
    }

    private function _urlPartsPreprocess()
    {
        if (
            empty($this->_urlInput)
            || '/' == $this->_urlInput
            || (
                isset($this->_urlParts["path"])
                && $this->_urlParts["path"] == '/'
            )
        ) {
            $this->_urlInput = "/index.html";
            $this->_urlParts['path'] = $this->_urlInput;
        }

        $url_ready = $this->_urlParts['path'];
        $url_ready = str_replace(PAGE_EXTENSION, '', $url_ready);
        $url_ready = ($url_ready{0} == '/') ? substr($url_ready, 1) : $url_ready;
        $url_ready = str_replace('"', '', $url_ready);

        $parts = explode('/', $url_ready);

        if (count($parts) > 1) {
            $url_ready = $parts[1];
            $_REQUEST['lang'] = $parts[0];
        } else {
            $url_ready = $parts[0];
        }

        $this->_urlReady = $url_ready;
    }

    /**
     * define lang and substitute $this->_urlInput with url without lang prefix
     */
    private function _parseLang()
    {
        /*
         * Try to match lang prefix in url
         * /de/ or /de
         * /de/grid/channel/period/items/list.html?periodID=482&channelID=1
         */
        preg_match('#^/(\w{2})(/|$)#', $this->_urlInput, $matches);

        //--- lang is passed ---//
        if(sizeof($matches) >= 2){
            $lang_code = $matches[1];
            $langObj = Lang::getInstance();

            /*
             * Eliminate lang from url for futher correct parsing.
             * Make url like it was passed without lang (for correct module or static page definition)
             *
             * /de -> /
             * /de/grid/channel/period/items/list.html?periodID=482&channelID=1 ->
             * /grid/channel/period/items/list.html?periodID=482&channelID=1
             */
            $url_without_lang = preg_replace('#^/\w{2}(/|$)(.*)#', '/$2', $this->_urlInput);

            /*
             * when lang code is incorrect against db records redirect user to the same page with defaul lang
             */
            if($langObj->isLangCodeCorrect($lang_code) == false){
                force_location($url_without_lang);
            }

            $this->_urlInput = $url_without_lang;
            $this->_langCode = $lang_code;
        }
    }

    private function _lookInModules()
    {
        $url_safe = $this->DB->escape($this->_urlReady);
        $sql = "
            SELECT `module`, `entity`, `action`
            FROM `base_url`
            WHERE
                `url` = {$url_safe}
            LIMIT 0,1
        ";
        $res = $this->DB->RunSql($sql);

        $final_res = null;
        if(!empty($res)){
            $final_res = $this->_finalResponse($res['module'], $res['entity'], $res['action']);
        }

        return $final_res;
    }

    private function _lookInStaticPages()
    {
        $url_safe  = $this->DB->escape($this->_urlReady);
        $url_with_ext = $this->_urlReady . PAGE_EXTENSION;
        $uri_with_ext_safe = $this->DB->escape($url_with_ext);
        $final_res = null;

        $langID = Lang::getInstance()->getLangID();

        $sql = "
            SELECT id_transl
            FROM `".\EkvGlobalHelper::TABLE_STAT_PAGE_TRANSL."`
            WHERE
                lang_id = '{$langID}'
                AND
                (
                    `sp_url` = {$url_safe}
                    OR `sp_url` = {$uri_with_ext_safe}
                )
             LIMIT 0,1
        ";
        $page = $this->DB->RunSql($sql);
        if(!empty($page)){
            $final_res = $this->_finalResponse('tcontent', 'statPageFr', 'sp_preview');
            $final_res["translation_id"] = $page["id_transl"];
        }

        return $final_res;
    }

    private function _finalResponse($module, $entity, $action)
    {
        return array(
            'module' => $module,
            'entity' => $entity,
            'action' => $action,
        );
    }
}

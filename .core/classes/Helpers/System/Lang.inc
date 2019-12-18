<?php
namespace Ekv\Helpers\System;

use EkvGlobalHelper;

class Lang
{
    const LANG_CODE_MAIN = "uk";
    const LANG_ID_MAIN = self::LANG_ID_UK; //see table ekv_lang

    const LANG_ID_UK = 1;
    const LANG_ID_EN = 2;
    const LANG_ID_DE = 3;
    const LANG_ID_RO = 4;
    const LANG_ID_RU = 5;

    private $DB;

    private $_langInfoCurrent;

    private function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    static function getInstance()
    {
        static $obj = null;
        if(is_null($obj)){
            $obj = new self();
        }

        return $obj;
    }

    function isLangCodeCorrect($lang_code)
    {
        $is_correct = false;
        $lang_info = $this->getLangInfoByCode($lang_code);

        if(
            !empty($lang_info)
            && $lang_info["lang_enabled"] == 1
        ){
            $is_correct = true;
        }
        return $is_correct;
    }

    function getLangInfoByCode($lang_code)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_LANG."`
            WHERE
                `lang_code` = ".$this->DB->escape($lang_code)."
        ";
        return $this->DB->RunSql($sql);
    }

    function getLangInfo($lang_id)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_LANG."`
            WHERE
                `id_lang` = ".$this->DB->escape($lang_id)."
        ";
        return $this->DB->RunSql($sql);
    }

    function setCurrentLangInfoByCode($lang_code)
    {
        if(empty($lang_code)){
            $lang_code = self::LANG_CODE_MAIN;
        }

        $lang_info = $this->getLangInfoByCode($lang_code);
        if(!empty($lang_info)){
            $this->_langInfoCurrent = $lang_info;
        }else{
            //--- if can define lang by input code - set main default lang ---//
            $this->_langInfoCurrent = $this->getLangInfoByCode(self::LANG_CODE_MAIN);
        }

        return $lang_info;
    }

    function getLangInfoCurrent()
    {
        return $this->_langInfoCurrent;
    }

    function getLangID()
    {
        $lang_id = 0;
        if(isset($this->_langInfoCurrent["id_lang"])){
            $lang_id = intval($this->_langInfoCurrent["id_lang"]);
        }

        return $lang_id;
    }

    static function getLangIDStatic()
    {
        return self::getInstance()->getLangID();
    }

    function getLangCode($empty_on_default = false)
    {
        $lang_code = self::LANG_CODE_MAIN;
        if (isset($this->_langInfoCurrent["lang_code"])) {
            $lang_code = $this->_langInfoCurrent["lang_code"];
        }

        if(
            $empty_on_default
            && $lang_code == self::LANG_CODE_MAIN
        ){
            $lang_code = '';
        }

        return $lang_code;
    }

    static function isLangIDMain($lang_id = null)
    {
        if(is_null($lang_id)){
            $lang_id = self::getInstance()->getLangID();
        }
        return $lang_id == self::LANG_ID_MAIN;
    }

    static function isCurrentLangIDMain()
    {
        return self::isLangIDMain();
    }

    function getLangsAvailable()
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_LANG."`
            WHERE
                lang_enabled = 1
            ORDER BY
                lang_sort ASC
        ";
        $res = $this->DB->RunSql($sql, true, 'id_lang');

        foreach($res as &$rval){
            $rval["url_flag"] = "/-images/lang/{$rval["lang_code"]}.png";
            $rval["url"] = "/{$rval["lang_code"]}/";
        }

        return $res;
    }

    function getLangAvailableOptions()
    {
        $langs_available = $this->getLangsAvailable();
        $options = array();

        foreach($langs_available as $lval){
            $options[$lval["id_lang"]] = "{$lval["lang_name"]} [{$lval["lang_code"]}]";
        }

        return $options;
    }

    function getLangsForTpl()
    {
        $langs_raw = $this->getLangsAvailable();
        foreach($langs_raw as &$lang_val){
            $lang_val["name_short_display"] = strtoupper($lang_val["lang_code"]);
        }

        $lang_id = $this->getLangID();

        \EkvException::ensure(isset($langs_raw[$lang_id]), "Active lang not found");

        $selected = $langs_raw[$lang_id];
        unset($langs_raw[$lang_id]);

        $res = array(
            'selected' => $selected,
            'others' => $langs_raw
        );

        return $res;
    }



}
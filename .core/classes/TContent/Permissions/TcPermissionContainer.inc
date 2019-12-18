<?php
namespace Ekv\TContent\Permissions;

use Ekv\Helpers\System\Lang;


/**
 * @property $hasPremoderation
 * @property $isSuperEditor
 */
abstract class TcPermissionContainer
{
    protected $_hasPremoderation;
    protected $_isSuperEditor;
    protected $_langFields;

//    //--- must be overrided ---//
//    protected $_fieldUk;
//    protected $_fieldEn;
//    protected $_fieldDe;
//    protected $_fieldRo;

    protected $_langMap = array(); // see _initLangMap()
//    array(
//        'news_lang_uk' => Lang::LANG_ID_UK,
//        'news_lang_en' => Lang::LANG_ID_EN,
//        'news_lang_de' => Lang::LANG_ID_DE,
//        'news_lang_ro' => Lang::LANG_ID_RO,
//    );

    /**
     * @var \EkvException
     */
    protected $ex;

    /**
     * @var TcHelper - descendant
     */
    protected $_tcHelperObj;

    function __construct($has_premoderation, $is_super_editor, $lang_fields)
    {
        $this->_initException();
        $this->_initOverridable();

        $this->_isSuperEditor = $is_super_editor == 1;
        $this->_hasPremoderation = $has_premoderation == 1;

        /*
         * Super editor can't have premoderation
         */
        if ($this->isSuperEditor) {
            $this->_hasPremoderation = false;
        }

        $this->_langFields = $lang_fields;
    }

    protected function _initOverridable()
    {
//        $this->_tcHelperObj = new TcHelper();
//        $this->_initLangMap();
    }

    protected function _initException()
    {
        $this->ex = new TcPermissionException();
    }
    
    protected function _initLangMap($uk, $en, $de, $ro, $ru)
    {
        $this->_langMap = array(
            $uk => Lang::LANG_ID_UK,
            $en => Lang::LANG_ID_EN,
            $de => Lang::LANG_ID_DE,
            $ro => Lang::LANG_ID_RO,
            $ru => Lang::LANG_ID_RU,
        );
    }

    function __get($name)
    {
        $key = "_{$name}";
        $this->ex->ensure(property_exists($this, $key), "Property doesn't exists {$key}");

        return $this->$key;
    }

    /**
     * LangIDs where user have access for editing
     * @return array
     */
    function getLangIdsActive()
    {
        $active_langs = array();
        foreach ($this->_langMap as $rawKey => $langID) {

            if (
                isset($this->_langFields[$rawKey])
                && $this->_langFields[$rawKey] == 1
            ) {
                $active_langs[$langID] = $langID;
            }

//            $active_langs[$langID] = $langID;
        }

//        $active_langs[5] = 5;

        return $active_langs;
    }

    private function _getLangFieldValueByID($langID)
    {
        $map = array_flip($this->_langMap);
        $lang_field = isset($map[$langID]) ? $map[$langID] : null;

        $this->ex->ensure(array_key_exists($lang_field, $this->_langFields), "Lang field value failed");

        return $this->_langFields[$lang_field];
    }

    function checkLangAccess($langID)
    {
        return $this->_getLangFieldValueByID($langID) == 1;
    }

    function checkStuffAccess($stuffFlag)
    {
        return $this->_langFields[$stuffFlag]  == 1;
    }
}
 
<?php
namespace Ekv\TContent\Permissions;

/**
 * @property $langUK
 * @property $langEN
 * @property $langDE
 * @property $langRO
 *
 * @property $hasPremoderationField
 * @property $superEditorField
 *
 * @property $sessionKey
 */
abstract class TcHelper
{
    private $_permLangFields;

    protected $_hasPremoderationField;
    protected $_superEditorField;

    protected $_sessionKey;

    function __construct()
    {
        $this->_init();
    }

    protected function _init()
    {
        //init lang
        //$this->_initPermLangFields()

        //init session key

        //init premoder and super editor
    }

    protected function _initPermLangFields($uk, $en, $de, $ro, $ru)
    {
        $this->_permLangFields = array(
            'langUK' => $uk,
            'langEN' => $en,
            'langDE' => $de,
            'langRO' => $ro,
            'langRU' => $ru,
        );
    }

    function __get($name)
    {
        if(in_array($name, array(
            'langUK', 'langEN', 'langDE', 'langRO', 'langRU'
        ))){
            return $this->_permLangFields[$name];
        }

        $innerName = "_{$name}";
        TcPermissionException::ensure(property_exists($this, $innerName), "Property doesn't exist");

        return $this->$innerName;

    }


}
 
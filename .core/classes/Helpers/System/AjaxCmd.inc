<?php
namespace Ekv\Helpers\System;
use SmartyClass, EkvCommonHelper;

abstract class AjaxCmd
{
    protected $DB;

    /**
     * @var SmartyClass
     */
    protected $tml;
    protected $data;

    function __construct($data = array(), SmartyClass $tml = null)
    {
        $this->DB = \DB::getInstance();
        $this->data = $data;
        $this->tml = $tml;

        $this->_init();
    }

    protected function _init(){}

    abstract function mainPerformCmd();

    protected function _assign($key, $val)
    {
        $this->tml->assign($key, $val);
    }

    protected function _display($resource_name)
    {
        $this->tml->display($resource_name);
    }

    protected function _fetch($resource_name)
    {
        return $this->tml->fetch($resource_name);
    }

    protected function _jsonHeader()
    {
        EkvCommonHelper::headerJson();
    }

    protected function _jsonEncode($raw)
    {
        return json_encode($raw);
    }

}
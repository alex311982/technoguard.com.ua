<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentDirectionheader extends Entity
{
    public $browseActionTranslation = "directionheader_list";

    public $PK = "id";

    public $directionheaderID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('directionheader'));

        $this->directionheaderID = @intval($_REQUEST["id"]);

        $this->_prepareDirectionheaderFields();
    }

    private function _prepareDirectionheaderFields()
    {
        $this->DB->table = 'directionheaders';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "directionheader");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Хедеры направлений";
        $breadcrumps []= array('title' => $title,
            'href' => 'directionheader_list');

        return $breadcrumps;
    }
}


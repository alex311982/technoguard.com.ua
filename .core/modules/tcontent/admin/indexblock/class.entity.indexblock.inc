<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentIndexblock extends Entity
{
    public $browseActionTranslation = "indexblock_list";

    public $PK = "id";

    public $indexblockID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('indexblock'));

        $this->indexblockID = @intval($_REQUEST["id"]);

        $this->_prepareDirectionheaderFields();
    }

    private function _prepareDirectionheaderFields()
    {
        $this->DB->table = 'indexblock';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "indexblock");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Блоки на стартовой странице";
        $breadcrumps []= array('title' => $title,
            'href' => 'indexblock_list');

        return $breadcrumps;
    }
}


<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class DirectionDirectionquote extends Entity
{
    public $browseActionTranslation = "directionquote_list";

    public $PK = "id";

    public $directionquoteID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('directionquote'));

        $this->directionquoteID = @intval($_REQUEST["id"]);

        $this->_prepareDirectionquoteIDFields();
    }

    private function _prepareDirectionquoteIDFields()
    {
        $this->DB->table = 'directionquotes';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "directionquote");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Цитаты направлений";
        $breadcrumps []= array('title' => $title,
            'href' => 'directionquote_list');

        return $breadcrumps;
    }
}


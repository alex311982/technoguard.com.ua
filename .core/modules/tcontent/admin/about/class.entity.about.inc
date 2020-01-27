<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentAbout extends Entity
{
    public $browseActionTranslation = "about_list";

    public $PK = "id";

    public $aboutID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('about'));

        $this->aboutID = @intval($_REQUEST["id"]);

        $this->_prepareAboutIDFields();
    }

    private function _prepareAboutIDFields()
    {
        $this->DB->table = 'abouts';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "about");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "О нас";
        $breadcrumps []= array('title' => $title,
            'href' => 'about_list');

        return $breadcrumps;
    }
}


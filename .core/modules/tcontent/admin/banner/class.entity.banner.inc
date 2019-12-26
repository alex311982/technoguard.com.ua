<?php

use Ekv\Helpers\System\Lang;
use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontHelper;
use Ekv\TContent\Banner\BannerRoot;
use Ekv\TContent\Slide\SlideRoot;

class TcontentBanner extends Entity
{
    public $browseActionTranslation = "banner_list";

    public $PK = "id";

    public $bannerID;

    function ManualBeforeExecuteIncludeAction()
    {
        $this->_initNew($this->PK, $this->browseActionTranslation, array('banner'));

        $this->bannerID = @intval($_REQUEST["id"]);

        $this->_prepareBannerFields();
    }

    private function _prepareBannerFields()
    {
        $this->DB->table = 'banners';
        $this->DB->fields = $this->DB->IncludeSpecificFields(__FILE__, "banner");
    }

    function SetBreadcrump()
    {
        $breadcrumps = array();

        $title = "Баннеры";
        $breadcrumps []= array('title' => $title,
            'href' => 'banner_list');

        return $breadcrumps;
    }

    /**
     * @return SortOrder
     */
    public function createSortOrderObj()
    {
        return new SortOrder("banners");
    }
}

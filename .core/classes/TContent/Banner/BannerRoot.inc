<?php
namespace Ekv\TContent\Banner;

class BannerRoot
{
    const TYPE_MAIN_TOP = 1;
    const TYPE_MAIN_MIDDLE = 2;
    const TYPE_SIDE_TOP = 3;
    const TYPE_SIDE_BOTTOM = 4;
    const TYPE_FOOTER_MICRO = 5;

    const SORT_ORDER_DIRECTION = 'DESC';

    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    public function getBannerInfo($bannerID)
    {
        return $this->DB->bFieldsGetInfoById(\EkvGlobalHelper::TABLE_BANNER, $bannerID, 'id_banner', '*');
    }
}

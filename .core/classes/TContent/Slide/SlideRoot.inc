<?php
namespace Ekv\TContent\Slide;

class SlideRoot
{
    private $DB;

    const SORT_ORDER_DIRECTION = "DESC";

    const SLIDE_TYPE_TOP = 1;
    const SLIDE_TYPE_BOTTOM = 2;
    const SLIDE_TYPE_SPECIAL_PROJECTS = 3;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    public function getSlideInfo($slideID)
    {
        return $this->DB->bFieldsGetInfoById(\EkvGlobalHelper::TABLE_SLIDE, $slideID, 'id_slide',
            'id_slide, lang_id, sl_name, sl_url, sl_type, sl_date, sl_photo, sort_order, sl_enabled');
    }

    public function getImgResizeSettingsKey($slideType)
    {
        $res = array(
            SlideRoot::SLIDE_TYPE_TOP => 'slidePhotos',
            SlideRoot::SLIDE_TYPE_BOTTOM => 'slidePhotosBottom',
            SlideRoot::SLIDE_TYPE_SPECIAL_PROJECTS => 'slidePhotosSpecialProjects'
        );

        return $res[$slideType];
    }

    public function getPhotosRelativePath($slideType)
    {
        $key = $this->getImgResizeSettingsKey($slideType);
        $imgResizeSettings = \imgResizeSettings::getSettingsItem($key);
        $basePhotoPathRelative = $imgResizeSettings["base_photo_path_relative"];
        return $basePhotoPathRelative;
    }
}

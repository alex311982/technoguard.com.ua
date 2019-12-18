<?php
namespace Ekv\TContent\Banner;

class BannerFormatter
{
    protected $imgSize = 'original';

    function __construct()
    {
    }

    public function setImgSize($size)
    {
        $this->imgSize = $size;
    }

    function prepareSingleItem($item)
    {
        $item["url_img"] = $this->getImageUrl($item["ban_photo"], $this->imgSize);
        $item["url_preview"] = $this->getUrlPreview($item["ban_url"]);

        return $item;
    }

    public function getUrlPreview($url)
    {
        if (!empty($url)) {
            if (
                strpos($url, '://') === false //no protocol used
                && strpos($url, '/') !== 0
            ) {
                $url = "/{$url}";
            }
        }

        return $url;
    }

    function getImageUrl($photo_file, $size = "size1")
    {
        $url = "";
        $relativePhotoUrl = $this->getRelativePhotosUrl();

        if(!empty($photo_file)){
            $url = "{$relativePhotoUrl}{$size}/{$photo_file}";
        }

        return $url;
    }

    protected function getRelativePhotosUrl()
    {
        $imgResizeSettings = \imgResizeSettings::getSettingsItem('bannerPhotos');
        return $imgResizeSettings["base_photo_path_relative"];
    }

    function prepareMultipleItems($items)
    {
        foreach($items as &$item_val){
            $item_val = $this->prepareSingleItem($item_val);
        }

        return $items;
    }
}

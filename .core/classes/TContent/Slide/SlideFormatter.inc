<?php
namespace Ekv\TContent\Slide;

class SlideFormatter
{
    protected $imgSize = 'size2';
    protected $slideType;

    function __construct($slideType = SlideRoot::SLIDE_TYPE_TOP)
    {
        $this->slideType = @intval($slideType);
    }

    public function setImgSize($size)
    {
        $this->imgSize = $size;
    }

    function prepareSingleItem($item)
    {
        $item["url_img"] = $this->getImageUrl($item["sl_photo"], $this->imgSize);
        $item["url_preview"] = $this->getUrlPreview($item["sl_url"]);

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
//            $url = "{$relativePhotoUrl}{$size}/{$photo_file}";
            $url = "{$relativePhotoUrl}/original/{$photo_file}";
        }

        return $url;
    }

    protected function getRelativePhotosUrl()
    {
        $slideRoot = new SlideRoot();
        return $slideRoot->getPhotosRelativePath($this->slideType);
    }

    function prepareMultipleItems($items)
    {
        foreach($items as &$item_val){
            $item_val = $this->prepareSingleItem($item_val);
        }

        return $items;
    }
}

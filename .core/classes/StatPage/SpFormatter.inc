<?php
namespace Ekv\StatPage;

class SpFormatter
{
//    private $_imgSize = 'size1';
    private $_imgSize = 'big';

    private $_addPreviewUrlHost = false;

    function setImageSize($img_size_name)
    {
        $this->_imgSize = $img_size_name;
    }

    public function setAddPreviewUrlHost($addPreviewUrlHost)
    {
        $this->_addPreviewUrlHost = $addPreviewUrlHost;
    }

    function prepareSingleItem($sp_item)
    {
        $sp_item["url_img"] = $this->getImageUrl($sp_item["photo_file"], $this->_imgSize);
        $sp_item["url_preview"] = $this->getUrlPreview($sp_item["sp_url"]);
        //$sp_item["date_formatted"] = $this->formatDate($sp_item["date_pub"]);
//        $this->morePubDateFormats($sp_item);
//        $this->_cssClasses($sp_item);

        return $sp_item;
    }

    function prepareMultipleItems($news_items)
    {
        foreach($news_items as &$news_val){
            $news_val = $this->prepareSingleItem($news_val);
        }

        return $news_items;
    }

    function getUrlPreview($url)
    {
        $url = "/{$url}";
        \EkvCommonHelper::appendUrlWithLangPrefixRef($url);

        if($this->_addPreviewUrlHost){
            $url = \EkvCommonHelper::appendUrlWithSiteHostConf($url);
        }
        return $url;
    }

    function getImageUrl($photo_file, $size = "size1")
    {
        $url = "";
        if(!empty($photo_file)){
            $url = "/images/stat-page/{$size}/{$photo_file}";
        }

        return $url;
    }

//    function formatDate($mysql_date_raw)
//    {
//        return \EkvDateHelper::getJqDatePickerFormatedDate($mysql_date_raw, true);
//    }
//
//    function morePubDateFormats(&$raw)
//    {
//        $date_pub = $raw["date_pub"];
//
//        $time = \EkvDateHelper::getTimeOnly($date_pub);
//        $date_only = \EkvDateHelper::getJqDatePickerFormatedDate($date_pub, false);
//
//        $raw["date_ext"] = array(
//            'date' => $date_only,
//            'time' => $time
//        );
//    }

}
 
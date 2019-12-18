<?php
namespace Ekv\News;

use EkvDateHelper;

class NewsFormatter
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

    function prepareSingleItem($news_item)
    {
        $news_item["url_img"] = $this->getImageUrl($news_item["photo_file"], $this->_imgSize);
        $news_item["url_preview"] = $this->getUrlPreview($news_item["news_id"]);
        $news_item["date_formatted"] = $this->formatDate($news_item["date_pub"]);

        $news_item['ukr_status'] = $news_item["type_document"] ? "[" . $news_item["type_document"] . "]" : "";

        $this->morePubDateFormats($news_item);

        $this->_cssClasses($news_item);

        return $news_item;
    }

    private function _cssClasses(&$news_item)
    {
        $css_classes_str = "";

        $is_top = @intval($news_item["is_top"]);
        $is_main = @intval($news_item["is_main"]);

        if($is_main){
            $css_classes_str .= "n-main ";
        }

        if($is_top){
            $css_classes_str .= "n-top";
        }

        $news_item["css_classes"] = $css_classes_str;
    }

    function prepareMultipleItems($news_items)
    {
        foreach($news_items as &$news_val){
            $news_val = $this->prepareSingleItem($news_val);
        }

        return $news_items;
    }

    function getUrlPreview($item_id)
    {
        $url = "/news.html?newsID={$item_id}";
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
            $url = "/images/news/{$size}/{$photo_file}";
        }

        return $url;
    }

    function formatDate($mysql_date_raw)
    {
        return EkvDateHelper::getJqDatePickerFormatedDate($mysql_date_raw, true);
    }

    function morePubDateFormats(&$raw)
    {
        $datePub = $raw["date_pub"];

        $time = EkvDateHelper::getTimeOnly($datePub);
        $dateOnly = EkvDateHelper::getJqDatePickerFormatedDate($datePub, false);

        $phpDate = EkvDateHelper::mysqlDate2PhpDate($datePub);
        $dateSym = EkvDateHelper::getDateWithTranslatedMonth_DateOnly($phpDate);

        /*
         * Displays, today, yesterday or symbolic date
         */
        $dateVar = $dateSym;
        if ($phpDate >= strtotime("today")) {
            $dateVar =  "сьогодні";
        } elseif ($phpDate >= strtotime("yesterday")) {
            $dateVar = "вчора";
        }

        $raw["date_sym"] = array(
            'date' => $dateSym,
            'date_var' => $dateVar
        );

        $raw["date_ext"] = array(
            'date' => $dateOnly,
            'time' => $time
        );
    }

}
 
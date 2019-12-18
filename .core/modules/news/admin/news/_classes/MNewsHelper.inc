<?php
namespace Ekv\B\News\News;

class MNewsHelper
{
    static function getUrlEdit($langID, $newsID, $commingFrom = null)
    {
        $url = "/admin/news/news/news_edit.html?langID={$langID}&newsID={$newsID}";

        if(!is_null($commingFrom)){
            $url .= "&commingFrom={$commingFrom}";
        }
        return $url;
    }

    static function getUrlCreate($langID, $newsID)
    {
        $url = "/admin/news/news/news_new.html?langID={$langID}&newsID={$newsID}";
        return $url;
    }

}
 
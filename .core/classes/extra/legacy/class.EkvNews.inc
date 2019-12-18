<?php
/**
 * User: sunsey
 * Date: 26 ���� 2011
 */

class EkvNews
{
    const TABLE_NEWS = "ekv_news";
    const TABLE_NEWS_RUBRICS = "ekv_news_rubrics";
    static $newImageSettingsKey = 'newsItemPhoto';
    protected $DB, $newsImageSize = "size1",
        $_add_host // adds host for built urls
    ;
    function __construct(DB $db = null, $add_host = false)
    {
        if(is_null($db) == false){
            $this->initDB($db);
        }
        else if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }
        $this->setAddHost($add_host);
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }
    
    function setAddHost($add_host)
    {
        $this->_add_host = $add_host;
    }

//    function getRubricOptions()
//    {
//        $sql = "
//            SELECT *
//            FROM `".self::TABLE_NEWS_RUBRICS."`
//            WHERE
//                `rubric_enabled` = 1
//            ORDER BY
//                `rubric_name` ASC
//        ";
//        $res = $this->DB->RunSql($sql, true);
//        $options = array();
//        foreach($res as $rval){
//            $options[$rval["id_rubric"]] = $rval["rubric_name"];
//        }
//        return $options;
//    }

    function prepareDisplayData(&$raw_data)
    {
        if(!empty($raw_data)){
            foreach($raw_data as &$val){
                $this->prepareDisplaySingleItem($val);
            }
        }
    }

    function prepareDisplaySingleItem(&$raw_item_data)
    {
        $add_host = $this->_add_host;
        $raw_item_data["url_full"] = $this->getFullUrl($raw_item_data, $add_host);
        if(isset($raw_item_data["header_photo"])){
            $size = $this->getNewsImageSize();
            $raw_item_data["url_image"] = $this->getImageUrl($raw_item_data["header_photo"], $size, $add_host);
        }
        $raw_item_data["date_display"] = $this->formatDate($raw_item_data["date"]);
        return $raw_item_data;
    }

    function formatDate($raw_date)
    {
        $formated_date = date('d.m.Y', mysqldate2phpdate($raw_date));
        return $formated_date;
    }

    function getImageUrl($raw_file_name, $size="size1", $add_host = false)
    {
        $url = "";
        if(!empty($raw_file_name)){
            $url = "/images/news/header_photos/{$size}/{$raw_file_name}";
            if($add_host == true){
                $url = "http://{$_SERVER["HTTP_HOST"]}{$url}";
            }
        }
        return $url;
    }

    function getFullUrl(&$raw_data, $add_host = false)
    {
        $url = "";
        if(!empty($raw_data)){
            $url = "/news/item.html?id={$raw_data["id"]}";
            if($add_host == true){
                $url = "http://{$_SERVER["HTTP_HOST"]}{$url}";
            }
        }
        return $url;
    }

    public function getNewsImageSize()
    {
        return $this->newsImageSize;
    }

    public function setNewsImageSize($newsImageSize)
    {
        $this->newsImageSize = $newsImageSize;
    }

    function getLatestNews($limit = 5)
    {
        $sql = "
            SELECT id, name, date
            FROM `".self::TABLE_NEWS."`
            WHERE
                `enabled` = 1
                AND date < NOW()
            ORDER BY
                date DESC
            LIMIT $limit
        ";
        $res = $this->DB->RunSql($sql, true);
        $this->setNewsImageSize("size1");
        $this->prepareDisplayData($res);
        return $res;
    }

    function getEventInfoForsocialNetworksShare($news_info_full)
    {
        //pa($event_info_full);exit;
        $title = urlencode($news_info_full["name"]);
        $title_js_escaped = htmlspecialchars($news_info_full["name"],  ENT_QUOTES); // vkontakte
        $link = $news_info_full["url_full"];
        $res =
            array(
                'title' => $title,
                'title_js_escaped' => $title_js_escaped,
                'link'  => $link
            );
        return $res;
    }
}

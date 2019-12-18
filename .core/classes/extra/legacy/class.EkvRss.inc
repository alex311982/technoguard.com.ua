<?php
/**
 * User: sunsey
 * Date: 3 ���� 2011
 */

class EkvRss
{
    protected $DB;
    protected $LIMIT_RAW_DATA = 20;
    function __construct(DB $db = null, $LIMIT_RAW_DATA = 20)
    {
        if(is_null($db) == false){
            $this->initDB($db);
        }
        else if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }

        $this->LIMIT_RAW_DATA = $LIMIT_RAW_DATA;
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }

    function prepareDisplayDataForRss(&$partially_prepared_data)
    {
//        $RSS_DOMAIN = "http://".SITE_HOST;
//        foreach($partially_prepared_data as &$val){
//            $val["rss"]["name"] = self::replaceUnsafeChars($val["name"]);
//            $val["rss"]["descr"] = self::replaceUnsafeChars($val["body_short"]);
//            $val["rss"]["url_item_preview_full"] = "{$RSS_DOMAIN}{$val["url_full"]}";
//            if(!empty($val["photo_file"])) {
//                $img_url_http = "{$RSS_DOMAIN}{$val["url_image"]}";
//                $img_url_disk = PATH_ROOT."{$val["url_image"]}";
//                $ln = filesize($img_url_disk);
//                $mime_type = GetMimeTypeByExtension($img_url_disk);
//                $val["rss"]["image"] =
//                    array(
//                        "url_image_http" => $img_url_http,
//                        "length"    => $ln,
//                        "mime_type" => $mime_type
//                    );
//            }
//        }
    }
    function getRawRssData()
    {}

    function getPreparedRssData()
    {
        
    }

    static function prepareRssDate($mysql_date)
    {
        $php_date = EkvDateHelper::mysqlDate2PhpDate($mysql_date);
        $formated_date = date("r", $php_date);
        return $formated_date;
    }

    static function replaceUnsafeChars($txt)
    {
		//    $txt = str_replace('&laquo;'," ",$txt);
		//    $txt = str_replace('&raquo;'," ",$txt);
//		$txt = preg_replace("#&.{2,6};#", "", $txt);
//		$txt = str_replace('&',"&amp;",$txt);
//		$txt = str_replace("<","&lt;",$txt);
//		$txt = str_replace(">","&gt;",$txt);
//		$txt = str_replace("'","&apos;",$txt);
//		$txt = str_replace('"',"&quot;",$txt);
//		$txt = str_replace('�',"",$txt);
//		$txt = str_replace('�',"",$txt);
        
        $txt = strip_tags($txt);
        $txt = stripslashes($txt);
        $txt = str_replace('&',"&amp;",$txt);
        $txt = str_replace("<","&lt;",$txt);
        $txt = str_replace(">","&gt;",$txt);
        $txt = str_replace("'","&apos;",$txt);
        $txt = str_replace('"',"&quot;",$txt);
		return $txt;
	}
}

//#------------------- EkvRssEvents -------------------#//
class EkvRssEvents extends EkvRss
{
    function getRawRssData()
    {
        $sql = "
            SELECT id, room_id, rubric_id, title, header_photo, date,
                 passed, html_keywords, description_base, event_text_id
            FROM `".EkvRoomHelper::TABLE_EVENT."` e
            WHERE
                e.enabled = 1
                AND e.passed = 0
                -- AND e.date >= NOW()
            ORDER BY
                e.date ASC
            LIMIT {$this->LIMIT_RAW_DATA}
        ";
        $res = $this->DB->RunSql($sql, true);
        if(!empty($res)){
            $eventObj = new EkvRoomEvent();
            $eventObj->prepareDisplayData($res);
        }
        return $res;
    }

    function prepareDisplayDataForRss(&$partially_prepared_data)
    {
        $RSS_DOMAIN = "http://".SITE_HOST;
        foreach($partially_prepared_data as &$val){
            $val["rss"]["name"] = self::replaceUnsafeChars($val["title"]);
            $val["rss"]["descr"] = self::replaceUnsafeChars($val["description_base"]);

            //$val["rss"]["name"] = $val["title"];
            //$val["rss"]["descr"] = $val["description_base"];
            $val["rss"]["date_display"] = self::prepareRssDate($val["date"]);
            $val["rss"]["url_item_preview_full"] = "{$RSS_DOMAIN}{$val["url_full"]}";
            if(!empty($val["header_photo"])) {
                $img_url_http = "{$RSS_DOMAIN}{$val["url_image"]}";
                $img_url_disk = PATH_ROOT."{$val["url_image"]}";
                $ln = @filesize($img_url_disk);
                $mime_type = GetMimeTypeByExtension($img_url_disk);
                $val["rss"]["image"] =
                    array(
                        "url_image_http" => $img_url_http,
                        "length"    => $ln,
                        "mime_type" => $mime_type
                    );
            }
        }
    }

    function getPreparedRssData()
    {
        $raw_data = $this->getRawRssData();
        $this->prepareDisplayDataForRss($raw_data);
        return $raw_data;
    }
}

//#------------------- EkvRssNews -------------------#//
class EkvRssNews extends EkvRss
{
    function getRawRssData()
    {
        $sql = "
            SELECT id, name, header_photo, date, text
            FROM `".EkvNews::TABLE_NEWS."`
            WHERE
                `enabled` = 1
                and date < NOW()
            ORDER BY
                date DESC
            LIMIT {$this->LIMIT_RAW_DATA}
        ";

        $res = $this->DB->RunSql($sql, true);
        $newsObj = new EkvNews($this->DB, false);
        $newsObj->setNewsImageSize("size2");
        $newsObj->prepareDisplayData($res);
        return $res;
    }

    function prepareDisplayDataForRss(&$partially_prepared_data)
    {
        $RSS_DOMAIN = "http://".SITE_HOST;
        foreach($partially_prepared_data as &$val){
            $val["rss"]["name"] = self::replaceUnsafeChars($val["name"]);
            $descr = self::replaceUnsafeChars($val["text"]);
            $val["rss"]["descr"] = $descr;

            $val["rss"]["date_display"] = self::prepareRssDate($val["date"]);
            $val["rss"]["url_item_preview_full"] = "{$RSS_DOMAIN}{$val["url_full"]}";
            if(!empty($val["header_photo"])) {
                $img_url_http = "{$RSS_DOMAIN}{$val["url_image"]}";
                $img_url_disk = PATH_ROOT."{$val["url_image"]}";
                $ln = @filesize($img_url_disk);
                $mime_type = GetMimeTypeByExtension($img_url_disk);
                $val["rss"]["image"] =
                    array(
                        "url_image_http" => $img_url_http,
                        "length"    => $ln,
                        "mime_type" => $mime_type
                    );
            }
        }
    }

    function getPreparedRssData()
    {
        $raw_data = $this->getRawRssData();
        $this->prepareDisplayDataForRss($raw_data);
        return $raw_data;
    }
}

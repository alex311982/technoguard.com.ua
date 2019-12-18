<?php
/**
 * User: sunsey
 * Date: 02.12.2010
 */
 
class EkvImageGallery
{
    const TABLE = "base_galleries";
    const TABLE_PHOTOS = "base_gallery_photos";
    
 	static public $DB = null;
    static private $shortenFields = "id, `title` as `name`, photo, date";
    static function getShortenFields()
    {
        return self::$shortenFields;
    }
 	static function initDb(&$db)
 	{
		self::$DB = $db;
	}

    static function prepareDisplayData(&$raw_data)
    {
        if(!empty($raw_data)){
            foreach($raw_data as &$val){
                self::prepareDisplaySingleItem($val);
            }
        }
    }

    static function prepareDisplaySingleItem(&$raw_item_data)
    {
        //$raw_item_data["url_image"] = self::getHeaderImageUrl($raw_item_data["header_photo"], "size1");
        $raw_item_data["date_display"] = self::formatDate($raw_item_data["date"]);
        $raw_item_data["url_full"] = self::getFullUrl($raw_item_data);
    }
    
    static function getHeaderImageUrl($raw_file_name, $size="size1", $add_host=false)
    {
        $url = "";
        if(!empty($raw_file_name)){
            $galleryHeaderPhotoTmp = imgResizeSettings::getSettingsItem("galleryHeaderPhoto");
            

            $url = "{$galleryHeaderPhotoTmp["base_photo_path_relative"]}{$size}/{$raw_file_name}";
            if($add_host){
                $url = "http://{$_SERVER['HTTP_HOST']}{$url}";
            }
        }
        return $url;
    }
    
    static function getFullUrl(&$raw_data)
    {
        $url = "";
        if(!empty($raw_data)){
            $url = "/image-gallery/item.html?id={$raw_data["id"]}";
        }
        return $url;
    }

    static function formatDate($raw_date)
    {
        $formated_date = "";
        if(!empty($raw_date)){
            $formated_date = date("m.d.y H:i:s", mysqldate2phpdate($raw_date));
        }
        return $formated_date;
    }

    static function getLatestGalleries($limit = 10)
    {
        $sql = "
            SELECT ".self::$shortenFields."
            FROM `".self::TABLE."`
            WHERE
                1=1
            ORDER BY
                `date` DESC
            LIMIT 0, $limit
        ";
        $res = self::$DB->RunSql($sql, true);
        self::prepareDisplayData($res);
        return $res;
    }

    static function getGalleryPhotosForCarousel($id_gallery)
    {
        $sql = "
            SELECT *
            FROM `".self::TABLE_PHOTOS."`
            WHERE
                `id_gallery` = '$id_gallery'
            ORDER BY
                `date` DESC
        ";
        $res = self::$DB->RunSql($sql, true);
        foreach($res as &$val){
            $val["url_image_ar"] = self::prepareAllSizeRelativeUrls($val["photo_file"]);
        }
        return $res;
    }

    static function prepareAllSizeRelativeUrls($fname)
    {
        $resize_data = imgResizeSettings::getSettingsItem("galleryPhotos");
        $sizes = array();
        foreach($resize_data["size_settings"] as $val){
            $key = "{$val["size_dir"]}";
            $sizes[$key] = "{$resize_data["base_photo_path_relative"]}{$val["size_dir"]}/{$fname}"; 
        }
        return $sizes;
    }

    
}

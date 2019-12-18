<?php
/**
 *****************
 * City versions
 *****************
 */

class EkvCity
{
    const EVENTS_LIST_BASE_URL = "/events/list.html";
    const TABLE_CITY_BANK = "ekv_city_bank";
    private $DB;
    function __construct(DB $db = null)
    {
        if(is_null($db) == false){
            $this->initDB($db);
        }
        else if( is_null($this->DB) ){
            $this->initDB(EkvCommonHelper::getDB());
        }
    }
    function initDB(DB $db)
    {
        $this->DB = $db;
    }

    function getCityOptions()
    {
        $sql = "
            SELECT id_city, city_name, enabled
            FROM `".EkvRoomHelper::TABLE_VERSION_CITY."`
            WHERE
                `enabled` = 1
            ORDER BY
                `city_name` ASC
        ";
        $this->DB->query($sql);
        $res = array();
        while($row = $this->DB->fetch_row()){
            $res[$row["id_city"]] = "{$row["city_name"]}";
        }
        return $res;
    }

    function getCitiesList($visible_only = true)
    {
        $SQL_VISIBLE = "";
        if($visible_only){
            $SQL_VISIBLE = " AND `is_visible` = 1 ";    
        }

        $sql = "
            SELECT id_city, city_name, city_text_id, enabled
            FROM `".EkvRoomHelper::TABLE_VERSION_CITY."`
            WHERE
                `enabled` = 1
                {$SQL_VISIBLE}
            ORDER BY
                `sort_order` ASC
        ";
        //echo "\n <pre> sql = $sql </pre> <br/>\n";
        $res = $this->DB->RunSql($sql, true);
        return $res;
    }

    static function getCurrentCityTextIdParam()
    {
        $city = "";
        if(isset($_REQUEST["city"])){
            $city = trim($_REQUEST["city"]);
        }
        return $city;
    }
    function getCityInfoByTextId($text_id)
    {
        $text_id_safe = mysql_real_escape_string($text_id);
        $sql = "
            SELECT id_city, city_name
            FROM `".EkvRoomHelper::TABLE_VERSION_CITY."`
            WHERE
                `city_text_id` = '{$text_id_safe}'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }
    function getCityInfoByIntId($int_id, $fields  = null)
    {
        $FIELDS_SQL = "id_city, city_name, city_text_id";
        if(is_null($fields) == false){
            $FIELDS_SQL = $fields;    
        }
        
        $sql = "
            SELECT {$FIELDS_SQL}
            FROM `".EkvRoomHelper::TABLE_VERSION_CITY."`
            WHERE
                `id_city` = '{$int_id}'
        ";
        $res = $this->DB->RunSql($sql);
        return $res;
    }
    function getCityIntIdByTextId($text_id)
    {
        $int_id = 0;
        $city_info = $this->getCityInfoByTextId($text_id);
        if(!empty($city_info)){
            $int_id = $city_info["id_city"];
        }
        return $int_id;
    }

    /**
     * get versions-city filter prepared data
     * @param string $page_type (common | outdated | specific_date)
     * @return array
     */
    function getEventCityFilterOptions($page_type = "common")
    {
        $DELIMITER_CITY = "?";
        $CSS_ACTIVE_CLASS = " selected ";
        $CSS_INACTIVE_CLASS = " not-selected ";

        //--- city param ---//
        $city_current = "";
        if(EkvSiteVersion::isVersionNeutral() == false){
            $city_current = EkvSiteVersion::getVersionTextId();
        }
        $ACTIVE_ITEM_FOUND = false;

        //$current_base_page = self::EVENTS_LIST_BASE_URL;
        $current_base_page = "/";

        $cities = $this->getCitiesList();

        /*
        if($page_type == "outdated"){
            $current_base_page .= "?view_type=outdated";
            $DELIMITER_CITY = "&";
        }
        else if($page_type == "specific_date"){
            $current_base_page .= @"?date={$_REQUEST["date"]}";
            $DELIMITER_CITY = "&";
        }
        //pa($page_type);
        */

        foreach($cities as &$cval){
            //$url_current_city = $current_base_page."{$DELIMITER_CITY}city={$cval["city_text_id"]}";
            $url_current_city = EkvSiteVersion::appendUrlWithDomain($cval["city_text_id"], $current_base_page);
            $cval["url_filter"] = $url_current_city;
            $is_active = false;
            $active_class = $CSS_INACTIVE_CLASS;

            if($city_current == $cval["city_text_id"]){
                $ACTIVE_ITEM_FOUND = true;
                $is_active = true;
                $active_class = $CSS_ACTIVE_CLASS;
            }
            $cval["is_active"] = $is_active;
            $cval["active_class"] = $active_class;
        }

        $no_filter_item = array();
        $no_filter_item[''] = array(
                "city_name" => "Все города",
                //"url_filter" => $current_base_page,
                "url_filter" => EkvSiteVersion::appendUrlWithDomain("", $current_base_page),            
                "is_active" => ($ACTIVE_ITEM_FOUND == false)? true : false,
                "active_class" => ($ACTIVE_ITEM_FOUND == false)? $CSS_ACTIVE_CLASS : $CSS_INACTIVE_CLASS,
            );
        $final_res = $no_filter_item + $cities;
        //pa($final_res);
        //Entity::prependOptionsWithDefaultValue()
        return $final_res;
    }

    static function getEventsCityUrl($text_id)
    {
        //$url = self::EVENTS_LIST_BASE_URL . "?city=$text_id";
        $url = self::EVENTS_LIST_BASE_URL;
        $url = "/";
        $url = EkvSiteVersion::appendUrlWithDomain($text_id, $url);

        return $url;
    }

//  NOW bank requisites are taken by fop    
//    /**
//     * prepares data to display in BANK-RECEIPT page
//     * @param  $city_id
//     * @return array|mixed
//     */
//    function getBankInfoByCityId($city_id)
//    {
//        /* PARTIALLY DEPRECATED */
//        $res = array();
//        $sql = "
//            SELECT *
//            FROM `".self::TABLE_CITY_BANK."` cb
//                INNER JOIN `".EkvRoomHelper::TABLE_VERSION_CITY."` vc
//                    ON cb.city_id = vc.id_city
//            WHERE
//                vc.id_city = $city_id
//                OR vc.city_text_id = 'kiev'
//        ";
//        $this->DB->query($sql);
//        while($row = $this->DB->fetch_row()){
//            $res[$row["id_city"]] = $row;
//        }
//
//        $final_res = array();
//        if(array_key_exists($city_id, $res)){
//            $final_res = $res[$city_id];
//        }
//        //--- if no bank for city - display kyiv bank ---//
//        else{
//            $final_res = array_shift($res);
//        }
//
//        $final_res["display"]["code"] = str_split($final_res["code"]);
//        $final_res["display"]["account"] = str_split($final_res["account"]);
//        $final_res["display"]["mfo"] = str_split($final_res["mfo"]);
//        return $final_res;
//    }
}

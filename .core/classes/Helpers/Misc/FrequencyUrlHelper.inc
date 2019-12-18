<?php
namespace Ekv\Helpers\Misc;

use EkvGlobalHelper;

class FrequencyUrlHelper
{
    private $DB;

    function __construct()
    {
        $this->DB = \DB::getInstance();
    }

    function getPopupFrequencyRateUrls($channelID)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_CHANNEL_ONLINE_URLS."`
            WHERE
                `channel_id` = '{$channelID}'
                AND enabled_at_popup = 1
                AND enabled = 1
            ORDER BY
                rate DESC
        ";
        $rates = $this->commonMethod($sql);

        if(isset($rates["list"]["mp3"]["items"][0])){

            $rates["active"] = $rates["list"]["mp3"]["items"][0];
        }

        return $rates;
    }

    private function commonMethod($sql)
    {
        $raw = $this->DB->RunSql($sql, true);

        $res = array(
            'aac' => array(
                'name' => "AAC+",
                'items' => array()
            ),
            'mp3' => array(
                'name' => "MP3",
                'items' => array()
            ),
            'ogg' => array(
                'name' => "OGG",
                'items' => array()
            )
        );

        foreach($raw as $rval){
            $res[$rval["format"]]["items"][] = $rval;
        }

        //--- remove empty ---//
        foreach($res as $resKey => $resVal){
            if(empty($resVal["items"])){
                unset($res[$resKey]);
            }
        }

        $final_res = array(
            'list' => $res,
            "active" => ''
            //'active' =>  isset($res["mp3"]["items"][0]) ? $res["mp3"]["items"][0] : ""
            //'active' =>  $res["ogg"]["items"][0]
        );

        return $final_res;
    }

    function getMobileFrequencyRateUrls($channelID)
    {
        $sql = "
            SELECT *
            FROM `".EkvGlobalHelper::TABLE_CHANNEL_ONLINE_URLS."`
            WHERE
                `channel_id` = '{$channelID}'
                AND enabled_at_mobile = 1
                AND enabled = 1
            ORDER BY
                rate ASC
        ";

        $rates = $this->commonMethod($sql);
        //pa($rates);

        return $rates;

    }
}
 
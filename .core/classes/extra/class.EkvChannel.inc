<?php

class EkvChannel
{
    private $DB;
    const DEFAULT_CHANNEL_ID = 1;

    function __construct()
    {
        $this->DB = DB::getInstance();
    }

    static function getChannelInfo($channel_id)
    {
        $channel_id = EkvCommonHelper::getIntOrZero($channel_id);

        $sql = "
            SELECT *,  ch_name_".ekvLangCode()." as ch_name,  ch_name_".ekvLangCode()." as ch_name_ext
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                `id_channel` = '{$channel_id}'
        ";
        $res = DB::getInstance()->RunSql($sql);

        return $res;
    }

    function getChannelOptions()
    {
        $sql = "
            SELECT id_channel, ch_name_".ekvLangCode()." as ch_name,  ch_name_".ekvLangCode()." as ch_name_ext
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                ch_enabled = 1
        ";
        $options = EkvCommonHelper::createOptionsSimple($sql, 'id_channel', 'ch_name');

        return $options;
    }

    function getChannels($addFields = '')
    {
        if (!empty($addFields)) {
            $addFields = ", {$addFields}";
        }

//            SELECT id_channel, ch_name_".ekvLangCode()." as ch_name, ch_name_ext {$addFields}

//echo($addFields);

        $sql = "
            SELECT id_channel, ch_name_".ekvLangCode()." as ch_name, ch_name_".ekvLangCode()." as ch_name_ext {$addFields}
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                ch_enabled = 1
            ORDER BY
                id_channel ASC
        ";
        
        $res = $this->DB->RunSql($sql, true, 'id_channel');
        return $res;
    }

    function getChannelsForMenu()
    {
//            SELECT id_channel, ch_name_".ekvLangCode()." as ch_name, ch_name_ext

        $sql = "
            SELECT id_channel, ch_name_".ekvLangCode()." as ch_name, ch_name_".ekvLangCode()." as ch_name_ext 
            FROM `".EkvGlobalHelper::TABLE_CHANNEL."`
            WHERE
                ch_enabled = 1
            ORDER BY
                id_channel ASC
        ";
        $res = $this->DB->RunSql($sql, true, 'id_channel');
        return $res;
    }

    /**
     * @deprecated
     * @param $channel_id
     * @param null $period_id
     * @return string
     */
    static function getChannelScheduleUrl($channel_id, $period_id = null)
    {
        $period_str = $period_id > 0 ? "&periodID={$period_id}" : "";
        $url = "/grid/channel/period/items/list.html?channelID={$channel_id}{$period_str}";
        return $url;
    }

    function getChannelsForArchiveTabs($calendar_settings = null)
    {
        $selected = array();
        $channels = $this->getChannels();
        foreach($channels as &$cval){
            $cval["tab_name"] = "archiveTab-{$cval["id_channel"]}";
            $cval["li_tab_id"] = "liArchiveTab-{$cval["id_channel"]}";
            if(!is_null($calendar_settings)){
                if($cval["id_channel"] == $calendar_settings["channel_id"]){
                    $cval["selected"] = true;
                    $cval["period_date_selected"] = $calendar_settings["period_date"];

                    $selected = array(
                        'li_tab_id' => $cval["li_tab_id"]
                    );
                }
            }
        }

        //pa($res);exit;
        $res = array(
            'channels' => $channels,
            'selected' => $selected
        );
        return $res;
    }

    function getLogoUrl($logo_fname)
    {
        return "/images/channel/logo/{$logo_fname}";
    }

    function getOnlinePopupUrl($channel_id)
    {
        return "/".ekvLangCode()."/schedule/play-live.html?channelID={$channel_id}";
    }

    function isChannelIdCorrect($channel_id)
    {
        $channel_info = $this->getChannelInfo($channel_id);
        return !empty($channel_info) && $channel_info["ch_enabled"];
    }

    function getChannelUrl($channel_id)
    {
        return "/channel.html?channelID={$channel_id}";
    }
}

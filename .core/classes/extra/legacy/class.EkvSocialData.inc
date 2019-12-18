<?php

class EkvSocialData
{
    /**
     * for Event Preview Page
     * @static
     * @param $raw_res
     * @param Entity|null $entity
     * @return array
     */
    static function getSocialSharableInfo_EventPreviewPage($raw_res, Entity $entity = null)
    {
        $soc_data = array(
            "title" => $raw_res["title"],
            "url_full" => $raw_res["url_full"],
            "url_image_social" => $raw_res["url_image"]
        );
        return self::getSocialSharableInfo($soc_data, $entity);
    }

    /**
     * Method for News Preview Page
     * @static
     * @param $raw_res
     * @param Entity|null $entity
     * @return array
     */
    static function getSocialSharableInfo_NewsPage($raw_res, Entity $entity = null)
    {
        $soc_data = array(
            "title" => $raw_res["name"],
            "url_full" => $raw_res["url_full"],
            "url_image_social" => $raw_res["url_image"]
        );
        return self::getSocialSharableInfo($soc_data, $entity);
    }

    /**
     * method for Room Preview page
     * @static
     * @param $raw_res
     * @param Entity|null $entity
     * @return array
     */
    static function getSocialSharableInfo_RoomPreviewPage($raw_res, Entity $entity = null)
    {
        $soc_data = array(
            "title" => $raw_res["title"],
            "url_full" => EkvCommonHelper::appendUrlWithSiteHostConf($raw_res["url_full"]),
            "url_image_social" => EkvCommonHelper::appendUrlWithSiteHostConf($raw_res["url_header_photo"])
        );
        return self::getSocialSharableInfo($soc_data, $entity);
    }

    /**
     * common method to assign proper smarty vars to display likes
     * and returns prepared to script
     * @static
     * @param $raw_res
     * @param Entity|null $entity - if passed - correct smarty vars are assigned automatically
     * @return array
     */
    static function getSocialSharableInfo($raw_res, Entity $entity = null)
    {
        $title = urlencode($raw_res["title"]);
        $title_js_escaped = htmlspecialchars($raw_res["title"],  ENT_QUOTES); // vkontakte
        $link = $raw_res["url_full"];

        $url_image_social = isset($raw_res["url_image_social"]) ? $raw_res["url_image_social"] : null;

        $social_networks_data =
            array(
                'title' => $title,
                'title_js_escaped' => $title_js_escaped,
                'link'  => $link,
                "url_image" => $url_image_social
            );

        if(!is_null($entity)){
            $entity->TML->assign("social_networks_data", $social_networks_data );
            $entity->TML->assign("FB_META_IMAGE", $url_image_social );
        }

        //pa($social_networks_data);
        return $social_networks_data;
    }
}

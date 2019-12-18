<?php
namespace Ekv\TContent\Banner\Front;

use Ekv\TContent\Banner\BannerFormatter;
use Ekv\TContent\Banner\BannerRoot;

class BannerFooterMicro
{
    private $DB;
    private $langID;
    private $limit;

    function __construct($langID, $limit = 6)
    {
        $this->DB = \DB::getInstance();
        $this->langID = @intval($langID);
        $this->limit = $limit;
    }

    public function mainBanners()
    {
        $sql = "
            SELECT id_banner, lang_id, ban_type, ban_name,
                ban_url, ban_photo, ban_date_created, ban_enabled
            FROM `".\EkvGlobalHelper::TABLE_BANNER."`
            WHERE
                ban_enabled = 1
                AND lang_id = '{$this->langID}'
                AND ban_type = '".BannerRoot::TYPE_FOOTER_MICRO."'
            ORDER BY
                sort_order ".BannerRoot::SORT_ORDER_DIRECTION."
            LIMIT {$this->limit}
        ";
        $res = $this->DB->RunSql($sql, true);

        if (!empty($res)) {
            $banFormatter = new BannerFormatter();
            $res = $banFormatter->prepareMultipleItems($res);
        }

        return $res;
    }
}

<?php
namespace Ekv\TContent\Banner\Front;

use Ekv\TContent\Banner\BannerFormatter;

class SingleBanner
{
    private $DB;
    private $langID;
    private $bannerType;
    private $exceptIds;

    function __construct($langID, $bannerType, $exceptIds = array())
    {
        $this->DB = \DB::getInstance();
        $this->langID = @intval($langID);
        $this->bannerType = @intval($bannerType);
        $this->exceptIds = $exceptIds;
    }

    /**
     * ORDER BY RAND() is very unefficient! Will optimize in future.
     * @return array
     */
    public function mainGetBanner()
    {
        $exceptIDsSql = "";
        if (!empty($this->exceptIds) && is_array($this->exceptIds)) {
            $exceptIDsSql = " AND id_banner NOT IN(".\EkvCommonHelper::implodeForSqlInCondition($this->exceptIds).")";
        }

        $sql = "
            SELECT id_banner, lang_id, ban_type, ban_name,
                ban_url, ban_photo, ban_date_created, ban_enabled
            FROM `".\EkvGlobalHelper::TABLE_BANNER."`
            WHERE
                ban_enabled = 1
                AND lang_id = '{$this->langID}'
                AND ban_type = '{$this->bannerType}'
                {$exceptIDsSql}
            ORDER BY
                RAND()
            LIMIT 1
        ";
        $res = $this->DB->RunSql($sql);

        if (!empty($res)) {
            $banFormatter = new BannerFormatter();
            $res = $banFormatter->prepareSingleItem($res);
        }
        return $res;

    }
}

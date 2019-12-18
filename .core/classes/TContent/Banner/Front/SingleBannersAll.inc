<?php
namespace Ekv\TContent\Banner\Front;

use Ekv\TContent\Banner\BannerRoot;

class SingleBannersAll
{
    private $DB;
    private $langID;
    private $exceptIDs;

    function __construct($langID, $exceptIds = null)
    {
        $this->DB = \DB::getInstance();
        $this->langID = @intval($langID);

        if (empty($exceptIds)) {
            $this->exceptIDs = $this->getBannerViewedCookie();
        }
    }

    public function getAllSingleBanners()
    {
        $singleTypes = array(
            BannerRoot::TYPE_MAIN_TOP => 'main_top',
            BannerRoot::TYPE_MAIN_MIDDLE => 'main_middle',
            BannerRoot::TYPE_SIDE_TOP => 'side_top',
            BannerRoot::TYPE_SIDE_BOTTOM => 'side_bottom',
        );

        $bannersList = array();
        $idsCollected = array();

        foreach ($singleTypes as $banType => $banSymKey) {
            $singleBanner = new SingleBanner($this->langID, $banType, $this->exceptIDs);
            $banner = $singleBanner->mainGetBanner();

            /*
             * Try get banner without excpetIds.
             * Fixes the case when there is only one banner and it is among exceptIds.
             * Show this single banner each time.
             */
            if (empty($banner)) {
                $singleBanner = new SingleBanner($this->langID, $banType);
                $banner = $singleBanner->mainGetBanner();
            }

            $bannersList[$banSymKey] = $banner;
            if (!empty($banner)) {
                $idsCollected[] = $banner["id_banner"];
            }

        }

        $this->saveBannerViewedCookie($idsCollected);

        return $bannersList;
    }

    private function saveBannerViewedCookie($bannerIds)
    {
        setcookie('ekvBannerIdsViewed', json_encode($bannerIds), time()+3600*24*1, '/', COOKIE_DOMAIN);
    }

    private function getBannerViewedCookie()
    {
        $bannerIds = array();
        if (isset($_COOKIE["ekvBannerIdsViewed"])) {
            $tmp = json_decode($_COOKIE["ekvBannerIdsViewed"], true);
            if (is_array($tmp)) {
                $bannerIds = $tmp;
            }
        }
        return $bannerIds;
    }
}

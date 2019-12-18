<?php

use Ekv\F\Index\Category\MChannelsEtherNow;
use Ekv\F\ScheduleMod\Bank\BankProgRelatedNetItems;
use Ekv\Helpers\System\Lang;
use Ekv\Misc\SkinRoot;
use Ekv\News\NewsSample;
use Ekv\News\NewsSideNrcu;
use Ekv\System\MenuFront\MenuFront;
use Ekv\System\MenuFront\MenuFrontBase;
use Ekv\TContent\Banner\Front\BannerFooterMicro;
use Ekv\TContent\Banner\Front\SingleBannersAll;

/**
 * @param $params
 * @param SmartyClass $smarty
 */
function smarty_function_assign_global_data($params, $smarty)
{

    global $DB, $langtext, $url_parts;
    $assign = array();
    $langID = ekvLangID();

    //--- langs (for switcher) ---//
    $langs = Lang::getInstance()->getLangsForTpl();
    $langs_ = Lang::getInstance()->getLangInfoCurrent();
    if ($langs_['lang_code'] != 'uk') $lurl = '/'.$langs_['lang_code']; else $lurl = '';

    //pa($langs);
    $smarty->assign("__langs", $langs);

    //--- channels for menu ---//
    $chanObj = new EkvChannel();
    $chan_menu = $chanObj->getChannelsForMenu();
    $smarty->assign("__chan_menu", $chan_menu);

    //--- archive channels  ---//
    $calendar_settings = $smarty->getTemplateVars('calendar_settings');

    $archive_channels_raw = $chanObj->getChannelsForArchiveTabs($calendar_settings);
    //pa($archive_channels_raw);exit;
    $smarty->assign("__archive_channels", $archive_channels_raw["channels"]);
    $smarty->assign("__archive_channel_selected", $archive_channels_raw["selected"]);

    //--- ether now ---//
    $etherNowObj = new MChannelsEtherNow();
    $ether_now_items = $etherNowObj->mainGetPeriodItemsPlayingNow();

    $piRelatedObj = new BankProgRelatedNetItems(289);
    $progs_next = $piRelatedObj->getLatestPassedProgs(1);


    foreach ($progs_next as $key => $item) {
        if (empty($item['audio_file_auto'])) {
            unset($progs_next[$key]);
        }
    }

    $progs_next[0]['channel_name'] = 'Новини';
    //pa($ether_now_items);
    $smarty->assign("__ether_now_items", $ether_now_items);
    $smarty->assign("news_now_item", $progs_next[0]);

    $page = $smarty->getTemplateVars('PAGE');
    $newsLimit = 5;
    if($page == 'news_preview'){
        $newsLimit = 10;
    }

    if (str_replace($lurl, '', $url_parts['path']) == '/news/list.html' AND isset($_GET['date'])) {

        $smarty->assign("__archive_channel_selected", array('li_tab_id' => 'liArchiveTab-5'));
        $smarty->assign("__date_news", $_GET['date']);
    }
//    print_r($smarty->getTemplateVars('PAGE'));
//    die();


    //--- latest news ---//
//    $newsObj = new NewsSample();
//    $news_side_latest = $newsObj->getLatestNews(ekvLangID(), $news_limit);
    $nrcuNewsObj = new NewsSideNrcu($langID);
    $newsSideNrcu = $nrcuNewsObj->mainNews($newsLimit);
    $smarty->assign('newsSideNrcu', $newsSideNrcu);

    $smarty->assign('newsSideNrcuUrlAll', $nrcuNewsObj->getAllNewsUrl());
    
    // -- quotes last ---//
    $smarty->assign('quotesLast', $nrcuNewsObj->mainQuotes());


    //--- menu ---//
    $menuFrontObj = new MenuFront();
    $menuFrontObj->initPrams(MenuFrontBase::MENU_TYPE_TOP, $langID);
    $menuTop = $menuFrontObj->getFullMenu();

    $menuMiddleObj = new MenuFront();
    $menuMiddleObj->initPrams(MenuFrontBase::MENU_TYPE_MIDDLE, $langID);
    $menuMiddle = $menuMiddleObj->getFullMenu();

    //--- define skin ---//
    $skinObj = new SkinRoot();
    $skinTypeSym = $skinObj->calculateFrontCurrentSkinType();

    //--- banners single ---//
    $bannersSingleAllObj = new SingleBannersAll($langID);
    $bannersSingleAll = $bannersSingleAllObj->getAllSingleBanners();

    //--- banners footer ---//
    $bannersFooterObj = new BannerFooterMicro($langID, 6);
    $bannersFooter = $bannersFooterObj->mainBanners();

    $sql = "SELECT * FROM `base_lang`";
    $smarty->assign("lurl", $lurl);
    $langtext_ = $DB->RetrieveRecordsSqlOptimized($sql, false, false);
    $langtext = array();
    $l_ = Lang::getInstance()->getLangCode();
    foreach($langtext_['records'] as $item) {

        $langtext[$item['key']] = $item[$l_];
    }

    //pa($menuMiddle);
    $smarty->assign('langText', $langtext);
    $smarty->assign('search_type', $_GET['type']);
    $smarty->assign('bannersFooter', $bannersFooter);
    $smarty->assign("bannersSingleAll", $bannersSingleAll);
    $smarty->assign('skinTypeSym', $skinTypeSym);
    $smarty->assign("__menuMiddle", $menuMiddle);
    $smarty->assign("__menuTop", $menuTop);

}

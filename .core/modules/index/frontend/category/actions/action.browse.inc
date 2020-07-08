<?php

$sql = "SELECT * FROM `banners` WHERE `enabled`=1 ORDER BY `sort_order` DESC";

$banners = $this->DB->runSql($sql,true);

$tempBanners = $resolvedBanners = [];

foreach ($banners as &$banner) {
    $banner['imagePath'] = '/images/banners/original/' . $banner['photo_file'];

    $tempBanners[$banner['code']][$banner['lang']] = $banner;
}

foreach ($tempBanners as $code => $banners) {
    if (!isset($banners[$_REQUEST['lang']])) {
        $resolvedBanners[$code] = $banners['ru'];
    } else {
        $resolvedBanners[$code] = $banners[$_REQUEST['lang']];
    }
}

$this->TML->assign('banners', $resolvedBanners);
$this->TML->assign('lang', $_REQUEST['lang']);

$sql = "SELECT * FROM `translations` WHERE `enabled`=1";

$translations = $this->DB->runSql($sql,true);

$tempTranslations = $resolvedTranslations = [];

foreach ($translations as $translation) {
    $tempTranslations[$translation['code']][$translation['lang']] = $translation['text'];
}

foreach ($tempTranslations as $code => $translations) {
    if (!isset($translations[$_REQUEST['lang']])) {
        $resolvedTranslations[$code] = $translations['ru'];
    } else {
        $resolvedTranslations[$code] = $translations[$_REQUEST['lang']];
    }
}

$this->TML->assign('translations', $resolvedTranslations);

if ($_REQUEST['lang'] == 'ru') {
    $this->TML->assign('selectedLang', 'Русский');
    $this->TML->assign('selectedLangCode', 'ru');
    $this->TML->assign('itemLang', 'Украинский');
    $this->TML->assign('itemLangCode', 'ua');
}
if ($_REQUEST['lang'] == 'ua') {
    $this->TML->assign('selectedLang', 'Украинский');
    $this->TML->assign('selectedLangCode', 'ua');
    $this->TML->assign('itemLang', 'Русский');
    $this->TML->assign('itemLangCode', 'ru');
}

$sql = "SELECT * FROM `indexblock` WHERE `enabled`=1 ORDER BY `code` ASC";

$indexblocks = $this->DB->runSql($sql,true);

$tempIndexblocks = $resolvedIndexblocks = [];

foreach ($indexblocks as &$indexblock) {
    $indexblock['imagePath'] = '/images/indexblock/original/' . $indexblock['photo_file'];

    $tempIndexblocks[$indexblock['code']][$indexblock['lang']] = $indexblock;
}

foreach ($tempIndexblocks as $code => $tempIndexblock) {
    if (!isset($tempIndexblock[$_REQUEST['lang']])) {
        $resolvedIndexblocks[$code] = $tempIndexblock['ru'];
    } else {
        $resolvedIndexblocks[$code] = $tempIndexblock[$_REQUEST['lang']];
    }
}

$this->TML->assign('indexblocks', $resolvedIndexblocks);

$this->Display('common:index/category/page.browse.tpl');
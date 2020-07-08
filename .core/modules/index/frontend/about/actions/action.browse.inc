<?php

$sql = "SELECT * FROM `abouts` WHERE enabled=1 ORDER BY `date_created` DESC";

$abouts = $this->DB->RunSql($sql, true);

$tempAbouts = []; $resolvedAbout = '';

foreach ($abouts as &$about) {
    $tempAbouts[$about['lang']] = $about;
}

if (!isset($tempAbouts[$_REQUEST['lang']])) {
    $resolvedAbout = $tempAbouts['ru'];
} else {
    $resolvedAbout = $tempAbouts[$_REQUEST['lang']];
}

$this->TML->assign('text', $resolvedAbout['text']);

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

$this->Display('common:about/category/page.browse.tpl');
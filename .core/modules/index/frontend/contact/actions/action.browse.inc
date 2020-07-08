<?php

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

$this->Display('common:contact/category/page.browse.tpl');
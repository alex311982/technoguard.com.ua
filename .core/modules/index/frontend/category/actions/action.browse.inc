<?php

$sql = "SELECT * FROM `banners` WHERE `enabled`=1 ORDER BY `sort_order` DESC";

$banners = $this->DB->runSql($sql,true);

foreach ($banners as &$item) {
    $item['imagePath'] = '/images/banners/original/' . $item['photo_file'];
}
$this->TML->assign('banners', $banners);
$this->TML->assign('lang', $_REQUEST['lang']);

$sql = "SELECT code, text FROM `translations` WHERE `enabled`=1 AND `lang`='" . DEFAULT_LANG . "'";

$translations = $this->DB->runSql($sql,true);

$resolvedTranslations = [];

foreach ($translations as $translation) {
    $resolvedTranslations[$translation['code']] = $translation['text'];
}
$this->TML->assign('translations', $resolvedTranslations);

if ($_REQUEST['lang'] == 'ru') {
    $this->TML->assign('selectedLang', 'Русский');
    $this->TML->assign('itemLang', 'Украинский');
}
if ($_REQUEST['lang'] == 'ua') {
    $this->TML->assign('selectedLang', 'Украинский');
    $this->TML->assign('itemLang', 'Русский');
}

$this->Display('common:index/category/page.browse.tpl');
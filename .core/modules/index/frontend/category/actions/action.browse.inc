<?php

$sql = "SELECT * FROM `banners` WHERE `enabled`=1 ORDER BY `sort_order` DESC";

$banners = $this->DB->runSql($sql,true);

foreach ($banners as &$item) {
    $item['imagePath'] = '/images/banners/original/' . $item['photo_file'];
}
$this->TML->assign('banners', $banners);

$sql = "SELECT code, text FROM `translations` WHERE `enabled`=1 AND `lang`='" . DEFAULT_LANG . "'";

$translations = $this->DB->runSql($sql,true);

$resolvedTranslations = [];

foreach ($translations as $translation) {
    $resolvedTranslations[$translation['code']] = $translation['text'];
}
$this->TML->assign('translations', $resolvedTranslations);

$this->Display('common:index/category/page.browse.tpl');
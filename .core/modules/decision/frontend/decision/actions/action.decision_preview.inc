<?php

$lang = $_COOKIE['lang'];

$newsId = isset($_REQUEST['id']) ? (int)$_REQUEST['id'] : 1;

$sql = "SELECT * FROM `decisions` WHERE `id` = '{$newsId}' AND enabled=1";

$item = $this->DB->RunSql($sql);

$item['imagePath'] = '/images/decisions/original/' . $item['photo_file'];

$directions = array(
    'retail' => 'Ритейл', 'agroprom' => 'Агропром', 'RFID' => 'RFID'
);

$direction_title = $directions[$item['direction_type']];

$sql = "SELECT * FROM `decisions` WHERE `direction_type` = '{$item['direction_type']}' AND enabled=1 AND `lang`= '{$lang}' AND `id` != {$newsId} ORDER BY `date_created` DESC";

$news = $this->DB->RunSql($sql, true);

$nextNewsIds = [];
$nextNewsId = null;

foreach ($news as &$news_) {
    $news_['imagePath'] = '/images/decisions/original/' . $news_['photo_file'];
    $nextNewsIds[] = (int)$news_['id'];
    $news_['id'] > $newsId ? $nextNewsId = $news_['id'] : null;
}

sort($nextNewsIds);

is_null($nextNewsId) && !empty($nextNewsIds) ? $nextNewsId = reset($nextNewsIds) : null;

$directions = array(
    'retail' => 'ритейла', 'agroprom' => 'агропрома', 'RFID' => 'RFID'
);

$sql = "SELECT * FROM `decisions` WHERE `code` = '{$item['code']}' AND enabled=1 AND `lang`= '{$lang}'";

$item = $this->DB->RunSql($sql);

$this->TML->assign('direction_title', $direction_title);
$this->TML->assign('item', $item);
$this->TML->assign('news', $news);
$this->TML->assign('direction_title2', $directions[$item['direction_type']]);
$this->TML->assign('next_news_id', $nextNewsId);
$this->TML->assign('title', $item['title']);
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

$this->Display('common:decision/decision/page.browse.tpl');
<?php

$newsId = isset($_REQUEST['id']) ? (int)$_REQUEST['id'] : 1;

$sql = "SELECT * FROM `news` WHERE `id` = '{$newsId}' AND enabled=1";

$item = $this->DB->RunSql($sql);

$item['imagePath'] = '/images/news/original/' . $item['photo_file'];

$directions = array(
    'retail' => 'Ритейл', 'agroprom' => 'Агропром', 'RFID' => 'RFID'
);

$direction_title = $directions[$item['direction_type']];

$sql = "SELECT * FROM `news` WHERE `direction_type` = '{$item['direction_type']}' AND enabled=1 AND `id` != {$newsId} ORDER BY `date_created` DESC";

$news = $this->DB->RunSql($sql, true);

$nextNewsIds = [];
$nextNewsId = null;

foreach ($news as &$news_) {
    $news_['imagePath'] = '/images/news/original/' . $news_['photo_file'];
    $nextNewsIds[] = (int)$news_['id'];
    $news_['id'] > $newsId ? $nextNewsId = $news_['id'] : null;
}

sort($nextNewsIds);

is_null($nextNewsId) ? $nextNewsId = reset($nextNewsIds) : null;

$directions = array(
    'retail' => 'ритейла', 'agroprom' => 'агропрома', 'RFID' => 'RFID'
);

$this->TML->assign('direction_title', $direction_title);
$this->TML->assign('item', $item);
$this->TML->assign('news', $news);
$this->TML->assign('direction_title2', $directions[$item['direction_type']]);
$this->TML->assign('next_news_id', $nextNewsId);
$this->TML->assign('title', $item['news_title']);
$this->TML->assign('lang', $_REQUEST['lang']);

$sql = "SELECT code, text FROM `translations` WHERE `enabled`=1 AND `lang`='" . DEFAULT_LANG . "'";

$translations = $this->DB->runSql($sql,true);

$resolvedTranslations = [];

foreach ($translations as $translation) {
    $resolvedTranslations[$translation['code']] = $translation['text'];
}
$this->TML->assign('translations', $resolvedTranslations);

$this->Display('common:news/news/page.browse.tpl');
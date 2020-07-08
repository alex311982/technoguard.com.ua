<?php

$type = isset($_REQUEST['type']) ? $_REQUEST['type'] : 'retail';

$sql = "SELECT * FROM `news` WHERE `direction_type` = '{$type}' AND enabled=1 ORDER BY `date_created` DESC";

$news = $this->DB->RunSql($sql, true);

$tempNews = $resolvedNews = [];

foreach ($news as &$new) {
    $new['imagePath'] = '/images/news/original/' . $new['photo_file'];

    $tempNews[$new['code']][$new['lang']] = $new;
}

foreach ($tempNews as $code => $new) {
    if (!isset($new[$_REQUEST['lang']])) {
        $resolvedNews[$code] = $new['ru'];
    } else {
        $resolvedNews[$code] = $new[$_REQUEST['lang']];
    }
}

$sql = "SELECT * FROM `decisions` WHERE `direction_type` = '{$type}' AND enabled=1 ORDER BY `sort_order` DESC";

$decisions = $this->DB->RunSql($sql, true);

$tempDecisions = $resolvedDecisions = [];

foreach ($decisions as &$decision) {
    $decision['imagePath'] = '/images/decisions/original/' . $decision['photo_file'];

    $tempDecisions[$decision['code']][$decision['lang']] = $decision;
}

foreach ($tempDecisions as $code => $decision) {
    if (!isset($decision[$_REQUEST['lang']])) {
        $resolvedDecisions[$code] = $decision['ru'];
    } else {
        $resolvedDecisions[$code] = $decision[$_REQUEST['lang']];
    }
}

$sql = "SELECT * FROM `directionheaders` WHERE `direction_type` = '{$type}' AND enabled=1 AND lang='{$_REQUEST['lang']}' ORDER BY `date_created` DESC";

$header = $this->DB->RunSql($sql);
$header['imagePath'] = '/images/directionheaders/original/' . $header['photo_file'];

$directions = array(
    'retail' => 'Ритейл', 'agroprom' => 'Агропром', 'RFID' => 'RFID'
);

$header['direction_title'] = $directions[$header['direction_type']];

switch ($type) {
    case 'agroprom':
        $type = 'agroprom';
        $title = 'Technoguard | Agroprom';

        break;
    case 'retail':
        $type = 'retail';
        $title = 'Technoguard | Retail';

        break;
    case 'RFID':
    case 'rfid':
        $type = 'RFID';
        $title = 'Technoguard | RFID';

        break;

    default:
        $type = 'retail';
        $title = 'Technoguard | Retail';
}

$sql = "SELECT * FROM `directionquotes` WHERE `enabled`=1 AND `direction_type` = '{$type}' ORDER BY `date_created` DESC";

$directionquotes = $this->DB->runSql($sql,true);

$tempDirectionquotes = $resolvedDirectionquotes = [];

foreach ($directionquotes as &$directionquote) {
    $tempDirectionquotes[$directionquote['code']][$directionquote['lang']] = $directionquote;
}

foreach ($tempDirectionquotes as $code => $directionquote) {
    if (!isset($directionquote[$_REQUEST['lang']])) {
        $resolvedDirectionquotes[$code] = $directionquote['ru'];
    } else {
        $resolvedDirectionquotes[$code] = $directionquote[$_REQUEST['lang']];
    }
}

$resolvedDirectionquotes1 = $resolvedDirectionquotes2 = [];

if ($resolvedDirectionquotes) {
    list($resolvedDirectionquotes1, $resolvedDirectionquotes2) =
        array_chunk($resolvedDirectionquotes, ceil(count($resolvedDirectionquotes) / 2));
}

$this->TML->assign('directionquotes1', $resolvedDirectionquotes1);
$this->TML->assign('directionquotes2', $resolvedDirectionquotes2);
$this->TML->assign('direction_type', $type);
$this->TML->assign('title', $title);
$this->TML->assign('news', $resolvedNews);
$this->TML->assign('decisions', $resolvedDecisions);
$this->TML->assign('header', $header);
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

$this->Display('common:direction/category/page.browse.tpl');
<?php

$type = isset($_REQUEST['type']) ? $_REQUEST['type'] : 'retail';

$sql = "SELECT * FROM `news` WHERE `direction_type` = '{$type}' AND enabled=1 ORDER BY `date_created` DESC";

$news = $this->DB->RunSql($sql, true);

foreach ($news as &$item) {
    $item['imagePath'] = '/images/news/original/' . $item['photo_file'];
}

$sql = "SELECT * FROM `decisions` WHERE `direction_type` = '{$type}' AND enabled=1 ORDER BY `date_created` DESC";

$decisions = $this->DB->RunSql($sql, true);

foreach ($decisions as &$decision) {
    $decision['imagePath'] = '/images/decisions/original/' . $decision['photo_file'];
}

$sql = "SELECT * FROM `directionheaders` WHERE `direction_type` = '{$type}' AND enabled=1 ORDER BY `date_created` DESC";

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
        $type = 'RFID';
        $title = 'Technoguard | RFID';

        break;

    default:
        $type = 'retail';
        $title = 'Technoguard | Retail';
}

$this->TML->assign('direction_type', $type);
$this->TML->assign('title', $title);
$this->TML->assign('news', $news);
$this->TML->assign('decisions', $decisions);
$this->TML->assign('header', $header);

$this->Display('common:direction/category/page.browse.tpl');
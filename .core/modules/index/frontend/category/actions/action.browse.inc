<?php

$sql = "SELECT * FROM `banners` WHERE `enabled`=1 ORDER BY `sort_order` DESC";

$banners = $this->DB->runSql($sql,true);

foreach ($banners as &$item) {
    $item['imagePath'] = '/images/banners/original/' . $item['photo_file'];
}
$this->TML->assign('banners', $banners);

$this->Display('common:index/category/page.browse.tpl');
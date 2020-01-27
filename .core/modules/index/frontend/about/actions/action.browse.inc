<?php

$sql = "SELECT * FROM `abouts` WHERE enabled=1 ORDER BY `date_created` DESC LIMIT 1";

$abouts = $this->DB->RunSql($sql, true);

$this->TML->assign('text', reset($abouts)['text']);

$this->Display('common:about/category/page.browse.tpl');
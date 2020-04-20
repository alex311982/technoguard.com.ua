<?php

$this->saveFilterState();

$this->DB->per_page = 20;
$this->DB->default_order = array('code' => "ASC");
$where = array();
$where_sql = "";

$result = $this->DB->RetrieveRecords("", $where, $where_sql);
$this->TML->assign('result', $result);

$this->assignFilterAction($this->action);
$this->Display('common:page.browse.tpl');

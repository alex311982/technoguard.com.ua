<?php
    //$this->DB->per_page = 1;
	$this->DB->default_order = array('key' => "ASC");
    $where = array();
    $where_sql = "";
	$result = $this->DB->RetrieveRecords("", $where, $where_sql);
	if(!empty($result['records'])){
	}

	$this->TML->assign('result', $result);
	$this->Display('common:page.browse.tpl');

?>

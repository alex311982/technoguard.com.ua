<?php
    //$this->DB->per_page = 1;
    $this->saveFilterState();
	$this->DB->default_order = array('id' => "DESC");
    $where = array();
    $where_sql = "";
	$result = $this->DB->RetrieveRecords("", $where, $where_sql);
	if(!empty($result['records'])){
	}

	$this->TML->assign('result', $result);
    $this->TML->assign("toolbar_create_disable", true );
	$this->Display('common:page.browse.tpl');

?>

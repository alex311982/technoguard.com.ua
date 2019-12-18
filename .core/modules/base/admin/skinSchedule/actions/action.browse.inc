<?php
/** @var $this Entity */

$this->DB->resetAllNotifications();

$where = array();
$this->DB->default_order = array('dt_begin' => 'DESC');
$result = $this->DB->RetrieveRecords("", $where);

if (isset($result["records"]) && !empty($result["records"])) {
    foreach ($result["records"] as &$rval) {
        $rval["name"]["href"] = "edit?id={$rval["id_row"]["value"]}";
        $rval["skin_type"]["href"] = "edit?id={$rval["id_row"]["value"]}";
    }
}

$this->TML->assign("result", $result);
$this->Display('common:page.browse.tpl');
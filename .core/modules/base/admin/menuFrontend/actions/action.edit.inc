<?php
/** @var $this BaseMenufrontend */

$result = $this->DB->RetrieveRecord($this->id);

$parent_id = $result["record"]["parent_id"]["value"];
//$this->prepareParentIdOptions($this->id, $result);

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
?>

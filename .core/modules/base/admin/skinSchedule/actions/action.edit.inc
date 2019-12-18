<?php
/** @var $this Entity */

$result = $this->DB->RetrieveRecord($this->id, $this->PK);

EkvDateHelper::fixDatePickerForEditAction($result, 'man_dt_begin', 'dt_begin');
EkvDateHelper::fixDatePickerForEditAction($result, 'man_dt_end', 'dt_end');

$this->TML->assign('result', $result);
$this->Display('common:page.edit.tpl');
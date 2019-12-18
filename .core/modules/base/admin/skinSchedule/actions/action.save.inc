<?php
/** @var $this BaseSkinSchedule */

$VARS = $this->get_parsed_vars();

$item_id_initial = @(int)$VARS[$this->PK];

$this->DB->resetAllNotifications();

EkvDateHelper::fixDatePickerForSaveAction($VARS, 'man_dt_begin', 'dt_begin');
EkvDateHelper::fixDatePickerForSaveAction($VARS, 'man_dt_end', 'dt_end');

//--- UPDATE REC ---//
if ($item_id_initial > 0) {
    $old_info = $this->DB->BGetInfoById(null, $item_id_initial, $this->PK);

    if(!$this->IsError()){
        $this->fixDatesOrder($VARS);
        $id = $this->DB->UpdateRecord($item_id_initial, $VARS, "", $this->PK);
    }
}
//--- INSERT NEW REC ---//
else {
    if(!$this->IsError()){
        $this->fixDatesOrder($VARS);
        $id = $this->DB->InsertRecord($VARS);
    }
}

if (empty($id)) {
    $result = $this->DB->AddNew();
    $result['record'] = $result['fields'];
    $this->DB->push_vals_to_fields($this->get_incoming_vars(), $result['record']);

    $this->TML->assign('result', $result);
    $this->Display('common:page.edit.tpl');
} else {
    $this->ChangeLocation($this->browseAction);
}
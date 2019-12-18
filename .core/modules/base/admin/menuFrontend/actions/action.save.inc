<?php
/** @var $this BaseMenufrontend */

use Ekv\B\Base\MenuFrontend\MMenuFrontCreate;
use Ekv\B\Base\MenuFrontend\MMenuFrontEdit;

$VARS = $this->get_parsed_vars();

$item_id_initial = @(int)$VARS['id'];

$this->DB->resetAllNotifications();

//--- UPDATE REC ---//
if ($item_id_initial > 0) {

    $saveObj = new MMenuFrontEdit($this, $VARS, $this->langID, $this->menuType);
    $saveObj->setItemID($item_id_initial);
    $id = $saveObj->mainSave();
}
//--- INSERT NEW REC ---//
else {
    $saveObj = new MMenuFrontCreate($this, $VARS, $this->langID, $this->menuType);
    $saveObj->setAdditionalParams($this->create_for_id, $this->add_type);
    $id = $saveObj->mainSave();
}

if (empty($id)) {
    //$this->AddTextError(PAGES_EXISTS);
    $result = $this->DB->AddNew();
    $result['record'] = $result['fields'];
    $this->DB->push_vals_to_fields($this->get_incoming_vars(), $result['record']);


    $this->TML->assign('result', $result);
    $this->Display('common:page.edit.tpl');
} else {
    $this->ChangeLocation($this->browseAction);
}

?>

<?php
use Ekv\System\MenuFront\MenuFrontBackend;

$id_item = @(int)$_REQUEST["_input_id"];

$menuObj = new MenuFrontBackend();
$menuObj->correctMenuItemDelete($id_item);
//MenuFrontend::correctMenuItemDelete($id_item, true);

$this->ChangeLocation('browse');
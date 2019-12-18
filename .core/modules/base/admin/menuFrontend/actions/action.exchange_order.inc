<?php
/** @var $this BaseMenufrontend */

use Ekv\Helpers\System\SortOrder;
use Ekv\System\MenuFront\MenuFrontBackend;
use Ekv\System\MenuFront\MenuFrontBase;

$item_id = @(int)$_REQUEST["item_id"];

$menuObj = new MenuFrontBackend();
$menu_info = $menuObj->getMenuItem($item_id);

if(!empty($menu_info)){
    $parent_id = intval($menu_info["parent_id"]);

    $menuType = intval($this->menuType);
    $langID = intval($this->langID);


    $sortObj = new SortOrder(EkvGlobalHelper::TABLE_MENU_FRONT);
    $sortObj->performExchange(
        $item_id,
        $_REQUEST["direction"],
        MenuFrontBase::SORT_ORDER_DIRECTION,
        'id',
        "
            AND `parent_id` = '{$parent_id}'
            AND `menu_type` = '{$menuType}'
            AND `lang_id` = '{$langID}'
        "
    );
}

$this->ChangeLocation("browse");
<?php
/** @var $this BaseMenufrontend */

use Ekv\System\MenuFront\MenuFrontBackend;

$this->DB->resetAllNotifications();

$menuObj = new MenuFrontBackend();
$menuObj->initPrams($this->menuType, $this->langID);
$tree = $menuObj->getFullTree();
//pa($tree);exit;

$this->TML->assign("html_tree", $tree["jq"]);
$this->Display('common:custom_templates/menu_frontend/page.browse.tpl');
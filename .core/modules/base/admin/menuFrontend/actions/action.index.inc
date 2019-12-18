<?php
/** @var $this Entity */

use Ekv\Helpers\System\Lang;
use Ekv\System\MenuFront\MenuFrontBase;

$base_url = "/admin/base/menuFrontend/browse.html";

$langs = Lang::getInstance()->getLangsAvailable();

$lang_code = Lang::getInstance()->getLangCode(true);

$top_menu = array();
$middle_menu = array();

foreach($langs as $lang_val){
    $langID = $lang_val["id_lang"];

    $name = "{$lang_val["lang_name"]} [{$lang_val["lang_code"]}]";

    $top_menu[] = array(
        'name' => $name,
        'href' => $base_url."?menuType=".MenuFrontBase::MENU_TYPE_TOP."&langID={$langID}"
    );

    $middle_menu[] = array(
        'name' => $name,
        'href' => $base_url."?menuType=".MenuFrontBase::MENU_TYPE_MIDDLE."&langID={$langID}"
    );
}

$this->TML->assign("lang_code", $lang_code);
$this->TML->assign("top_menu", $top_menu);
$this->TML->assign("middle_menu", $middle_menu);
$this->Display('common:custom_templates/menu_frontend/page.index.tpl');
<?php
namespace Ekv\System\MenuFront;

class MenuFrontHelper
{
    static function menuTypeOptions()
    {
        return array(
            MenuFrontBase::MENU_TYPE_TOP => "верхнє",
            MenuFrontBase::MENU_TYPE_MIDDLE => "проміжне",
        );
    }

    static function getMenuTypeName($menuType)
    {
        $menuOptions = self::menuTypeOptions();
        $name = "";

        if(isset($menuOptions[$menuType])){
            $name = $menuOptions[$menuType];
        }

        return $name;
    }

}
 
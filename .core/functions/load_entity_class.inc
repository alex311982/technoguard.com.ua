<?php


function load_entity_class($module = '', $entity = '', $view = SITE_CORE_VIEW)
{
    if (empty($entity) || empty($module)) return;

    $module_class_path = PATH_CORE . DIRECTORY_SEPARATOR . 'modules' . DIRECTORY_SEPARATOR;
    $entity_path = $module_class_path . $module . DIRECTORY_SEPARATOR . $view . DIRECTORY_SEPARATOR . $entity;
    $entity_class_path = $entity_path . DIRECTORY_SEPARATOR . 'class.entity.' . $entity . '.inc';

    $db_class_path = $entity_path . DIRECTORY_SEPARATOR . 'class.db.' . $entity . '.inc';
    $db_class_name = 'Db' . $module . $entity;
    $entity_class_name = $module . $entity;
    //pa($entity_class_path, $db_class_path);

    if (file_exists($entity_path) && file_exists($db_class_path)) {
        include_once($db_class_path);
        include_once($entity_class_path);
        $_db = new $db_class_name();
        $_entity = new $entity_class_name();
        $_entity->DB = &$_db;
        if (file_exists($entity_path . DIRECTORY_SEPARATOR . 'fields.inc'))
            $_entity->DB->fields = include($entity_path . DIRECTORY_SEPARATOR . 'fields.inc');
        $_entity->Init($module, $entity);

        $_entity->ManualAfterDbInit();

        $_SESSION['RedirectQuery_' . $module . $entity] = $_entity->RedirectQuery;
        return $_entity;
    } else {
        trigger_error('Could not load Entity class: <i>"' . $entity_class_name . '"</i>', E_USER_ERROR);
    }

    return;
}

/**
 * loads module DB and/or Entity classes only if they are found
 * otherwise loads common classes instead of rising error as load_entity_class() does
 * @see load_entity_class
 * @param string $module
 * @param string $entity
 * @param string $view
 * @return bool
 */
function load_entity_class_optional($module = '', $entity = '', $view = SITE_CORE_VIEW)
{
    if (empty($entity) || empty($module)){
        return false;
    };

    $module_class_path = PATH_CORE . DIRECTORY_SEPARATOR . 'modules' . DIRECTORY_SEPARATOR;
    $entity_path = $module_class_path . $module . DIRECTORY_SEPARATOR . $view . DIRECTORY_SEPARATOR . $entity;
    $entity_class_path = $entity_path . DIRECTORY_SEPARATOR . 'class.entity.' . $entity . '.inc';

    $db_class_path = $entity_path . DIRECTORY_SEPARATOR . 'class.db.' . $entity . '.inc';
    $db_class_name = 'Db' . $module . $entity;
    $entity_class_name = $module . $entity;
    //pa($entity_class_path, $db_class_path);
    //pa($entity_path);

    if (file_exists($entity_path)) {
        //--- load module db if exists ---//
        if(file_exists($db_class_path)){
            include_once($db_class_path);
        }else{
            //--- or use common DB class ---//
            $db_class_name = "DB";
        }

        //--- load specific module Entity class ---//
        if(file_exists($entity_class_path)){
            include_once($entity_class_path);
        }else{
            //--- or common Entity ---//
            $entity_class_name = "Entity";
        }

        $_db = new $db_class_name();

        $_entity = new $entity_class_name();
        $_entity->DB = &$_db;
        if (file_exists($entity_path . DIRECTORY_SEPARATOR . 'fields.inc'))
            $_entity->DB->fields = include($entity_path . DIRECTORY_SEPARATOR . 'fields.inc');
        $_entity->Init($module, $entity);

        $_entity->ManualAfterDbInit();

        $_SESSION['RedirectQuery_' . $module . $entity] = $_entity->RedirectQuery;
        return $_entity;
    } else {
        //trigger_error('Module-entity doesn\'t exist: <i>"' . $entity_class_name . '"</i>', E_USER_ERROR);
        //throw new Exception('Module-entity doesn\'t exist: <i>"' . $entity_class_name . '"</i>');
    }

    return true;
}

function load_custom_class($class_file_name, $folder = "extra")
{
    EkvAutoload::mainAutoload($class_file_name);
    //__ekvAutoload($class_file_name);
//		$base_class_dir = PATH_CORE."/classes/";
//		$full_path = $base_class_dir."{$folder}/class.{$class_file_name}.inc";
//		if(isset($full_path)){
//			include_once $full_path;
//		}
}

?>

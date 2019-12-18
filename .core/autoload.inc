<?php
class EkvAutoloadException extends Exception{}


class EkvAutoload
{
    /**
     * can turn autoload off (when unserializing for example)
     * to avoid exception throwing when class can't be found
     * @var bool
     */
    static $THROW_EXCEPTION_NOT_FOUND = true;
    
    const SITE_BACKEND = "admin";
    const SITE_FRONTEND = "frontend";

    static private $_moduleClassesAuotoloadPathes = array();

    /**
     * True for backward compatible classes which require their namespace being converted to real path with lowering
     * Ekv\Api_Kassa\ApiKassaSalesManage -> ../api_kassa/ApiKassaSalesManage path
     * By setting flag to true we provide  support for camel case namespaces which don't require lowering
     * Ekv\ApiKassa\ApiKassaSalesManage -> ../ApiKassa/ApiKassaSalesManage path
     * For legacy code first try to load with lowering if not found one more try without lowering
     * and only then throw exception
     * @var bool
     */
    static private $_performMeaningfullPathLowering = true;

    /**
     * if sure module class is required you should invoke this method to add
     * module path to the list of search directories
     * @static
     * @param  $module
     * @param  $entity
     * @param  $view_type
     * @return void
     */
    static function addModuleClassPath($module, $entity, $view_type = self::SITE_BACKEND)
    {
        $path_key = "{$module}_{$entity}_{$view_type}";

        //--- make autoload pathes unique ---//
        if(array_key_exists($path_key, self::$_moduleClassesAuotoloadPathes) == false){
            $base_class_path = self::composeBasePath($module, $entity, $view_type);
            self::$_moduleClassesAuotoloadPathes[$path_key] = $base_class_path;
        }
    }

    static function addModuleClassPathFrontend($module, $entity)
    {
        self::addModuleClassPath($module, $entity, self::SITE_FRONTEND);
    }

    /**
     * entity root path /module/viewtype/entity/ 
     * @static
     * @param $module
     * @param $entity
     * @param $view_type
     * @return string
     */
    static function composeEntityPath($module, $entity, $view_type)
    {
        $base_class_path = PATH_CORE."/modules/{$module}/{$view_type}/{$entity}/";
        return $base_class_path;
    }

    static function composeBasePath($module, $entity, $view_type)
    {
        $base_class_path = PATH_CORE."/modules/{$module}/{$view_type}/{$entity}/_classes/";
        return $base_class_path;
    }

//    static function composeFullClassPath($module, $entity, $view_type, $class_name)
//    {
//        $base_class_path = self::composeBasePath($module, $entity, $view_type);
//        $full_class_path = $base_class_path . self::_getClassFilename($class_name);
//        if(file_exists($full_class_path) == false){
//            throw new EkvAutoloadException("composeFullClassPath incorrect path");
//        }
//        return $full_class_path;
//    }
    

    static private function _getClassFilename($class_name)
    {
        //return "class.{$class_name}.inc";
        return "{$class_name}.inc";
    }

    static function autoloadForModuleClasses($class_name)
    {
        //#------------------- autoload of module class is required -------------------#//
        if(
            !is_null(self::$_moduleClassesAuotoloadPathes)
            && strpos($class_name, "EkvM_") !== false
        ){

            foreach(self::$_moduleClassesAuotoloadPathes as $module_path_val){
                $full_path = $module_path_val . self::_getClassFilename($class_name);
                if(file_exists($full_path)){
                    include_once $full_path;
                    return true;// !!!
                    
                }
            }
        }
    }

    /**
     * autoload from all classes folder
     * @param  $class_name
     * @throws EkvAutoloadException
     * @return void
     */
    static function autoloadForCommonClassesFolder($class_name)
    {
        $folders = array(
            'legacy'
        );
        $base_path = PATH_CORE."/classes/extra/";
        $class_file_name = "class.{$class_name}.inc";
        //$full_class_path = PATH_CORE."/classes/extra/class.{$class_name}.inc";
        $full_class_path = $base_path . $class_file_name;

        if( file_exists($full_class_path) ){
            include_once($full_class_path);
        }else{
            $is_any_path_correct = false;
            //--- fetch possible subfolders name ---//
            foreach($folders as $folder_name){
                $tmp_full_class_path = "{$base_path}{$folder_name}/{$class_file_name}";

                if( file_exists($tmp_full_class_path) ){
                    include_once($tmp_full_class_path);
                    $is_any_path_correct = true;
                    break;
                }
            }
            //--- all path variants are wrong ---//
            if(
                $is_any_path_correct == false
                && strpos($class_name, "PHPExcel") === false
                && self::$THROW_EXCEPTION_NOT_FOUND
            ){
                throw new EkvAutoloadException("__autoload ->> can't load class << '$class_name' >>");
            }
        }
    }

    /**
     * @static
     * @param $class_name
     * @return bool
     */
    static function mainAutoload($class_name)
    {
        //pa($class_name);
        /*
         * avoid exception for smarty classes, simply skip them and allow smarty own loader
         * classname BEGINS FROM 'smarty' but can have word "smarty" inside
         */
        //if(stripos($class_name, 'smarty') !== false){
        if(stripos($class_name, 'smarty') === 0){
            //pa($class_name);
            return false;
        }

        //--- without namespaces ---//
        if(strpos($class_name, '\\') === false){
            //--- try to load module class ---//
            $module_load_res = self::autoloadForModuleClasses($class_name);
            if($module_load_res !== true){
                self::autoloadForCommonClassesFolder($class_name);
            }
        }
        //--- class having namespace ---//
        else{
            self::_loadNamespacedClassAttemps($class_name);
        }

        return true;
    }

    static private function _loadNamespacedClassAttemps($class_name)
    {
        //--- first - try to load class with $_performMeaningfullPathLowering = true  ---//
        try{
            self::loadNamespacedClass($class_name);
        }catch (EkvAutoloadException $aeObj){
            /*
             * If we are here it means that first attemp to load class failed then try to load
             * without lowering by setting $_performMeaningfullPathLowering = false
             */
            self::$_performMeaningfullPathLowering = false;

            /*
             * Turn back $_performMeaningfullPathLowering to default . So for new classes it would be the same
             * behaviour (first try path lowering, then try direct namespace to path convertion)
             */
            try{
                self::loadNamespacedClass($class_name);
            }catch(EkvAutoloadException $aeSecondTryObj){
                throw $aeSecondTryObj;
            }

            /*
             * Reset flag for overcomming classes regardless any conditions (bug when lowering has been turned off
             * while loading class, but next class requires lowering, make sure that any new class is being tried
             * to be loaded with lowering first)
             */
            self::$_performMeaningfullPathLowering = true;

        }
    }

    /**
     * Ekv\B\Trans\Stats\M_TrKassaStats
     * Ekv\F\Trans\Stats\M_TrKassaStats
     * @static
     * @param $class_name_with_nsp - classname containing fully qualified namespace (nsp)
     * @throws EkvAutoloadException
     */
    static function loadNamespacedClass($class_name_with_nsp)
    {
        $final_class_path = "";

        $nsp_parts = explode('\\', $class_name_with_nsp);
        if(count($nsp_parts) >= 2){
            //--- our local nsp ---//
            if($nsp_parts[0] == "Ekv"){
                $part1 = $nsp_parts[1];

                /**
                 * module namespace
                 * 'B' - backend - /modules/mod_name/admin/entity_name/_classes
                 * 'F' - frontend - /modules/mod_name/frontend/entity_name/_classes
                 */
                if($part1 == "B" || $part1 == "F"){
                    $view_type = "admin";
                    if($part1 == "F"){
                        $view_type = "frontend";
                    }
                    $base_module_path = PATH_MODULES . DIR_SEP;

                    /**
                     * get rid of Ekv\B\ModName\EnityName or Ekv\F\ModName\EnityName and keep the rest of nsp
                     * cause we need to interpose VIEW_TYPE after module name
                     */
                    $module_parts = array_slice($nsp_parts, 4);
                    $module_name = self::_toLowerOrLcfirst($nsp_parts[2]);
                    $entity_name = self::_toLowerOrLcfirst($nsp_parts[3]);

                    /**
                     * exclude class name cause it must not be lowered, and have to remain as it is
                     */
                    $class_fname_partial = array_pop($module_parts);
                    //--- make strlowered rest of path ---//
                    foreach($module_parts as &$mod_part_val){
                        $mod_part_val = self::_toLower($mod_part_val);
                    }

                    //--- add classes folder ---//
                    $classes_folder = "_classes";

                    $extra_module_path = implode(DIRECTORY_SEPARATOR, $module_parts);
                    if(!empty($extra_module_path)){
                        $extra_module_path .= DIR_SEP; // prevent duplicating //
                    }
                    /**
                     * .core\modules\trans\admin\stats\_classes\M_TrKassaStats.inc
                     * .core\modules\api\frontend\cmd\_classes\xml\M_ApiCmdResultXml.inc
                     */
                    $final_class_path = $base_module_path . $module_name . DIR_SEP .
                        $view_type . DIR_SEP . $entity_name . DIR_SEP . $classes_folder . DIR_SEP .
                        $extra_module_path . "{$class_fname_partial}.inc";
                }
                //--- /.core/classes folder global classes ---//
                else{
                    $final_class_path = self::_loadClassesFolder($nsp_parts);
                }
            }

            if(!empty($final_class_path)){
                //if(self::$_performMeaningfullPathLowering){$final_class_path = "fake";} pa($final_class_path);
                if(isset($_COOKIE["ekv_debug"]) && $_COOKIE["ekv_debug"] == 1){
                    pa($final_class_path, file_exists($final_class_path) && is_file($final_class_path));
                }

                if (file_exists($final_class_path) && is_file($final_class_path)) {
                    require_once $final_class_path;
                }else{
                    throw new EkvAutoloadException("Can't load namespaced class: {$final_class_path}");
                }
            }else{
                throw new EkvAutoloadException("Can't load namespaced class1: {$final_class_path}");
            }
        }
    }

    /**
     * autoloader for namespaced global(not module) classes like Ekv\Extra\Api\ApiManualHallDraw
     * @static
     * @param $namespaced_parts
     * @return string - full class path
     */
    static private function _loadClassesFolder($namespaced_parts)
    {
        //--- classname is the last item of namespaced parts ---//
        $class_name = array_pop($namespaced_parts);
        $nsp_classes_path = array_slice($namespaced_parts, 1);

        $base_classes_folder_path = PATH_CORE . DIR_SEP . "classes" .DIR_SEP;
        $nsp_path = implode('/', $nsp_classes_path);
        $nsp_path = self::_toLower($nsp_path);

        $final_class_path = "{$base_classes_folder_path}{$nsp_path}/{$class_name}.inc";
        return $final_class_path;
    }

    static private function _toLower($input_str)
    {
        $res = $input_str;
        if(self::$_performMeaningfullPathLowering){
            $res = strtolower($input_str);
        }
        return $res;
    }

    static private function _toLowerOrLcfirst($input_str)
    {
        if(self::$_performMeaningfullPathLowering){
            $res = strtolower($input_str);
        }else{
            /*
             * check whether we need to perform lcfirst (only when module or entity are something like 'schedMod',
             * let single-word modules be in lower case, like 'scheme/events' but 'schemeMod/eventsEn')
             */
            preg_match_all('#[A-Z]+#', $input_str, $matches);
            if(
                isset($matches[0])
                && sizeof($matches[0]) > 1
            ){
                $res = lcfirst($input_str);
            }else{
                $res = strtolower($input_str);
            }
        }
        return $res;
    }


}

/**
 * pay attention to load_custom_class() !!
 */
spl_autoload_register('EkvAutoload::mainAutoload');
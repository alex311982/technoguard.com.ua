<?php
namespace Ekv\B\Base\MenuFrontend;

abstract class MMenuFrontSaveCommon
{
    protected $DB;

    protected $vars;
    protected $_createForID;
    protected $_addType;

    protected $_entityObj;

    protected $_langID;
    protected $_menuType;

    function __construct(\Entity $entityObj, $vars, $langID, $menuType)
    {
        $this->_entityObj = $entityObj;

        $this->DB = $entityObj->DB;
        $this->vars = $vars;

        $this->DB->fields = $this->DB->IncludeSpecificFields(PATH_MODULES."/base/admin/menuFrontend/", "menu");
        $this->DB->table = \EkvGlobalHelper::TABLE_MENU_FRONT;

        $this->_langID = intval($langID);
        $this->_menuType = intval($menuType);

        \EkvException::ensure($this->_langID > 0 && $this->_menuType > 0, "Lang or MenuType error");
    }

    function mainSave()
    {
        $this->vars["title"] = trim($this->vars["title"]);

//        $url = str_replace(" ", "_", $this->vars['url']);
//        $url = str_replace('"', '', $url);
//        $url = str_replace('"', '', $url);
        $this->fixUrl();
    }

    function fixUrl()
    {
        $url = isset($this->vars["url"]) ? trim($this->vars["url"]) : "";
        $urlNew = "";

        if(!empty($url)){
            $parseRes = parse_url($url);

            if(isset($parseRes["path"])){
                $path = $parseRes["path"];

                //--- fix '/' at the beginning of the string ---//
                if(
                    !empty($path)
                    && $path[0] != '/'
                ){
                    $path = "/{$path}";
                }

                $query = "";
                if(isset($parseRes["query"])){
                    $query = "?{$parseRes["query"]}";
                }

                $urlNew = "{$path}{$query}";
            }
        }

        $this->vars["url"] = $urlNew;
    }
}
 
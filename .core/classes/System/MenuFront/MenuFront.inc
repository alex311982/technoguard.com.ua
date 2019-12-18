<?php
namespace Ekv\System\MenuFront;

use EkvCommonHelper;

class MenuFront extends MenuFrontBase
{
    private function _appendFrontUrl(&$raw)
    {
        $url = $raw["url"];
        if(empty($url)){
            $url = "/";
        }

        EkvCommonHelper::appendUrlWithLangPrefixRef($url);

        $raw["url"] = $url;
    }

    function getFullMenu()
    {
        $plainTree = $this->_getPlainRawTreeData(true);

        if(!empty($plainTree)){
            foreach($plainTree as &$ptVal){
                $this->_appendFrontUrl($ptVal);
            }
        }

        $treeHierar = $this->_parseTreeUnified($plainTree);

        return $treeHierar;
    }
}
 
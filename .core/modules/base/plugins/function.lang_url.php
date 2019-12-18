<?php

use Ekv\Helpers\System\Lang;

function smarty_function_lang_url($params, &$smarty)
{
    $url_input = @$params["url"];
    $assign = isset($params["assign"]) ? $params["assign"] : null;

    //$lang_info = Lang::getInstance()->getLangInfoCurrent();
    //pa($lang_info);

    $url_final = EkvCommonHelper::appendUrlWithLangPrefix($url_input);

    if(!is_null($assign)){
        $smarty->assign($assign, $url_final);
    }else{
        return $url_final;
    }
}

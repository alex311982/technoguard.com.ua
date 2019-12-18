<?php

use Ekv\Helpers\System\Lang;

function force_location($url = '')
{
    header('Location:' . $url);
    exit();
}

/**
 * takes url witout lang and appends it with lang prefix if required and performs redirect
 * @param $url_without_lang
 */
function force_location_lang($url_without_lang)
{
//    $langObj = Lang::getInstance();
//    $lang_code = $langObj->getLangCode(true);
//
//    //--- if nondefault langcode append it ---//
//    $url_final = $url_without_lang;
//    if(!empty($lang_code)){
//        $url_final = "/{$lang_code}{$url_final}";
//    }

    $url_final = EkvCommonHelper::appendUrlWithLangPrefix($url_without_lang);

    force_location($url_final);
}


function force_location_index()
{
    header('Location:' . "/");
    exit();
}

function force_location_login()
{
    header('Location:' . "/user/login.html");
    exit();
}

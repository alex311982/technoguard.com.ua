<?php

function smarty_function_inner_use($params, &$smarty)
{
    //pa(get_class($smarty));exit;

    $do_action = $params["do_action"];
    if($do_action == "get_full_tpl_dir_path"){
        return PATH_ROOT;
    }
    elseif($do_action == "exit"){
        exit;
    }
    elseif($do_action == "social_buttons"){
        $social_buttons = array(
            "vk" => array(
                "icon_small" => "/-images/social/vk.png",
                "url_global" => "http://vkontakte.ru/club25518769"
            ),
            "fb" => array(
                "icon_small" => "/-images/social/fb.png",
                "url_global" => "http://www.facebook.com/internetbilet"
            ),
            "tw" => array(
                "icon_small" => "/-images/social/twit.png",
                "url_global" => "http://twitter.com/internetbilet"
            ),
            "rss" => array(
                "icon_small" => "/-images/social/rss.png",
                "url_global" => EkvCommonHelper::appendUrlWithSiteHostConf("/rss/events.rss")
            ),
            "gplus" => array(
                "icon_small" => "/-images/social/gplus.png",
                "url_global" => "https://plus.google.com/b/105481092562197597262/"
            ),
            "odnokl" => array(
                "icon_small" => "/-images/social/odnokl.png",
                "url_global" => "http://www.odnoklassniki.ru/group/52069000413423"
            )
        );
        $assign = isset($params["assign"]) ? $params["assign"] : "_plugin_social_buttons";
        //pa($social_buttons);
        $smarty->assign($assign, $social_buttons);
    }
    elseif($do_action == "novinator"){
        $novinator_path = PATH_ROOT . '/novinator_ib/novinator.php';
        if(file_exists($novinator_path)){
            include_once $novinator_path;
            return "<!-- novinator done ok -->" . Novinator_get();
        }

    }

}

?>
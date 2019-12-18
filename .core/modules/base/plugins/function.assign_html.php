<?php

function smarty_function_assign_html($params, &$smarty)
{
    $additionalTitleParts = isset($params["add_parts"]) ? $params["add_parts"] : array();

    if (isset($_GET['qq']))print_r($params);

    if (SITE_CORE_VIEW == SITE_BACKEND) {
        $siteTitle = SITE_TITLE;
        echo(SITE_TITLE);
    } else {
        if (defined('SITE_TITLE_FRONTEND')) {
            $siteTitle = SITE_TITLE_FRONTEND;
        } else {
            $siteTitle = SITE_TITLE;
        }
    }
    if (isset($params['title'])) {
        //titles are formin in page.xxx.tpl manually
        if (isset($params['direct_title'])) {
            //echo "<h1>direct</h1>";exit;
            $siteTitle = $params['title'];
            $smarty->assign('html_title', $siteTitle);
        } else {
//                if (@$params['direction'] == "back"){
//                    $smarty->assign('html_title', $params['title'].SITE_TITLE_DELIMITER.$title);
//                }else{
//                    $smarty->assign('html_title', $title.SITE_TITLE_DELIMITER.$params['title']);
//                }

            if (SITE_CORE_VIEW == "frontend") {
                if (isset($params["direction"])
                    && $params['direction'] != "back"
                ) {
                    $smarty->assign('html_title', $siteTitle . SITE_TITLE_DELIMITER . $params['title']);
                } else {
                    $titleFinal = $params['title'] . SITE_TITLE_DELIMITER . $siteTitle;
                    // news name | субновини | Новини | Українське радіо
                    if (!empty($additionalTitleParts)) {
                        $titleFinal = $params['title'] . SITE_TITLE_DELIMITER ;
                        $titleFinal .= implode(SITE_TITLE_DELIMITER, $additionalTitleParts);
                        $titleFinal .= SITE_TITLE_DELIMITER . $siteTitle;
                    }

                    //--- DEFAULT PRINCIPLE - direction = back -> first is page title than standart site title ---//
                    $smarty->assign('html_title', $titleFinal);
                }
            }
        }
    } else {
        $smarty->assign('html_title', $siteTitle .
            SITE_TITLE_DELIMITER . ucfirst($_REQUEST['module']) .
            SITE_TITLE_DELIMITER . ucfirst($_REQUEST['entity']) .
            SITE_TITLE_DELIMITER . ucfirst($_REQUEST['action']));
    }
    if (isset($params['keywords'])) {
        $smarty->assign('html_keywords', $params['keywords']);
    }
    if (isset($params['description'])) {
        $smarty->assign('html_description', $params['description']);
    }

}

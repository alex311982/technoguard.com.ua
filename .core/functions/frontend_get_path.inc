<?php
/**
 * @deprecated
 * @param $url
 * @return array
 */
function frontend_get_path($url)
{
    $url_parts = parse_url($url);

    //--- fix index url & support params for short index url '/' ---//
    if (
        empty($url)
        || '/' == $url
        || (
            isset($url_parts["path"])
            && $url_parts["path"] == '/'
        )
    ) {
        $url = "/index.html";
        //pa($url);exit;
        $url_parts['path'] = $url;
    }

    //$url = (empty($url) || '/' == $url)?'/index.html':$url;
    $url_initial = $url;
    $uri = $url_parts['path'];
    $uri = str_replace(PAGE_EXTENSION, '', $uri);
    $uri = ($uri{0} == '/') ? substr($uri, 1) : $uri;
    $uri = str_replace('"', '', $uri);
    $uri_safe = mysql_real_escape_string($uri);
    $uri_with_ext = $uri . PAGE_EXTENSION;
    $uri_with_ext_safe = mysql_real_escape_string($uri_with_ext);

    if (!class_exists('DB')) {
        err404();
    }
    $DB = new DB();

    //#------------------- Look in modules -------------------#//
    $DB->query('SELECT `module`, `entity`, `action` FROM `base_url` WHERE `url` = "' . $uri_safe . '" LIMIT 0,1');
    if ($DB->get_affected_rows() == 1) {
        $url = $DB->fetch_row();
        //pa($url_initial, $uri_safe);

        return array(
            'module' => $url['module'],
            'entity' => $url['entity'],
            'action' => $url['action'],
        );
    }


    //#------------------- Look in static pages -------------------#//
    $sql = "
            SELECT `id`
            FROM `base_page`
            WHERE
                `url` = '{$uri_safe}'
                OR `url` = '{$uri_with_ext_safe}' 
             LIMIT 0,1
        ";
    $DB->query($sql);
    if ($DB->get_affected_rows() == 1) {
        $page = $DB->fetch_row();

        return array(
            'module' => 'base',
            'entity' => 'page',
            'action' => 'browse',
            'page_id' => $page['id'],
        );
    }

    err404();
}

?>

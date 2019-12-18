<?php
namespace Ekv\B\News\News;

use Ekv\Helpers\Backend\BackendUserGlobal;
use Ekv\Helpers\System\Lang;
use Ekv\News\Permissions\NewsPermissionContainer;
use Ekv\TContent\Backend\TranslBrowse;
use EkvGlobalHelper, DB, NewsNews, EkvDateHelper;

class NewsTranslBrowse extends TranslBrowse
{

    protected function _init()
    {
        $this->_rootIDKey = "news_id";
        $this->_tcTitleKey = "news_title";
        $this->_translTable = \EkvGlobalHelper::TABLE_NEWS_TRANSL;
    }

    protected function _getUrlCreate($langID, $tcID)
    {
        return MNewsHelper::getUrlCreate($langID, $tcID);
    }

    protected function _getUrlEdit($langID, $tcID)
    {
        return MNewsHelper::getUrlEdit($langID, $tcID);
    }

    protected function _postFormatResultRow($row)
    {
        //can be news or material viewd by current logged moder
        $sql = "
            SELECT id_news, type_document FROM `".\EkvGlobalHelper::TABLE_NEWS_BASE."`
            WHERE
                id_news = '{$row['news_id']['value']}'
            LIMIT 1
        ";

        $news = $this->DB->RunSql($sql);
        $newsTypeDocument = explode(',', $news['type_document']);

        //get admins
        $sql = "SELECT * FROM `".\EkvGlobalHelper::TABLE_BASE_USER."`";
        $admins = $this->DB->query($sql);

        $adminLogins = [];
        foreach ($admins as $admin) {
            $adminLogins[$admin['login']] = $admin;
        }

        if ($this->_user && in_array($this->_user->login, array_keys($adminLogins))) {
            $adminTypes = explode(',', $adminLogins[$this->_user->login]['type_document_after_create']);
            array_intersect($adminTypes, $newsTypeDocument) ? $canBeEdited = true : $canBeEdited = false;
        } else {
            $stuffLangUkCurrentModer = $this->_tcPermContainer->checkStuffAccess('stuff_lang_uk');
            $langUkCurrentModer = $this->_tcPermContainer->checkLangAccess(LANG::LANG_ID_UK);

            $moderTypes = [];
            $stuffLangUkCurrentModer ? $moderTypes[] = 'R' : null;
            $langUkCurrentModer ? $moderTypes[] = 'N' : null;

            array_intersect($moderTypes, $newsTypeDocument) ? $canBeEdited = true : $canBeEdited = false;
        }

        if ($canBeEdited) {
            $row = $this->_applyTransTitleUrl($row);
        } else {
            foreach ($row["man_langs"]["data"]["langs"] as $key => $item) {
                if ($item["lang_id"] == 1) {
                    $row["man_langs"]["data"]["langs"][$key]["has_access"] = false;
                }
            }
        }

        $row = $this->_markTopImportanceNews($row);
        $row = $this->_prepareManDate($row);
        $row = $this->_prepareEditors($row);

        return $row;
    }

    private function _prepareEditors($row)
    {
        $editors = $this->getEditorsForNews($row['news_id']['value']);

        $logins =  $this->getLogins();

        $logins[33] = ['moder_login' => 'admin'];
        $logins[35] = ['moder_login' => 'olga'];
        $logins[82] = ['moder_login' => 'apervak'];
        $logins[88] = ['moder_login' => 'newsadmin'];

        $formattedEditors = [];

        if (empty($editors)) {
//            echo '<pre>';
//            var_dump($row["last_editor"]);exit;
            $login = $logins[(int)$row["last_editor"]["value"]]['moder_login'];
            $formattedEditors[] = ['value' => $login . '(' . 1 . ')'];
        }

        foreach ($editors as $editor) {
            $login = $logins[(int)$editor['last_editor']]['moder_login'];
            $count = $editor['count'];
            $formattedEditors[] = ['value' => $login . '(' . $count . ')'];
        }

        $row["editors"]["data"] = $formattedEditors;

        return $row;
    }

    private function _prepareManDate($row)
    {
        $is_published = $row["is_published"]["value"];
        $date_published_mysql = $row["date_pub"]["value"];
        $date_pub_php = EkvDateHelper::mysqlDate2PhpDate($date_published_mysql);

        $mark_unpublished = false;
        if(
            !$is_published
            || $date_pub_php > time()
        ){
            $mark_unpublished = true;
        }

        $res = array(
            'date_created' => array(
                'value' => EkvDateHelper::getJqDatePickerFormatedDate($row["date_created"]["value"]),
                'mark_unpublished' => $mark_unpublished
            ),
            'date_pub' => array(
                'value' => EkvDateHelper::getJqDatePickerFormatedDate($date_published_mysql),
            ),
            'date_closed' => array(
                'value' => EkvDateHelper::getJqDatePickerFormatedDate($row["date_closed"]["value"]),
            )
        );

        $row["man_date_common"]["data"] = $res;
        return $row;
    }

    /*
     * Mark important and top news with special css classes
     */
    private function _markTopImportanceNews($row)
    {
        $css_class = "";
        if($row["is_top"]["value"] == 1){
            $css_class = "news-top";
        }elseif($row["is_main"]["value"] == 1){
            $css_class = "news-main";
        }

        if(!empty($css_class)){
            $row["news_title"]["css_class"] = $css_class;
            $row["text_short"]["css_class"] = $css_class;
        }
        return $row;
    }

    private function getEditorsForNews($newsId)
    {
        $sql = "
            SELECT *
            FROM `" . EkvGlobalHelper::TABLE_NEWS_EDITOR_SAVED_COUNT . "`      
            WHERE
                news_id = '{$newsId}'
            ORDER BY date_updated_count DESC
            LIMIT 5
        ";

        return $this->DB->RunSql($sql, true);
    }

    private function getLogins()
    {
        $sql = "
            SELECT backend_user.uid_global, moder.moder_login
            FROM `".EkvGlobalHelper::TABLE_USER_BACKEND."` backend_user
            LEFT JOIN `".EkvGlobalHelper::TABLE_MODER."` moder 
                ON backend_user.outer_user_id = moder.id_moder
            WHERE backend_user.uid_global NOT IN (33, 35, 82)
            ORDER BY backend_user.uid_global
        ";

        return $this->DB->RunSql($sql, true, 'uid_global');
    }

    private function selectModerAdditionalProperties($last_editor_glogal_uid)
    {
        $sql = "
            SELECT moder.stuff_lang_uk AS moder_stuff_lang_uk, moder.news_lang_uk AS moder_news_lang_uk
            FROM `".EkvGlobalHelper::TABLE_USER_BACKEND."` backend_user
            LEFT JOIN `".EkvGlobalHelper::TABLE_MODER."` moder 
                ON backend_user.outer_user_id = moder.id_moder
            WHERE backend_user.uid_global = '{$last_editor_glogal_uid}'
            LIMIT 1
        ";

        return current($this->DB->RunSql($sql, true));
    }
}
 
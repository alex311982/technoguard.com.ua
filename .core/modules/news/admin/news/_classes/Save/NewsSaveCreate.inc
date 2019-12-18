<?php
namespace Ekv\B\News\News\Save;

use Ekv\B\News\News\MNewsCreate;
use Ekv\B\News\News\MNewsTranslationRelated;
use Ekv\Helpers\System\Lang;
use EkvGlobalHelper;

class NewsSaveCreate extends NewsSaveCommon
{
    private $newsID;
    private $langID;

    protected function _init()
    {
        $this->newsID = $this->entityObj->newsID;
        $this->langID = $this->entityObj->langID;
    }


    function mainPerform()
    {
        parent::mainPerform();

        $id_final = 0;

        $create_for_news_id_flag = $this->newsID > 0;
        $this->entityObj->showLangFieldOptions();

        if($this->entityObj->isLangIDValid()){
            $this->vars["lang_id"] = $this->langID;
        }

        $this->DB->CheckVariables($this->vars);

        //--- check lang access ---//
        if(!$this->entityObj->IsError()){
            $this->_checkLangPermission($this->vars["lang_id"]);
        }

        $this->imgUploadObj->mainUploadAndResizeSimplified();

        /*
         * Create translation for specific existing newsID
         */
        if($create_for_news_id_flag){
            $this->vars["news_id"] = $this->newsID;
        }else{
            /*
             * Create news and translation at once (the lang is chosen from lang options)
             * Workaround for duplicating lang key. This will be updated in a few lines below, but to avoid duplication issue
             * just set temporary a random news_id
             */
            $this->vars["news_id"] = mt_rand(0, 9999) * -1;
        }

        //if(!$this->entityObj->IsError()){

            $this->_justBeforeSavingQuery();

            $this->vars["enabled"] = 1;
            $this->vars["date_created"] = \EkvDateHelper::mysqlNow();

            $id_final = $this->DB->InsertRecord($this->vars);

            if($id_final > 0){
                $this->_saveTags($id_final);

                if(!$create_for_news_id_flag){
                    $nCreateObj = new MNewsCreate($this->_newsPermContainer);
                    $nCreateObj->updateTcRootID($id_final);
                }
            }
        //}

        $this->_finalID = $id_final;


        $sql = "
            SELECT news_id FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."`
            WHERE
                id_transl = '{$id_final}'
            LIMIT 1
        ";

        $news_id = current($this->DB->RunSql($sql));

        //get admins
        $sql = "SELECT * FROM `".\EkvGlobalHelper::TABLE_BASE_USER."`";
        $admins = $this->DB->query($sql);

        $adminLogins = [];
        foreach ($admins as $admin) {
            $adminLogins[$admin['login']] = $admin;
        }

        if ($this->user && in_array($this->user->login, array_keys($adminLogins))) {
            $set = $adminLogins[$this->user->login]['type_document_after_create'];
        } else {
            $stuffLangUkCurrentModer = $this->_newsPermContainer->checkStuffAccess('stuff_lang_uk');
            $langUkCurrentModer = $this->_newsPermContainer->checkLangAccess(LANG::LANG_ID_UK);

            if ($langUkCurrentModer && $stuffLangUkCurrentModer) {
                $set = 'N,R';
            } else if ($langUkCurrentModer && !$stuffLangUkCurrentModer) {
                $set = 'N';
            } else if (!$langUkCurrentModer && $stuffLangUkCurrentModer) {
                $set = 'R';
            }
        }

        $sql = "
                UPDATE `".EkvGlobalHelper::TABLE_NEWS_BASE."` tl
                SET type_document= '{$set}'
                WHERE id_news= '{$news_id}' LIMIT 1
            ";

        $this->DB->query($sql);

        $sql = "
            SELECT period_item_track_link FROM `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."`
            WHERE
                id_transl = '{$id_final}'
        ";

        $period_item_track_link = current($this->DB->RunSql($sql));

        if (!preg_match("/play-live/i", $period_item_track_link)) {

            preg_match_all('/(\d+)$/', $period_item_track_link, $linkIds);

            $id = $linkIds[0][0];

            $id = $id ? $id : 0;

            $sql = "
            UPDATE `" . EkvGlobalHelper::TABLE_NEWS_TRANSL . "`
            SET period_item_track_id = {$id} 
            WHERE id_transl = {$id_final} limit 1
        ";

            $this->DB->query($sql);

        } elseif (preg_match("/play-live/i", $period_item_track_link)) {
            preg_match_all('/(\d+)$/', $period_item_track_link, $linkIds);

            $id = $linkIds[0][0];

            $id = $id ? $id : 0;

            $sql = "
                UPDATE `" . EkvGlobalHelper::TABLE_NEWS_TRANSL . "`
                SET period_item_track_id_live = {$id} 
                WHERE id_transl = {$id_final} limit 1
            ";

            $this->DB->query($sql);
        }

        $this->_finalizaing();
    }

    protected function _markPublished()
    {
        /*
         * In this place THERE IS A GUARANTEE THAT USER DOESN'T REQUIRE PREMODERATION !!,
         * so for this users the news is published after creation.
         * For supereditors let's mark a checkbox
         */
        $vars_is_published = true;

        //--- let supereditor check the checkbox "is_published" ---//
        if($this->_newsPermContainer->isSuperEditor){
            $vars_is_published = @$this->vars["is_published"] == 1;
        }

        //pa($this->vars);exit;
        $this->_setMarkPublishedFields($vars_is_published, $this->_getJqManDatePublished());
    }
}
 
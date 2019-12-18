<?php
namespace Ekv\B\News\News\Save;

use Ekv\B\News\News\MNewsTranslationRelated;
use EkvGlobalHelper;

class NewsSaveEdit extends NewsSaveCommon
{
    private $_srcItemID;
    private $_oldInfo;

    protected function _init()
    {
        $this->_srcItemID = self::getSrcItemID($this->vars, $this->PK);
    }

    function setlockStatus($status = 0)
    {
        $newsHelper = new MNewsTranslationRelated($this->_srcItemID);

        $newsHelper->setLockStatusAllLangs($status);
    }

    function mainPerform()
    {
        parent::mainPerform();

        $id_final = 0;
        $old_info = $this->DB->BGetInfoById(null, $this->_srcItemID, $this->PK);
        $this->_oldInfo = $old_info;

        $this->DB->CheckVariables($this->vars);

        //--- check lang access ---//
        $this->_checkLangPermission($this->vars["lang_id"]);

        //--- performs upload only if none errors have occured ---//
        $this->imgUploadObj->mainUploadAndResizeSimplified();

        if(!$this->entityObj->IsError()){

            $this->_justBeforeSavingQuery();

            $this->imgUploadObj->deleteImageOnEditingPageConditional($old_info);

//print_r($this->vars);
//die('1');
            $id_final = $this->DB->UpdateRecord($this->_srcItemID, $this->vars, "", $this->PK);

            $this->_saveTags($id_final);

            $this->imgUploadObj->removeAllSizePhotosDependingOnUploading($old_info[$this->photoFieldName]);
        }else{
            $_POST[$this->entityObj->inputName($this->photoFieldName)] = $old_info[$this->photoFieldName];
        }

        $this->_finalID = $id_final;

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
                SET period_item_track_id = {$id},
                    period_item_track_id_live = 0
                WHERE id_transl = {$id_final} limit 1
            ";

            $this->DB->query($sql);

        } elseif (preg_match("/play-live/i", $period_item_track_link)) {
            preg_match_all('/(\d+)$/', $period_item_track_link, $linkIds);

            $id = $linkIds[0][0];

            $id = $id ? $id : 0;

            $sql = "
                UPDATE `" . EkvGlobalHelper::TABLE_NEWS_TRANSL . "`
                SET period_item_track_id_live = {$id},
                    period_item_track_id = 0
                WHERE id_transl = {$id_final} limit 1
            ";

            $this->DB->query($sql);
        }

        $this->_finalizaing();
    }

    /**
     * Assumes that premoderation is not required when calling this method and has already been checked previously
     */
    protected function _markPublished()
    {
        $old_is_published = @$this->_oldInfo["is_published"] == 1;
        $vars_is_published = @$this->vars["is_published"] == 1;

        //--- let supereditor check the checkbox "is_published" ---//
        if($this->_newsPermContainer->isSuperEditor){
            $vars_is_published = $vars_is_published == 1;
        }else{
            $vars_is_published = $old_is_published;// save old published value
        }

        $this->_setMarkPublishedFields($vars_is_published, $this->_getJqManDatePublished());

    }

}
 
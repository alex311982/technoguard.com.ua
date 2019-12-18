<?php
namespace Ekv\B\News\News\Save;
use Ekv\B\News\News\Tags\MTagsSave;
use Ekv\Helpers\System\FileUpload\EntityImageUpload;
use Ekv\News\Permissions\NewsPermissionContainer;
use EkvGlobalHelper, NewsNews;

abstract class NewsSaveCommon
{
    const COMMING_FROM_APPROVAL = "approval";

    protected $DB;
    protected $entityObj;
    protected $vars;
    protected $photoFieldName;
    protected $PK;

    /**
     * @var EntityImageUpload
     */
    protected $imgUploadObj;

    protected $_imgFilenamePrefix = "news";

    protected $_finalID;

    /**
     * @var NewsPermissionContainer
     */
    protected $_newsPermContainer;

    function __construct(NewsNews $entityObj, $vars, $photo_field_name, $user = null)
    {
        $this->entityObj = $entityObj;
        $this->PK = $entityObj->PK;
        $this->DB = $entityObj->DB;
        $this->vars = $vars;
        $this->user = $user;

        $this->photoFieldName = $photo_field_name;

        $this->imgUploadObj = new EntityImageUpload($entityObj, "newsPhotos", $this->vars, false);
        $this->imgUploadObj->setExtraParams($_FILES, $this->photoFieldName, $this->_imgFilenamePrefix);

        $this->_newsPermContainer = $entityObj->getNewsPermissionsContainer();

        $this->_init();
    }

    protected function _init(){}

    function mainPerform()
    {
        $this->vars["last_editor"] = \EkvBackendAdmins::getSignedInGlobalUid();
    }

    protected function _justBeforeSavingQuery()
    {
        /*
         * If user has premoderation then he can't do anything with publishing.
         * Even when the news item was already published but such user performed "saving" then unpublish it
         */
        if($this->_newsPermContainer->hasPremoderation){
            $this->_setMarkPublishedFields(false, $this->_getJqManDatePublished());
        }else{

            //--- no premoderation is required ---//
            $this->_markPublished();
        }
    }

    /**
     * Date can be changed by all users but is_published only by superadmin.
     * When user with premoderation saves translation the transl is unpublished automatically
     */
    protected function _markPublished()
    {

    }

    protected function _setMarkPublishedFields($is_published, $jq_date_published = '')
    {
        //--- if date is not passed - set pub date as now ---//
        $mysql_date_published = \EkvDateHelper::mysqlNow();

        if(!empty($jq_date_published)){
            $mysql_date_published = \EkvDateHelper::convertJqDatePickerDate2MysqlDate($jq_date_published);
        }

        $this->vars["is_published"] = $is_published;
        $this->vars["date_pub"] = $mysql_date_published;

        if(!empty($this->vars["man_date_closed"])){
//            $this->vars["date_closed"] = EkvDateHelper::convertJqDatePickerDate2MysqlDate($this->vars["man_date_closed"]);
            $this->vars["date_closed"] = date('Y-m-d H:i:s', strtotime($this->vars["man_date_closed"]));
        }
            else {

            $this->vars["date_closed"] = '';
        }
    }

    protected function _checkLangPermission($langID)
    {
        if(!($this->_newsPermContainer->checkLangAccess($langID)
            || $this->_newsPermContainer->checkStuffAccess('stuff_lang_uk'))
        ){
            $this->entityObj->AddTextError("Ви не маєте прав для редагування новин у обраній мові");
        }
    }

    static function getSrcItemID($vars, $PK)
    {
        $src_item_id = isset($vars[$PK]) ? intval($vars[$PK]) : 0;
        return $src_item_id;
    }

    protected function _saveTags($translID)
    {
        $tagsObj = new MTagsSave($this->vars["man_tags"], $translID);
        $tagsObj->mainHandleTags();
    }

    protected function _finalizaing()
    {
        if(
            $this->_finalID > 0
            && !$this->entityObj->IsError()
        ){
            $this->_manageTopFlag();
            $this->entityObj->removeRedirectVars();

            /*
             * Comming from common news list or confirming translation
             */
            $browseAction = "";//$this->entityObj->browseAction;
//            if($this->entityObj->commingFrom == $this->entityObj->browseActionTranslation){
//                $browseAction = "transl_browse";
//            }

            switch($this->entityObj->commingFrom){
                case self::COMMING_FROM_APPROVAL:
                    $browseAction = "news_list";
                    break;
                default:
                    $browseAction = $this->entityObj->browseAction;
            }

            $url = compose_url_backend($browseAction);
            $this->entityObj->ChangeLocation($url);
        } else {
            $result = $this->DB->AddNew();
            $this->entityObj->setTagsPreloadedFromCsvString($result, $this->vars["man_tags"]);

            $this->entityObj->TML->assign('result', $result);
            $this->entityObj->displayEditTpl();

        }
    }

    /**
     * Keep is_top flag checked only for single PUBLISHED translation (within unique lang)
     * but preserve is_top checked for all unpublished
     */
    private function _manageTopFlag()
    {
        $is_top = @intval($this->vars["is_top"]);
        $is_published = @intval($this->vars["is_published"]);

        /*
         * Allow not single is_top flag for unpublished news
         * Don't perform deselecting of is_top when current news transl has not is_top or is not published
         */
        if(
            !$is_top
            || !$is_published
        ){
            return false;
        }

        $lang_id = @intval($this->vars["lang_id"]);

        $sql = "
            UPDATE `".\EkvGlobalHelper::TABLE_NEWS_TRANSL."`
            SET
                `is_top` = 0
            WHERE
                lang_id = '{$lang_id}'
                AND id_transl != '{$this->_finalID}'
                AND is_published = 1
        ";
        $this->DB->query($sql);

        return true;
    }

    /**
     * @param $PK
     * @param NewsNews $entityObj
     * @param $vars
     * @param $photo_field_name
     * @return NewsSaveCommon
     */
    static function factoryObj($PK, NewsNews $entityObj, $vars, $photo_field_name, $user = null)
    {
        $obj = null;

        //--- save after edit action ---//
        if(self::getSrcItemID($vars, $PK) > 0){
            $obj = new NewsSaveEdit($entityObj, $vars, $photo_field_name);
        }
        //--- save after create action ---//
        else{
            $obj = new NewsSaveCreate($entityObj, $vars, $photo_field_name, $user);
        }

        return $obj;
    }

    protected function _getJqManDatePublished()
    {
        return $this->vars["man_date_published"];
    }
}
 
<?php
/** @var $this BankPresenter */
use Ekv\Helpers\System\FileUpload\EntityImageUpload;

$table = $this->DB->table;
$table_pk = $this->PK;
$VARS = $this->get_parsed_vars();
$err = array();

$imgUploadObj = new EntityImageUpload($this, "bannerPhotos", $VARS, false);
$photo_field_name = "photo_file";
$photo_prefix = "banner";
$this->DB->CheckVariables($VARS);//!!!!!!!!!!

$id_item = @(int)$VARS[$table_pk];

$id = 0;
//#------------------- EDIT -------------------#//
if($id_item > 0)
{

    $old_info = $this->DB->BGetInfoById($table, $id_item, $table_pk);

    //--- performs upload only if none errors have occured ---//
    $imgUploadObj->mainUploadAndResize($_FILES, $photo_field_name, $photo_prefix);

    if ($this->IsError() == false){
        $imgUploadObj->deleteImageOnEditingPageConditionalVARS($old_info, $VARS, $photo_field_name);

        $id = $this->DB->UpdateRecord($id_item, $VARS, "", $table_pk);
        $imgUploadObj->removeAllSizePhotosDependingOnUploading($old_info[$photo_field_name]);
    }
}
//#------------------- INSERT -------------------#//
else
{
    $imgUploadObj->mainUploadAndResize($_FILES, $photo_field_name, $photo_prefix);

    /** @var \Ekv\Helpers\System\SortOrder $sortObj */
    $sortObj = $this->createSortOrderObj();
    $VARS["sort_order"] = $sortObj->getNextMaxOrder();

    if ($this->IsError() == false){
        $id = $this->DB->InsertRecord($VARS);
    }
}

if($id > 0 && !$this->IsError())
{
    $url = compose_url_backend('/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/banner_list');
    $extra_url = "";
    $this->ChangeLocation($url, $extra_url);
}
else
{
    $result = $this->DB->AddNew();
    $this->TML->assign('result', $result);
    $this->Display('common:page.edit.tpl');
}

?>

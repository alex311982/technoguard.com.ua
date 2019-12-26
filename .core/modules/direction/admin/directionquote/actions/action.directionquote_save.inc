<?php
/** @var $this BankPresenter */
use Ekv\Helpers\System\FileUpload\EntityImageUpload;

$table = $this->DB->table;
$table_pk = $this->PK;
$VARS = $this->get_parsed_vars();
$err = array();

$this->DB->CheckVariables($VARS);//!!!!!!!!!!

$id_item = @(int)$VARS[$table_pk];

$id = 0;
//#------------------- EDIT -------------------#//
if($id_item > 0)
{
    $id = $this->DB->UpdateRecord($id_item, $VARS, "", $table_pk);
}
//#------------------- INSERT -------------------#//
else
{
    $id = $this->DB->InsertRecord($VARS);
}

if($id > 0 && !$this->IsError())
{
    $url = compose_url_backend('/'.SITE_BACKEND.'/'.$this->module.'/'.$this->entity.'/directionquote_list');
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

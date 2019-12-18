<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */
//pa($_SESSION["backend_admin_info"]);

$page_id = @(int)$_REQUEST['page_id'];
if($page_id > 0 == false){
    force_location("/");
}
$result = $this->DB->bGetInfoById("base_page", $page_id);
if(empty($result) || $result["is_active"] == 0){
    force_location("/");
}
$body = $result["body"];

GalleryMultiUpload::initDb($this->DB);
$gallery_replaced_res  = GalleryMultiUpload::replaceGalleryTag($body);
//pa($gallery_replaced_res);

if($gallery_replaced_res !== false){
    $this->TML->assign("jquery_galleries", true);
    $this->TML->assign("galleries_structure", $gallery_replaced_res["galleries_structure"]);
    $this->TML->assign("use_jquery", true );
    $result["body"] = $gallery_replaced_res["text_body"];
}

$this->TML->assign("description_custom", $result["description"] );
$this->TML->assign("keywords_custom", $result["keywords"] );
$this->TML->assign('page', $result);
$this->Display('common:base/page/page.details.tpl');

?>

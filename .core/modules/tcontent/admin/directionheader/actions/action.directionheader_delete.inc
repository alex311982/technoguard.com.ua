<?php

use Ekv\Helpers\System\FileUpload\EntityImageUpload;

foreach($this->ids as $id){
    //--- delete news photo ---//


    $sql = "
                    SELECT photo_file
                    FROM directionheaders
                    WHERE
                        id = '{$id}'
                ";

    $news = $this->DB->RunSql($sql);

    if (!empty($news['photo_file'])) {
        $mockVars = array();
        $entityImgUploadObj = new EntityImageUpload(new \Entity(), "directionheaderPhotos", $mockVars, false);
        $entityImgUploadObj->removeAllSizePhotos($news['photo_file']);
    }

    //--- delete transl row ---//
    $sql = "
                    DELETE
                    FROM directionheaders
                    WHERE
                        id = '{$id}'
                ";

    $this->DB->query($sql);
}

$this->ChangeLocation('directionheader_list');
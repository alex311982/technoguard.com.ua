<?php

use Ekv\Helpers\System\FileUpload\EntityImageUpload;

foreach($this->ids as $id){
    //--- delete news photo ---//


    $sql = "
                    SELECT photo_file
                    FROM news
                    WHERE
                        id = '{$id}'
                ";

    $news = $this->DB->RunSql($sql);

    if (!empty($news['photo_file'])) {
        $mockVars = array();
        $entityImgUploadObj = new EntityImageUpload(new \Entity(), "newsPhotos", $mockVars, false);
        $entityImgUploadObj->removeAllSizePhotos($news['photo_file']);
    }

    //--- delete transl row ---//
    $sql = "
                    DELETE
                    FROM news
                    WHERE
                        id = '{$id}'
                ";

    $this->DB->query($sql);
}

$this->ChangeLocation('news_list');
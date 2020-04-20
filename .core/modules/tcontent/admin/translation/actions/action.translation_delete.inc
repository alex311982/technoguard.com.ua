<?php

use Ekv\Helpers\System\FileUpload\EntityImageUpload;

foreach($this->ids as $id){
    //--- delete transl row ---//
    $sql = "
                    DELETE
                    FROM translations
                    WHERE
                        id = '{$id}'
                ";

    $this->DB->query($sql);
}

$this->ChangeLocation('translation_list');
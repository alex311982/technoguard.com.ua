<?php
namespace Ekv\StatPage\Permissions;

use Ekv\TContent\Permissions\TcHelper;

class SpHelper extends TcHelper
{
    protected function _init()
    {
        $this->_hasPremoderationField = "sp_has_premoderation";
        $this->_superEditorField = "sp_super_editor";

        $this->_sessionKey = "statPagePermContainer";

        $this->_initPermLangFields(
            'sp_lang_uk',
            'sp_lang_en',
            'sp_lang_de',
            'sp_lang_ro',
            'sp_lang_ru'
        );
    }

}
 
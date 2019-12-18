<?php
namespace Ekv\News\Permissions;

use Ekv\TContent\Permissions\TcHelper;

class NewsPermHelper extends TcHelper
{
    protected function _init()
    {
        $this->_hasPremoderationField = "news_has_premoderation";
        $this->_superEditorField = "news_super_editor";

        $this->_sessionKey = "newsPermContainer";

        $this->_initPermLangFields(
            'news_lang_uk',
            'news_lang_en',
            'news_lang_de',
            'news_lang_ro',
            'news_lang_ru'
        );
    }
}
 
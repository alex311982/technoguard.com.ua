<?php
namespace Ekv\B\News\News;

use Ekv\TContent\Backend\TcCreate;
use EkvGlobalHelper;

class MNewsCreate extends TcCreate
{
    protected function _init()
    {
        $this->_tableRoot = EkvGlobalHelper::TABLE_NEWS_BASE;
        $this->_tableTransl = EkvGlobalHelper::TABLE_NEWS_TRANSL;
        $this->_fieldTcID = "news_id";
    }

}

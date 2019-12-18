<?php
namespace Ekv\News\Tags;
use Ekv\TContent\Tags\TagsRoot;
use EkvGlobalHelper;

class NewsTagsRoot extends TagsRoot
{
    protected function _initOverridable()
    {
        $this->_currentType = self::TYPE_NEWS;
    }

}
 
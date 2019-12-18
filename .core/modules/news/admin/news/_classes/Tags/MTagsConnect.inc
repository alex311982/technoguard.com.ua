<?php
namespace Ekv\B\News\News\Tags;
use Ekv\TContent\Tags\Save\TagsConnect;
use Ekv\TContent\Tags\TagsRoot;
use EkvGlobalHelper;

class MTagsConnect extends TagsConnect
{
    protected function _initOverridable()
    {
        $this->_currentType = TagsRoot::TYPE_NEWS;
    }


}
 
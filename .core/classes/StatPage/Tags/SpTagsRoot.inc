<?php
namespace Ekv\StatPage\Tags;

use Ekv\TContent\Tags\TagsRoot;

class SpTagsRoot extends TagsRoot
{
    protected function _initOverridable()
    {
        $this->_currentType = self::TYPE_STAT_PAGE;
    }
}
 
<?php
namespace Ekv\News\Permissions;
use Ekv\TContent\Permissions\TcPermissionContainer;

/**
 * Container which holds news permissions for specific user(moderator) and facilitates retrieving and comparing operations
 */
class NewsPermissionContainer extends TcPermissionContainer
{
    protected function _initException()
    {
        $this->ex = new NewsPermissionException();
    }

    protected function _initOverridable()
    {
        $npHelper = new NewsPermHelper();
        $this->_initLangMap(
            $npHelper->langUK,
            $npHelper->langEN,
            $npHelper->langDE,
            $npHelper->langRO,
            $npHelper->langRU
        );
        $this->_tcHelperObj = $npHelper;
    }
}
 
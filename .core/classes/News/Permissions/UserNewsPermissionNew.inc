<?php
namespace Ekv\News\Permissions;

use Ekv\TContent\Permissions\TcHelper;
use Ekv\TContent\Permissions\TcPermissionContainer;
use Ekv\TContent\Permissions\UserTсPermission;

class UserNewsPermissionNews extends UserTсPermission
{
    /**
     * Set tchelper descendant
     * Set session key $sessionKey
     * @return TcHelper
     */
    protected function _initOverridable()
    {
        $this->tcHelper = new NewsPermHelper();
        $this->sessionKey = $this->tcHelper->sessionKey;
    }

    /**
     * @param $has_premoderation
     * @param $super_editor
     * @param $moder_info
     * @return TcPermissionContainer - descendant
     */
    protected function _createTcPermContainer($has_premoderation, $super_editor, $moder_info)
    {
        return new NewsPermissionContainer(
            $has_premoderation,
            $super_editor,
            $moder_info
        );
    }

    protected function _initException()
    {
        $this->ex = new NewsPermissionException();
    }
}
 
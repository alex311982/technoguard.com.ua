<?php
namespace Ekv\StatPage\Permissions;

use Ekv\TContent\Permissions\TcPermissionContainer;

class SpPermissionContainer extends TcPermissionContainer
{
    protected function _initException()
    {
        $this->ex = new SpPermissionException();
    }

    protected function _initOverridable()
    {
        $spHelper = new SpHelper();
        $this->_initLangMap(
            $spHelper->langUK,
            $spHelper->langEN,
            $spHelper->langDE,
            $spHelper->langRO,
            $spHelper->langRU
        );
        $this->_tcHelperObj = $spHelper;
    }

}
 
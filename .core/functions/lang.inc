<?php
use Ekv\Helpers\System\Lang;

/**
 * User: sunsey
 * Date: 27.06.13
 */

function ekvLangID()
{
    return Lang::getInstance()->getLangID();
}


function ekvLangCode()
{
    return Lang::getInstance()->getLangCode();
}

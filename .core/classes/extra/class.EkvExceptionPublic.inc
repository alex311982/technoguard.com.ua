<?php

/**
 * If private message is passed then it's critical exception.
 * If only public message is passed - then handle it and display to user as-is
 */
class EkvExceptionPublic extends EkvException
{
    private $_msgPublic;

    function __construct($msg_public, $msg_private = null, $code = null)
    {
//        if(is_null($msg_private)){
//            $msg_private = $msg_public;
//        }

        parent::__construct($msg_private, $code);
        $this->_msgPublic = $msg_public;
    }

    function getPublicMsg()
    {
        return $this->_msgPublic;
    }

    function getPrivateMsg()
    {
        return $this->message;
    }

    function hasPrivateMsg()
    {
        return !empty($this->message);
    }

    static function ensure($expr, $fail_msg_public = "", $fail_msg_private = null)
    {
        if(!$expr){
            throw new static($fail_msg_public, $fail_msg_private);
        }
    }

//    static function rise($fail_msg)
//    {
//        self::ensure(false, $fail_msg);
//    }
}

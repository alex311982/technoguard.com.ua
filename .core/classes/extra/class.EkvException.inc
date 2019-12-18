<?php

class EkvException extends Exception
{
    static function ensure($expr, $fail_msg = "")
    {
        if(!$expr){
            throw new static($fail_msg);
        }
    }

    static function rise($fail_msg)
    {
        self::ensure(false, $fail_msg);
    }
}

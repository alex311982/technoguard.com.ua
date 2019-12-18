<?php

class ErrorCustom
{
    var $errors = array();
    var $messages = array();

    function __construct()
    {
        if (!isset($_SESSION['errors'])) $_SESSION['errors'] = array();
        if (!isset($_SESSION['messages'])) $_SESSION['messages'] = array();
        $this->errors = $_SESSION['errors'];
        $this->messages = $_SESSION['messages'];
    }

    function IsError()
    {
        if (!empty($_SESSION['errors']))
            return true;
        else
            return false;
    }

    function IsMessges()
    {
        if (!empty($this->messages))
            return true;
        else
            return false;
    }

    function AddError($code, $attrs)
    {
        $matches = array();
        preg_match('|%%(.+)%%|i', $code, $matches);
        $replacment = array();
        $search = array();
        foreach ($matches as $m) {
            if (isset($attrs[$m])) {
                $search [] = '/%%' . $m . '%%/i';
                $replacment [] = $attrs[$m];
            }
        }
        $error = preg_replace($search, $replacment, $code);
        $this->AddTextError($error);
    }

    function addTextErrorWithWildcard($msg_error_wildcard, $field_name)
    {
        $this->AddError($msg_error_wildcard, array('field' => $field_name));
    }

    function AddTextError($str)
    {
        $this->errors [] = $str;
        $_SESSION['errors'] = $this->errors;
    }

    function AddTextMessage($str)
    {
        $this->messages [] = $str;
        $_SESSION['messages'] = $this->messages;
    }

    function ResetErrors()
    {
        $this->errors = array();
        $_SESSION['errors'] = $this->errors;
    }

    function ResetMessages()
    {
        $this->messages = array();
        $_SESSION['errors'] = $this->messages;
    }

    /**
     * known bug
     * must call on $this->DB to avoid error and message duplicating
     */
    function resetAllNotifications()
    {
        $this->ResetErrors();
        $this->ResetMessages();
    }

    function GetErrors()
    {
        return $this->errors;
    }

    function GetMessages()
    {
        return $this->messages;
    }
}

?>
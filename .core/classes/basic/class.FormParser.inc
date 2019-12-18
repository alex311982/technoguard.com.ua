<?php

include 'error_codes.php';

/**
 * @deprecated
 */
class FormParser extends Base
{
    var $Fields = "";
    var $Types = "";
    var $Errors = array();
    var $Messages = array();
    var $VARS = array();

    function __construct()
    {
        parent::__construct();
        if (is_file('types.php'))
            $this->Types = include('types.php');
        if (is_file('fields.php'))
            $this->Fields = include('fields.php');
    }

    function Validate($formvars, $types = '', $fields = '')
    {
        $this->Clear();
        if ($types != '') $this->Types = $types;
        if ($fields != '') $this->Fields = $fields;
        if (empty($this->Fields)) {
            $this->AddError(E_PARSER_EMPTYFIELDS);
            return false;
        }
        if (empty($this->Types)) {
            $this->AddError(E_PARSER_EMPTYTYPES);
            return false;
        }
        if (empty($formvars)) {
            $this->AddError(E_PARSER_EMPTYINPUT);
            return false;
        }
        foreach ($this->Fields as $key1 => $item) {
            if (!isset($formvars[$item['name']])) {
                $formvars[$item['name']] = '';
            }

            $var = $formvars[$item['name']];
            if ($this->ParseVar($item['name'], $var, $item['type']))
                $this->VARS[$item['name']] = array('value' => $var, 'valid' => true);
            else {
                if (isset($item['error_message']))
                    $message = $item['error_message'];
                else
                    $message = M_PARSER_INVALID;
                $this->VARS[$item['name']] = array('value' => $var = $formvars[$item['name']], 'valid' => false, 'message' => $message);
            }
            if (isset($item['values']))
                $this->VARS[$item['name']]['values'] = $item['values'];

        }

        return count($this->Errors) == 1;
    }

    function Clear()
    {
        $this->Messages = array();
        $this->Errors = array();
        $this->VARS = array();
    }

    function AddError($message)
    {
        array_push($this->Errors, $message);
    }

    function AddMessage($message)
    {
        array_push($this->Messages, $message);
    }

    function ParseVar($name, &$var, $type)
    {
        if (isset($this->Types[$type]))
            $condition = $this->Types[$type];
        else {
            $this->AddError(sprintf(E_PARSER_TYPENOTFOUND, $type));
            return false;
        }
        $out_error = "";
        if (!$this->CheckVar($name, $var, $condition, $out_error))
            if (isset($condition['default'])) {
                $var = $condition['default'];
                $this->AddMessage(sprintf(M_PARSER_CHANGEDTODEFAULT, $name));
            }
            else {
                $this->AddError($out_error);
                return false;
            }
        return true;
    }

    function CheckVar($name, &$var, &$condition, &$out_error)
    {
        if (!isset($var)) {
            $out_error = sprintf(E_PARSER_NOSET);
            return false;
        }
        if (!isset($condition['data_type'])) $condition['data_type'] = 'string';
        switch ($condition['data_type']) {
            case 'integer':
            case 'float':
                if ($condition['data_type'] == 'integer')
                    $var = (int)$var;
                else
                    $var = (double)$var;
                if (isset($condition['min_value']) && $var < $condition['min_value']) {
                    $out_error = sprintf(E_PARSER_TOOLOW, $name);
                    return false;
                }
                if (isset($condition['max_value']) && $var > $condition['max_value']) {
                    $out_error = sprintf(E_PARSER_TOOHIGH, $name);
                    return false;
                }
                break;
            case 'enum':
                if (isset($condition['values'])) {
                    $found = false;
                    foreach ($condition['values'] as $value)
                        if ($var == $value) {
                            $found = true;
                            break;
                        }
                    if (!$found) {
                        $out_error = sprintf(E_PARSER_INVALID, $name);
                        return false;
                    }
                }
                break;

            default: //string
                if (isset($condition['replace']))
                    foreach ($condition['replace'] as $key => $value)
                        $var = str_replace($key, $value, $var);
                if (isset($condition['min_length']) && strlen($var) < $condition['min_length']) {
                    $out_error = sprintf(E_PARSER_TOOSHORT, $name);
                    return false;
                }
                if (isset($condition['max_length']) && strlen($var) > $condition['max_length']) {
                    $out_error = sprintf(E_PARSER_TOOLONG, $name);
                    return false;
                }
                if (isset($condition['reg']) && !preg_match($condition['reg'], $var)) {
                    $out_error = sprintf(E_PARSER_REG, $name);
                    return false;
                }
        }
        return true;
    }

    function Display($VARS = '', $Fields = '', $Template = '')
    {
        if (empty($VARS))
            $vars = $this->VARS;
        else
            $vars = $VARS;
        if (empty($Fields))
            $fields = $this->Fields;
        else
            $fields = $Fields;
        if (empty($Template))
            $template = 'content.formparser.tpl';
        else
            $template = $Template;
        $this->TML->smatry->assign('formvars', $vars);
        $this->TML->smatry->assign('formfields', $fields);
        $this->TML->smatry->Display($template);
    }
}

?>

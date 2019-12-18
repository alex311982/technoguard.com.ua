<?php
/**
 * Smarty plugin
 * @package Smarty
 * @subpackage plugins
 */



function smarty_modifier_correct_url($string)
{
	//echo "<h2>STRING = $string</h2>";
    return "/{$string}.html";
}



?>

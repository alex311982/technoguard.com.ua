<?php
/**
 * @var $this NewsNews
 */

$result = $this->DB->AddNew();

$this->TML->assign('result', $result);
$this->displayEditTpl();
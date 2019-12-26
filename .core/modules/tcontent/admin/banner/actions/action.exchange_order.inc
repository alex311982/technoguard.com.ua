<?php

$item_id = @(int)$_REQUEST["item_id"];

/** @var \Ekv\Helpers\System\SortOrder $sortObj */
$sortObj = $this->createSortOrderObj();

$sortObj->performExchange(
    $item_id,
    $_REQUEST["direction"],
    "DESC",
    $this->PK
);

$this->RedirectQuery = "";
$this->ChangeLocation("banner_list");
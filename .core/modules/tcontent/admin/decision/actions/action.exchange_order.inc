<?php

$item_id = @(int)$_REQUEST["item_id"];

$result = $this->DB->runSql("SELECT direction_type FROM decisions WHERE id={$item_id}");

if($result){
    /** @var \Ekv\Helpers\System\SortOrder $sortObj */
    $sortObj = $this->createSortOrderObj($result['direction_type']);

    $sortObj->performExchange(
        $item_id,
        $_REQUEST["direction"],
        "DESC",
        $this->PK
    );

    $this->RedirectQuery = "";
    $this->ChangeLocation("decision_list");
}
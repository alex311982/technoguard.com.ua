<?php

if (__UID > 0) {
    if(isset($_REQUEST["cmd"]) && $_REQUEST["cmd"] == "cancel_order"){
        include_once "inc.order_cancel.inc";
    }

    $sql = "
        SELECT *, ob.id_order as order_id
        FROM `".EkvRoomHelper::TABLE_ORDER_BASE."` ob
        WHERE
            ob.uid = ".__UID."
        ORDER BY
            ob.id_order DESC
    ";
    
    //--- accumulate unique event ids ---//
    $event_ids = array();
    
    //--- fetch orders ---//
    $orders = array();
    $this->DB->query($sql);
    while($row = $this->DB->fetch_row()){
        $orders[] = $row;
        $event_ids[$row["event_id"]] = $row["event_id"];
    }

    //--- events info ---//
    $eventObj = new EkvRoomEvent();
    $events = $eventObj->eventFullInfoPreparedToDisplayMultiple($event_ids);

    //--- formated order seats ---//
    $orderFormatObj = new EkvOrderFormat();
    $formated_order_seats = $orderFormatObj->getFormatedExtraOrderStrs($orders);
    //pa($formated_order_seats);exit;
    
    $FORMATED_RES = array();
    foreach($orders as $order_val){
        $order_id_current = $order_val["id_order"];
        $event_id_current = $order_val["event_id"];
        
        //--- can be canceled ---//
        $can_be_canceled = $this->canOrderStatusBeCanceled($order_val);
        $url_cancel = "/user/orders.html?cmd=cancel_order&order_id={$order_id_current}";

        $FORMATED_RES[] = array(
            "main_info" =>
                array(
                    "order_id" => $order_id_current,
                    "event_name" => $events[$event_id_current]["event_name"],
                    "event_url" => $events[$event_id_current]["url_full"],
                    "total_price" => $order_val["total_price"],
                    "can_be_canceled" => $can_be_canceled,
                    "url_cancel" => $url_cancel,
                    "status_name" => EkvRoomHelper::getOrderStatusName($order_val["order_status"])
                ),
            "order2string_data" => isset($formated_order_seats[$order_id_current])? $formated_order_seats[$order_id_current] : '',
        );
    }
    
    $this->TML->assign("orders_formated", $FORMATED_RES );
    $this->Display('common:base/user/page.orders.tpl');
}
else {
    force_location("/");
}

?>
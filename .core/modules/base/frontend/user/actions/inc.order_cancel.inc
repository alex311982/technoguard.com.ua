<?php
/**
 * @var $this BaseUser
 * user fails allowed orders
 */

$order_id = @intval($_REQUEST["order_id"]);
$uid = __UID;

if($order_id > 0 && $uid > 0){
    $order_info = array();
    $orderObj = EkvRoomEventOrder::factoryOrderInfoObjectNew($order_id, $order_info);
    //pa($order_info);

    if(
        !empty($order_info)
        && $order_info["uid"] == $uid
        && $this->canOrderStatusBeCanceled($order_info)
    ){
        $orderObj->setOrderStatusFailed($order_id, true);
        $orderObj->sendOrderStatusToUser($order_id, EkvRoomHelper::ORDER_STATUS_PAYMENT_FAILED);

        $subject = "отмена заказа #{$order_id} пользователем";
        $msg = "Отмена пользователем заказа #{$order_id} <br><br>Время: "
            . EkvDateHelper::convert2DefaultBackendDateFormat(EkvDateHelper::mysqlNow());
        EkvMailNotifier::sendSimpleAdminNotification($subject, $msg);

        $loggerObj = new EkvFileLogger("orders_canceled_by_user");
        $loggerObj->writeStringPlain("#{$order_id} - userID: {$uid}");
    }
    force_location("/user/orders.html");
}
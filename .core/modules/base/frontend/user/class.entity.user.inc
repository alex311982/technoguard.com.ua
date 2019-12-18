<?php
/**
 * ============================================================
 * File name: xxx.xxx
 *      Date: xx/xx/2009
 *   Purpose: 
 * ============================================================
 */

class BaseUser extends Entity
{
    function __construct()
    {
        parent::__construct();
        load_config ( dirname(__FILE__).DIRECTORY_SEPARATOR."conf.inc" );
    }

    function prepareProfileFields()
    {
        //--- assumes registration fields are currently in use ---//
        unset($this->DB->fields["password"]);
        unset($this->DB->fields["captcha"]);
        unset($this->DB->fields["reg_date"]);
        unset($this->DB->fields["hash"]);
        unset($this->DB->fields["enabled"]);
    }

    function prepareSubscriptionIndepFields()
    {
        $this->DB->fields["password"]["required"] = false;
        unset($this->DB->fields["phone"]);
    }

    /**
     * order status is uncancelable or operator has put comment
     * @param  $order_info
     * @return bool
     */
    function canOrderStatusBeCanceled($order_info)
    {
        $status_int = $order_info["order_status"];
        $responsible_admin_id = $order_info["responsible_admin_id"];
        
        //--- noncancelable statuses ---//
        $noncancelable_statuses = array(
            EkvRoomHelper::ORDER_STATUS_PAYMENT_FAILED,
            EkvRoomHelper::ORDER_STATUS_SUCCEED,
            EkvRoomHelper::ORDER_STATUS_IN_DELIVERY,
        );
        $can_be_canceled = true;
        if(
            in_array($status_int, $noncancelable_statuses)
            || $responsible_admin_id > 0
        ){
            $can_be_canceled = false;
        }
        return $can_be_canceled;
    }

}

?>
<div class="user-orders content-middle2">
	<h3 class="h3-colored">Ваши заказы</h3>

    <!--{if !empty($orders_formated)}-->
    <table class="standart-display" cellpadding="0" cellspacing="0" width="99%">
    <tr>
        <th>№</th>
        <th>мероприятие</th>
        <th>места</th>
        <th>сумма</th>
        <th>статус</th>
        <th>&nbsp;</th>
    </tr>
    <!--{foreach from=$orders_formated item=order_val}-->
    <!--{assign var="order_val_main_info" value=$order_val.main_info}-->
    <tr>
        <td>
            <!--{$order_val_main_info.order_id}-->
        </td>
        <td>
            <a href="<!--{$order_val_main_info.event_url}-->"><!--{$order_val_main_info.event_name}--></a>
        </td>
        <td>
        <!--{if isset($order_val.order2string_data.row_seats_data)}-->
            <!--{foreach from=$order_val.order2string_data.row_seats_data item=row_seats_data_val}-->
                <p class="sector-name"><!--{$row_seats_data_val.sector_name}--></p>
                <!--{if $row_seats_data_val.use_row_num_seat_num == 1}-->
                <!--{foreach from=$row_seats_data_val.rows_info item=row_val}-->
                    ряд <!--{$row_val.row_num}-->: <!--{$row_val.seats_csv}-->;
                <!--{/foreach}-->
                <!--{else}-->
                    количество мест: <!--{$row_seats_data_val.cells_number_without_rows_seats}-->
                <!--{/if}-->
            <!--{/foreach}-->
        <!--{elseif isset($order_val.order2string_data.karabas_data)}-->
            <div class="kar-seats">
            <!--{foreach from=$order_val.order2string_data.karabas_data item=kar_seat}-->
                <!--{$kar_seat.place_text}--> <br>
            <!--{/foreach}-->
            </div>

        <!--{elseif isset($order_val.order2string_data.vticket_data)}-->
            <div class="vtk-seats">
            <!--{foreach from=$order_val.order2string_data.vticket_data item=vtk_seat}-->
                <em><!--{$vtk_seat.price_name}--></em> - мест: <!--{$vtk_seat.group_amount}-->
                <br/>
            <!--{/foreach}-->
            </div>
        <!--{/if}-->

        </td>
        <td>
            <!--{$order_val_main_info.total_price}-->
        </td>
        <td>
            <!--{$order_val_main_info.status_name}-->
        </td>
        <td>
            <!--{if $order_val_main_info.can_be_canceled == true}-->
            <a href="<!--{$order_val_main_info.url_cancel}-->"
                ><img src="/-images/btns/delete.png" alt="отменить заказ" title="отменить заказ" /></a>
            <!--{/if}-->
        </td>
    </tr>
    <!--{/foreach}-->
    </table>
    <!--{/if}-->


</div>

<style>
.user-orders .kar-seats,
.user-orders .vtk-seats
{
    font-size: 13px;
}
</style>

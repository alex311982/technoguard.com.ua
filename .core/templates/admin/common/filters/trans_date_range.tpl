<!--{*
<!--{pa a=$attrs}-->
<!--{pa a=$smarty.post}-->
 *}-->

<!--{* USED IN BROWSE ACTION, INSTEAD OF STANDART FILTER *}-->

<style type="text/css">
    table.grid tr td input.dr-input{
        width: 80px;
    }
    .dr-clear{
        cursor: pointer;
    }
</style>

<!--{assign var="dr_input_from" value="_filter_`$attrs.filter_data.input_date_from`"}-->
<!--{assign var="dr_js_ID_from" value="idJs`$dr_input_from`"}-->

<!--{assign var="dr_input_to" value="_filter_`$attrs.filter_data.input_date_to`"}-->
<!--{assign var="dr_js_ID_to" value="idJs`$dr_input_to`"}-->

<!-- date inputs -->
<ul class="nulled">
<li>
    <label for="<!--{$dr_js_ID_from}-->">От:</label>
    <input class="dr-input" id="<!--{$dr_js_ID_from}-->" type="text"
       name="<!--{$dr_input_from}-->" value="<!--{$smarty.post.$dr_input_from|escape}-->" />
    <img src="/-js/date-picker/images/clear_btn.png" alt="clear" class="dr-clear" title="очистить">
</li>
<li style="padding-top:1px;">
    <label for="<!--{$dr_js_ID_to}-->">До:</label>
    <input class="dr-input" id="<!--{$dr_js_ID_to}-->" type="text"
       name="<!--{$dr_input_to}-->" value="<!--{$smarty.post.$dr_input_to|escape}-->" />
    <img src="/-js/date-picker/images/clear_btn.png" alt="clear" class="dr-clear" title="очистить">
</li>
</ul>
<!-- /date inputs/ -->

<script type="text/javascript">
    $(function(){
        (function(){
            //--- dr = date range ---//
            var drIdFrom = '<!--{$dr_js_ID_from}-->';
            var drIdTo = '<!--{$dr_js_ID_to}-->';
            var localDatepickerSettings = jQuery.extend({}, dtpSettingsGlobal);

            //--- dont show timepicker ---//
            localDatepickerSettings.showTime = false;

            $('#'+drIdFrom).datepicker(localDatepickerSettings);
            $('#'+drIdTo).datepicker(localDatepickerSettings);
            //--- clear input ---//
            $('.dr-clear').click(function(){
                $(this).siblings().closest(".dr-input").val('');
            });
        }());

    });
</script>
<!--{* USED FOR ALL CUSTOM MARKUP PAGES , WHICH REQUIRE DATE RANGE FILTER  *}-->

<script type="text/javascript">
$(function() {
    var dtpSettings = dtpSettingsGlobal;
    dtpSettings.showTime = false;

    <!--{assign var="input_name" value="_input_date_from"}-->
    $('input[name=<!--{$input_name}-->][type=text]').datepicker(dtpSettings);
    $("#id<!--{$input_name}-->_ClearBtn").click(function(){
        $('input[name=<!--{$input_name}-->]').val("");
    });

    <!--{assign var="input_name" value="_input_date_to"}-->
    $('input[name=<!--{$input_name}-->][type=text]').datepicker(dtpSettings);
    $("#id<!--{$input_name}-->_ClearBtn").click(function(){
            $('input[name=<!--{$input_name}-->]').val("");
    });
});
</script>


<!-- = = = = = = [ date range filter automatic ] = = = = = = -->
<div style="">
    <!-- date from -->
    <!--{assign var="input_name" value="_input_date_from"}-->
    С: <input type="text" readonly="readonly" name="<!--{$input_name}-->" value="<!--{$vars.date_from}-->"/>
    <img id="id<!--{$input_name}-->_ClearBtn" src="/-js/date-picker/images/clear_btn.png" alt="clear" class="common-link" title="очистить">

    <!-- date to -->
    <!--{assign var="input_name" value="_input_date_to"}-->
    По: <input type="text" name="<!--{$input_name}-->" value="<!--{$vars.date_to}-->"/>
    <img id="id<!--{$input_name}-->_ClearBtn" src="/-js/date-picker/images/clear_btn.png" alt="clear" class="common-link" title="очистить">

    <!--{if isset($dr_extra_filter_tpl)}-->
        <!--{include file=$dr_extra_filter_tpl}-->
    <!--{/if}-->
    <input type="submit" value="отправить">
</div>
<!-- = = = = = = [ /date range filter automatic/ ] = = = = = = -->
<!--{include file="common:undertoolbar/_common/jquery_ui_timepicker_trent.tpl"}-->

<script type="text/javascript">
$(function(){
    $('input[name=_input_item_time]').timepicker({
        showSecond: true,
       	timeFormat: 'HH:mm:ss',
        duration: ''
        //,stepSecond: 5
    });
});
</script>
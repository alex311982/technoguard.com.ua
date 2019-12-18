<!-- support for ui timepicker -->
<!--{include file="common:undertoolbar/_common/jquery_ui_timepicker_trent.tpl"}-->
<script type="text/javascript">
$(function(){
    $('input[name=_input_item_time]').timepicker({
        showSecond: true,
       	timeFormat: 'HH:mm:ss',
        duration: '',
        stepSecond: 1
    });
});
</script>

<!-- mark bank progs and presenters with enhanced select - chosen -->
<script type="text/javascript">
    $(function(){
        $("#idPresenterConn").chosen({});

        $("select[name=_input_prog_id]").chosen();
    });
</script>


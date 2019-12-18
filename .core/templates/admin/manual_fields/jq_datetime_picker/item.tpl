<!--{assign var="input_name" value="_input_`$this_field_name`"}-->
<!--{*pa a=$this_field_full*}-->

<!--{if !isset($this_field_full.show_time) || $this_field_full.show_time == true}-->
    <!--{assign var="dtp_show_time" value="true"}-->
<!--{else}-->
    <!--{assign var="dtp_show_time" value="false"}-->
<!--{/if}-->
<script type="text/javascript">
$(function() {
    var dtpSettings = {
        duration: 0,
        showTime: <!--{$dtp_show_time}-->,
        stepMinutes:5,
        constrainInput: false,
        time24h: true,
        changeMonth: true,
        changeYear: true,
        showOn: 'both',
        buttonImage: '/-js/date-picker/images/calendar.gif',
        buttonImageOnly: true
    };
    $('input[name=<!--{$input_name}-->][type=text]').datepicker(dtpSettings);
    $("#id<!--{$name}-->_ClearBtn").click(function(){
        if(confirm("Уверены?")){
            $('input[name=_input_<!--{$name}-->]').val("");
        }
    });
});
</script>
<input<!--{if $this_field_readonly}--> readonly<!--{/if}-->  type="text"
    value="<!--{$this_field_value}-->" name="<!--{$input_name}-->"
    style="width:200px;"/>
    
<!--{if $this_field_full.show_reset_btn == true}-->
    <a id="id<!--{$name}-->_ClearBtn" class="common-link">очистить</a>
<!--{/if}-->


<script type="text/javascript" src="/-js/jquery.min.js"></script>
<script type="text/javascript" src="/-js/_misc/date_format.js"></script>
<form id="main_form" action="" method="post" enctype="multipart/form-data">
<input type="hidden" name="_input_is_form_submited" />
<input type="hidden" name="MAX_FILE_SIZE" value="50000000">


<script type="text/javascript">
var beginFetchFromId = <!--{$beginFetchFromId}-->;
var intervalHandler;

$(function(){
    $('#idBtn').click(function(){
        checkNewProcessedItems()
        return false ;
    });

    intervalHandler = setInterval("checkNewProcessedItems()", 5000)
})

function checkNewProcessedItems()
{
    $.getJSON(
        '/admin/base/mailDispatch/q_extra_browse.html?task_id=<!--{$task_id}-->',
        {
            doAjaxAction:'fetchNewProcessed',
            beginFrom:beginFetchFromId
        },
        function(backData){
            var now = new Date();
            var strDate = now.format("dd.mm.yy HH:MM:ss");
            var emailContainer = $('#idEmailContainer');
            var tmpBeginFetchFromId = 0;

            if( (tmpBeginFetchFromId = parseInt(backData.beginFetchFromId, 10) ) > 0 ){
                beginFetchFromId = tmpBeginFetchFromId;
            }

            var liBuffer = "", extraItem = null;
            for(var extraItemKey in backData.extra_items){
                extraItem = backData.extra_items[extraItemKey];
                liBuffer += "<li>["+strDate+"] "+extraItem.email+" ["+extraItem.id+"]</li>";
            }
            emailContainer.prepend(liBuffer);
            liBuffer = "";

            if(backData.is_task_finished == 1){
                $('#idFinalRes').html("<h2>Рассылка закончена</h2>");
                clearInterval(intervalHandler);
            }
        }
    )
}
</script>
<!-- = = = = = = [ main ] = = = = = = -->
<div class="main"  id="main-scroll">
	<!--{include file="common:block.errors.tpl"}-->

    <div>
        <a href="/admin/base/mailDispatch/queue_browse.html">назад</a>
    </div>

    <!--
    <input type="submit" value="add" id="idBtn">
     -->

    <div id="idFinalRes"></div>

    <ul id="idEmailContainer">
    <!--{if !empty($extra_items)}-->
    <!--{foreach from=$extra_items item=ei_val}-->
        <li><!--{$ei_val.email}--></li>
    <!--{/foreach}-->
    <!--{/if}-->
    </ul>
</div>
</form>

<style>
#idEmailContainer{
    font: 12px verdana,arial,sans-serif;
}
#idEmailContainer{
    list-style:none;
}
</style>

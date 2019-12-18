<!--{if $no_page_file != 1}-->
    <!--{assign var="_DISPLAY_TYPE" value="3columns"}-->
    <!--{assign_html title = "Управление подписками" }-->
    <!--{include file="common:page.main.tpl" content_tpl_direct_html="common:base/user/page.subscriptions.tpl"}-->
<!--{else}-->

<!-- jgrowl -->
<!--{if $result == 'ok'}-->
    <link rel="stylesheet" type="text/css" href="/-js/jgrowl/jquery.jgrowl.css" />
    <script src="/-js/jgrowl/jquery.jgrowl_minimized.js"></script>
    <script>
    $(function(){
        $('#idjGrowlRes').jGrowl("Изменения сохранены", {  theme:'ekvjGrowlTheme', glue:'after' });
    })
    </script>
<!--{/if}-->
<!-- /jgrowl/ -->

<div class="content-middle3 standart-text centered">
    <h3 class="h3-colored">Управление подписками</h3>
    <br/>

    <form action="/user/cabinet.html?cmd=subscriptions" method="post">
    <table style="padding-left:0px;" width="100%">
    <tr><td>
        <input type="checkbox" name="_input_dispatch_subscriber" value="1" id="idSubcrEmail"
            <!--{if $vars.dispatch_subscriber == 1}-->checked<!--{/if}-->/>
        <label for="idSubcrEmail">Получать email рассылку</label>
    </td></tr>

    <!-- sms -->
    <tr><td>
        <input type="checkbox" name="_input_sms_dispatch_subscriber" value="1" id="idSubcrSms"
            <!--{if $vars.sms_dispatch_subscriber == 1}-->checked<!--{/if}--> />
        <label for="idSubcrSms">Получать SMS рассылку</label>
    </td></tr>

    <tr><td>
        <br/>
        <input type="submit" value="отправить" name="_input_is_form_submited" />
    </td></tr>
    </table>
    </form>
    
    <div style="padding-left:150px;" id="idjGrowlRes"></div>

</div>

<!--{/if}-->

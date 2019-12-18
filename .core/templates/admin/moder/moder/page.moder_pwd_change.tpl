<!--{if !isset($include_self)}-->
<!--{assign_html}-->
<!--{include file="common:page.main_custom.tpl"
    content_tpl = $ekv_resource_name
    include_self = 1
    }-->
    <!--{inner_use do_action='exit'}-->
<!--{/if}-->

<br/><br/>
<form action="" method="post">
    <h3>Введіть новий пароль для модератора:
        <span style="color:red;"><!--{$moder_info.moder_name}--> (<!--{$moder_info.moder_login}-->)</span></h3>
    <input type="text" name="_input_moder_password" />

    <input type="submit" value="зберегти" />
</form>
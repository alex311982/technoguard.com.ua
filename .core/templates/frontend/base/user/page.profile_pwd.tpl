<!--{if $no_page_file != 1}-->
    <!--{* assign var="_DISPLAY_TYPE" value="3columns" *}-->
    <!--{assign_html title = "Профиль. Смена пароля" }-->
    <!--{include file="common:page.main.tpl" content_tpl_direct_html="common:base/user/page.profile_pwd.tpl"}-->
<!--{else}-->

<div class="user-container">
<!--{if $show_success_greeting}-->
    <h3 class="h3-colored">Ваш пароль обновлен</h3>
    <br/>
    <a href="/user/cabinet.html">Вернуться в личный кабинет</a>
<!--{else}-->
	<h3 class="h3-colored">Профиль. Смена пароля</h3>
	<form action="/user/profile.html?pwd_change=1" method="post">
    <ul class="frm frm-user">
        <!-- password -->
        <li>
            <!--{if isset($err.password)}-->
                <div class="err"><!--{$err.password}--></div>
            <!--{/if}-->
            <label>Новый пароль*</label>
            <input name="_input_password" value="<!--{$vars.password}-->" class="text" type="password"/>
        </li>

        <li>
            <!--{if isset($err.password_confirm)}-->
                <div class="err"><!--{$err.password_confirm}--></div>
            <!--{/if}-->
            <label>Повторить пароль*</label>
            <input name="_input_password_confirm" value="<!--{$vars.password_confirm}-->" class="text" type="password"/>
        </li>

        <li class="centered">
            <input type="hidden" name="_input_is_form_submited">
            <input type="submit" value="отправить">
        </li>
    </ul>
    </form>
<!--{/if}-->
</div>

<style>
</style>

<!--{/if}-->

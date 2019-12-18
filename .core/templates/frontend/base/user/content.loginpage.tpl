<div class="content-middle2 user-container">
	<h3 class="h3-colored">Вход пользователей</h3>

	<form action="/user/login.html" method="post">
	<input type="hidden" name="_input_is_form_submited" />
	<input type="hidden" name="_input_return_url" value="<!--{$referer}-->" />

    <!--{if isset($err.global)}-->
        <h3 class="global-warning"><!--{$err.global}--></h3>
    <!--{/if}-->

    <ul class="frm frm-user">
        <!-- email -->
        <li>
            <!--{if isset($err.email)}-->
                <div class="err"><!--{$err.email}--></div>
            <!--{/if}-->
            <label>Email*</label>
            <input name="_input_email" value="<!--{$vars.email|escape}-->" class="text" type="text"/>
        </li>

        <!-- password -->
        <li>
            <!--{if isset($err.password)}-->
                <div class="err"><!--{$err.password}--></div>
            <!--{/if}-->
            <label>Пароль*</label>
            <input name="_input_password" value="" class="text" type="password"/>
        </li>

        <li class="centered">
            <input id="idAutologinFull" name="_input_autologin" value="1" type="checkbox" checked/>
            <label style="display:inline;" for="idAutologinFull">запомнить меня</label>
        </li>

        <li class="centered">
            <input type="submit" class="btn-fancy" value="войти"/>
            <!--
            <button type="submit" class="btn-common">войти</button>
             -->
        </li>
        <li class="centered">
            <a href="/user/forgot.html">забыли пароль</a> |
            <a style="font-size: 16px;" href="https://loginza.ru/api/widget?token_url=http%3A%2F%2Finternet-bilet.ua%2Fuser%2Flogin.html">вход через соцсети</a> |
            <a href="/user/signup.html">регистрация</a>
        </li>
    </ul>

	</form>
						
</div>
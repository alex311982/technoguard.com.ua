<div class="content-middle2">
<div class="standart-text global-warning">
    Для завершения авторизации, введите Ваш <strong>действующий адрес электронной почты.</strong> <br/>
    На этот е-мейл Вы будете получать электронные билеты, а также уведомления о заказах и оплатах
    <br/>
</div>
<br/>
<div class="user-container">

	<form action="/user/login.html?ask_email=1" method="post">
	<input type="hidden" name="_input_is_form_submited" />

    <ul class="frm frm-user">
        <!-- email -->
        <li>
            <!--{if isset($err.email)}-->
                <div class="err"><!--{$err.email}--></div>
            <!--{/if}-->
            <label>Email*</label>
            <input name="_input_email" value="<!--{$vars.email|escape}-->" class="text" type="text"/>
        </li>

        <li class="centered">
            <input type="submit" value="отправить"/>
        </li>
    </ul>

	</form>
</div>
</div>
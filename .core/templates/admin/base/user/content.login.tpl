<!--{include file="common:block.errors.tpl"}-->
<form method="post" action="" id="main-scroll">
	<div align="center" style="margin-top:50px;">
    <table cellspacing="3" cellpadding="0">
        <!--{if isset($error)}-->
		<tr>
		    <td colspan="2" style="color:red; text-align:center;">
		        <!--{if $error == "login"}-->
		            Неправильный логин/пароль
		        <!--{elseif $error == "ip"}-->
		            Вы не можете ввойти с этого ip: <!--{$current_ip}-->
		        <!--{/if}-->
            </td>
		</tr>
		<!--{/if}-->
		<tr style="height:20px;">
			<td align="right">Логин:</td>
            <td>
                <input id="idEkvLogin" type="text" name="_input_login" value="<!--{$smarty.request.login|escape}-->" />
            </td>
		</tr>
		<tr style="height:30px;">
			<td align="right">Пароль:</td><td><input type="password" name="_input_password" value="" /></td>
		</tr>
		<tr>
			<td></td><td style="vertical-align:middle;"><input class="button button-login" type="submit" value="Войти" alt="login" /></td>
		</tr>
    </table>
	</div>
</form>
<script type="text/javascript">
(function(){
    var h = document.getElementById("idEkvLogin");
    if(h){ h.focus(); }
}());
</script>
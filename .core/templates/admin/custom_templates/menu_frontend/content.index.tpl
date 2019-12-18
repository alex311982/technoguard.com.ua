<h2>Верхнє меню</h2>
<ul class="nulled">
<!--{foreach from=$top_menu item='tval'}-->
    <li>
        <a href="<!--{$tval.href}-->"><!--{$tval.name}--></a>
    </li>
<!--{/foreach}-->
</ul>


<h2>Проміжне меню</h2>
<ul class="nulled">
<!--{foreach from=$middle_menu item='mval'}-->
    <li>
        <a href="<!--{$mval.href}-->"><!--{$mval.name}--></a>
    </li>
<!--{/foreach}-->
</ul>
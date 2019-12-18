<!--{*<!--{pa param1=$cell_info.data}-->*}-->

<ul class="nulled" id="manTranslLangs">
<!--{foreach from=$cell_info.data.langs item='lval'}-->
    <li class="<!--{if $lval.has_translation}-->existed<!--{/if}-->">
        <!--{assign var="_is_current" value=""}-->
        <!--{if $lval.is_current}-->
            <!--{assign var="_is_current" value=" *"}-->
        <!--{/if}-->
        <!--{if $lval.has_access}-->
            <a href="<!--{$lval.edit_url}-->"><!--{$lval.lang_code}--><!--{$_is_current}--></a>
        <!--{else}-->
            <!--{$lval.lang_code}--><!--{$_is_current}-->
        <!--{/if}-->

    </li>
<!--{/foreach}-->
</ul>
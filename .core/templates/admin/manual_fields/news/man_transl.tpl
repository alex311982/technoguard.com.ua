<!--{*<!--{pa param1=$cell_info.data}-->*}-->

<!--{* @deprecated *}-->

<ul class="news-browse nulled">
<!--{foreach from=$cell_info.data item='data_val'}-->
<li>
    <!--{assign var="lnk_class" value="create"}-->
    <!--{if $data_val.has_translation}-->
        <!--{assign var="lnk_class" value="edit"}-->
    <!--{/if}-->

    <!--{* when user has access - show edit links, when doesn't - no liks just lang name *}-->
    <!--{if $data_val.has_access}-->
        <a class="action-lnk <!--{$lnk_class}-->" href="<!--{$data_val.edit_url}-->"
            ><!--{$data_val.lang_name|escape}--><!--{if !$data_val.has_translation}--> [створити]<!--{/if}--></a>

        <!--{if $data_val.has_translation}-->
            <div class="news-title">
                <!--{$data_val.news_title}-->
                <a href="<!--{$data_val.edit_url}-->">[редагувати]</a>
            </div>
        <!--{/if}-->
    <!--{else}-->
        <div class="action-lnk"><!--{$data_val.lang_name|escape}--></div>
        <!--{if $data_val.has_translation}-->
            <div class="news-title">
                <!--{$data_val.news_title}--> [створено]
            </div>

        <!--{/if}-->
    <!--{/if}-->
    <div class="spacer"></div>
</li>
<!--{/foreach}-->
</ul>

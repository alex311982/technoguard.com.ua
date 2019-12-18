<!--{*<!--{pa param1=$cell_info.data}-->*}-->
<ul class="nulled man-trans-date">
    <li <!--{if $cell_info.data.date_created.mark_unpublished}-->class="unpublished"<!--{/if}-->>
        ств. <!--{$cell_info.data.date_created.value}-->
    </li>
    <li>
        пуб. <!--{$cell_info.data.date_pub.value}-->
    </li>
    <!--{if $cell_info.data.date_closed.value}--><li>
        знт. <!--{$cell_info.data.date_closed.value}-->
    </li><!--{/if}-->
</ul>
<!--{if $news_now_item.has_audio}-->
<div class="listen">
    <a onclick="ekvPopup.popupArchive('/schedule/play-archive.html?periodItemID=<!--{$news_now_item.id_item}-->');"><h1 class="marked">
            <span><!--{$news_now_item.channel_name}--></span>
            <!--{$news_now_item.item_time}-->
        </h1>
        <p><!--{$news_now_item.prog_name_display|escape}--></p>
    </a>
</div>
<!--{/if}-->
<hr/>
<!--{if !empty($__ether_now_items)}-->
<!--{foreach from=$__ether_now_items item='air_channel_val'}-->
    <!--{assign var="period_item_listen_val" value=$air_channel_val.period_items.0}-->
    <div class="listen">
        <a onclick="ekvPopup.popupLive('<!--{$air_channel_val.url_popup_live}-->')"><h1 class="marked">
            <span><!--{$air_channel_val.channel_name}--></span>
            <!--{$period_item_listen_val.item_time}-->
        </h1>
   		<p><!--{$period_item_listen_val.prog_name_display|escape}--></p>
        </a>
   	</div>
   	<hr/>
   	
<!--{/foreach}-->

<div style="margin-bottom:15px;">
<a href="https://kazky.suspilne.media">
<img src="/images/_kazky.jpg" width="100%" alt="казки" />
</a>
<a href="https://classical.suspilne.media">
<img src="/images/_classical.jpg" width="100%" alt="казки" />
</a>
</div>
<!--{/if}-->
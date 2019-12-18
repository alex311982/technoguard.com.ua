<!--{assign var="sp_title" value=$sp_info.sp_title|escape}-->
<!--{if !isset($include_self)}-->
    <!--{assign_html title = $sp_title}-->
    <!--{include file="common:page.main.tpl"
        content_tpl=$ekv_resource_name
        include_self = 1
        }-->
        <!--{inner_use do_action='exit'}-->
<!--{/if}-->


<div class="news-preview">
    <h1 class="n-title"><!--{$sp_title}--></h1>

<!--{*
    <!-- social likes -->
    <!--{if $__LIVE_VERSION}-->
    <ul class="nulled social">
        <!--{assign var="_social_url" value=$sp_info.url_preview}-->
        <!-- fb -->
        <li class="item">
            <div id="fb-root" ></div>
            <script>(function(d, s, id) {
              var js, fjs = d.getElementsByTagName(s)[0];
              if (d.getElementById(id)) return;
              js = d.createElement(s); js.id = id;
              js.src = "//connect.facebook.net/en_US/all.js#xfbml=1";
              fjs.parentNode.insertBefore(js, fjs);
            }(document, 'script', 'facebook-jssdk'));</script>

            <div class="fb-like" data-width="450" data-layout="button_count" data-show-faces="true" data-send="false"
                 data-href="<!--{$_social_url}-->"
            ></div>
        </li>

        <!-- twitter -->
        <li class="item twitter">
            <a href="https://twitter.com/share" class="twitter-share-button"
                data-url="<!--{$_social_url}-->"
                data-text="" data-lang="uk">Твіт</a>
            <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>
        </li>
    </ul>
    <!--{/if}-->
*}-->

    <div class="clear"></div>
    <!--{if !empty($sp_info.url_img)}-->
        <img src="<!--{$sp_info.url_img}-->" alt="<!--{$news_title}-->" class="img"<!--{if !empty($sp_info.photo_width)}--> style="width:<!--{$sp_info.photo_width}-->px;"<!--{/if}-->/>
    <!--{/if}-->

    <div>
        <!--{eval var=$sp_info.text_long}-->
    </div>

    <!--{if !empty($tags_related)}-->
    <div class="spacer"></div>
    <ul class="tagit tagit-read">
    <li class="init">
        Теги:
    </li>
    <!--{foreach from=$tags_related item='tag_val'}-->
        <li class="tagit-choice tagit-choice-editable">
            <span class="tagit-label"><a href="<!--{$tag_val.url}-->"><!--{$tag_val.tag_name|escape}--></a></span>
        </li>
    <!--{/foreach}-->
    </ul>
    <!--{/if}-->

<!--{*
    <!--{if !empty($related_news)}-->
        <div class="clear"></div>
        <h1 class="page-title">Подібні новини</h1>
        <ul class="nulled n-related">
            <!--{foreach from=$related_news item='rn_val'}-->
                <li>
                    <strong><!--{$rn_val.date_formatted}--></strong>
                    <a href="<!--{$rn_val.url_preview}-->"
                       class="<!--{$rn_val.css_classes}-->"><!--{$rn_val.news_title|escape}--></a>
                </li>
            <!--{/foreach}-->
        </ul>
    <!--{/if}-->
 *}-->

</div>

<link rel="stylesheet" href="/_js/tag-it/jquery.tagit.css"/>

<!--{if !empty($result.records)}-->

<table>
    <tbody>
    <!--{foreach from=$result.records item='news_item'}-->
        <tr>
            <td>
                <img alt="" src="<!--{$news_item.url_img}-->" style="width: 200px; height: 150px;"></td>
            <td>
                <p style="text-align: justify;">
                    <a href="<!--{$news_item.url_preview}-->" target="_blank">
                        <span style="font-family: Tahoma, Geneva, sans-serif;"><span style="font-size: 14px;"><strong><!--{$news_item.news_title}--></strong></span></span>
                    </a>
                </p>
                <p style="text-align: justify;">
                    <span style="font-family: Tahoma, Geneva, sans-serif;"><span style="font-size: 14px;"><em><!--{$news_item.lid}--></em></span></span>
                </p>
            </td>
        </tr>
    <!--{/foreach}-->
    </tbody>
</table>

<!--{include file="common:block.pagination.tpl"}-->

<!--{/if}-->
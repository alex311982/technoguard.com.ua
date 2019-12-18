<script src="/_js/tag-it/tag-it.js" type="text/javascript" charset="utf-8"></script>
<link href="/_js/tag-it/jquery.tagit.css" rel="stylesheet" type="text/css">

<script type="text/javascript">
    var newsLangID = '<!--{$currentTcLangID}-->';

    $(function(){
        $("select[name=_input_lang_id]").change(function(){
            newsLangID = $(this).val()
        }).change();

        //-------------------------------
        // Allow spaces without quotes.
        //-------------------------------
        $('#idNewsTags').tagit({
            //availableTags: sampleTags,
            autocomplete:{
                /**
                 * This complication is done to support lang switch via dropdown on super page
                 * @param req
                 * @param callBack
                 */
                source: function(req, callBack){
                    //http://stackoverflow.com/questions/18769001/jquery-autocomplete-source-from-another-js-function
                    $.getJSON(
                        "/admin/news/news/AJAX_tags.html",
                        {
                            langID: newsLangID,
                            term: req.term
                        },
                        function(data){
                            callBack(data);
                        }
                    );
                },
                //source: "/admin/news/news/AJAX_tags.html?langID="+newsLangID,

                minLength: 2,
                select: function( event, ui ) {
                    //console.log(ui);
//                    log( ui.item ?
//                    "Selected: " + ui.item.value + " aka " + ui.item.id :
//                    "Nothing selected, input was " + this.value );
                }

            },
            fieldName: "_input_man_tags",
            removeConfirmation: true,
            caseSensitive: false,
            singleField: true,
            singleFieldDelimiter: ',',
            allowSpaces: true
        });
    });
</script>

<ul id="idNewsTags">
<!--{if !empty($this_field_full.data.tags)}-->
<!--{foreach from=$this_field_full.data.tags item='tag_val'}-->
    <li>
        <!--{$tag_val}-->
    </li>
<!--{/foreach}-->
<!--{/if}-->
</ul>

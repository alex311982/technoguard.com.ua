<!--{add_debug_point value="template_head_start"}-->
<head>
	<title><!--{$html_title}--></title>
	<style media="all" type="text/css">@import "/_css/main.css";</style>

	<meta name="keywords" content="<!--{$html_keywords}--> " />
	<meta name="description" content="<!--{$html_description}--> " />
	<meta http-equiv="content-type" content="text/html; charset=<!--{$smarty.const.SITE_HTML_CHARSET}-->" />
	<link rel="shortcut icon" href="/favicon.ico" />
    <script type="text/javascript">
        //--- make code still working with console (without console support) ---//
        var fakeConsole = function(){
           if (typeof console === "undefined" || typeof console.log === "undefined") {
               console = {};
               console.log = function () {};
               console.dir = function () {};
           }
        }
        fakeConsole();
    </script>
	<script type="text/javascript" src="/_js/functions.js"></script>
	<script type="text/javascript" src="/_js/layout.js"></script>
	<script type="text/javascript" src="/_js/menu.js"></script>
	<script type="text/javascript" src="/_js/grid.js"></script>
	<script type="text/javascript" src="/_js/debug.js"></script>
	
	<!-- new CKEditor -->
	<script type="text/javascript" src="/ckeditor/ckeditor.js"></script>
	<script type="text/javascript" src="/ckeditor/_ckfinder/ckfinder.js"></script>
	<!-- jquery -->
	<script type="text/javascript" src="/-js/jquery.min.js"></script>


	<!-- jq datepicker -->
	<script type="text/javascript" src="/-js/jquery.ui.custom.min.js"></script>
	<!--{if !isset($_DATEPICKER_DATE_ONLY) || $_DATEPICKER_DATE_ONLY == false}-->
    <script type="text/javascript" src="/-js/date-picker/timepicker.js"></script>
    <!--{/if}-->
    <script type="text/javascript" src="/-js/date-picker/ui.datepicker-uk.js"></script>
    <link type="text/css" rel="stylesheet" href="/-js/date-picker/jquery.ui.custom.css" />

    <!-- date picker global settings -->
    <script type="text/javascript">
        var dtpSettingsGlobal = {
            duration: '',
            showTime: true,
            stepMinutes:5,
            constrainInput: false,
            time24h: true,
            showOn: 'both',
            buttonImage: '/-js/date-picker/images/calendar.gif',
            buttonImageOnly: true,
            changeMonth: true,
            changeYear: true
        };
    </script>
	<!-- /jq datepicker/ -->

	<!--{if $JS_CHOSEN_USE}-->
        <script type="text/javascript" src="/-js/chosen/chosen.jquery.min.js"></script>
        <link rel="stylesheet" href="/-js/chosen/chosen.css" />
	<!--{/if}-->


	<!--{*<script type="text/javascript">
		<!--{include file="common:js/functions.js"}-->
	</script>*}-->
    <script type="text/javascript" src="/-js/popup.js"></script>

</head>
<!--{add_debug_point value="template_head_end"}-->
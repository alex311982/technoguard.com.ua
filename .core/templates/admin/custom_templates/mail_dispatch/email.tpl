<html>
<head>
    <title><!--{$smarty.const.SITE_TITLE_FRONTEND}--></title>
    <META  http-equiv="Content-Type" content="text/html; charset=UTF-8">
</head>
<body>
    <!--{assign var="site_host_full" value="http://`$smarty.const.SITE_HOST`"}-->
    <!--{assign var="background_header_image" value="`$site_host_full`/files/mail_dispatch_static/background.png"}-->
    <!--{assign var="logo_header_image" value="`$site_host_full`/files/mail_dispatch_static/logo.png"}-->

    <div>
        <!--{$email_vars.message_body}-->
    </div>

</body>
</html>

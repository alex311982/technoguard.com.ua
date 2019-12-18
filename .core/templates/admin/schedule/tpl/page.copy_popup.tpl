<html>
<head>
    <title>Копіювання шаблону</title>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
    <link type="text/css" rel="stylesheet" href="/_css/main.css" />
    <script type="text/javascript" src="/-js/jquery.min.js"></script>
    <script type="text/javascript" src="/-js/jquery.ui.custom.min.js"></script>
    <script type="text/javascript" src="/-js/date-picker/ui.datepicker-uk.js"></script>
    <link type="text/css" rel="stylesheet" href="/-js/date-picker/jquery.ui.custom.css" />
    <script type="text/javascript">
        $(function ()
        {
            var dtpSettings = {
                duration:0,
                showTime:true,
                stepMinutes:5,
                constrainInput:false,
                time24h:true,
                showOn:'both',
                buttonImage:'/-js/date-picker/images/calendar.gif',
                buttonImageOnly:true,
                changeMonth:true,
                changeYear:true
            };
            $('input[name=date_schedule][type=text]').datepicker(dtpSettings);

        });
    </script>
</head>
<body>

<!--{include file="common:block.errors.tpl"}-->

<!--{if !empty($tpl_info)}-->

<h2>Канал: '<!--{$tpl_info.ch_name}-->' Шаблон: '<!--{$tpl_info.tpl_name}-->'</h2>
<p>Оберіть дату для сітки мовлення</p>

<form method="post">
    <label>Дата
        <input type="text" name="date_schedule"/>
    </label>

    <br><br>
    <input type="submit" value="копіювати" />
</form>

<!--{/if}-->

</body>
</html>
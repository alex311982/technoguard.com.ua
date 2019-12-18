<div class="date-picker">
	<!--{if !isset($start_year)}--><!--{assign var=start_year value="-5"}--><!--{/if}-->
	<!--{html_select_date
			field_order		= "`$smarty.const.DATE_PICKER_ORDER`"
			time			= "`$date_value`"
			field_array		= "_input_`$date_name`"
			display_days	= "`$attrs.display_days`"
			day_extra		= 'class="day"'
			display_months	= "`$attrs.display_months`"
			month_extra	= 'class="month"'
			year_extra	= 'class="year"'
			display_years	= "`$attrs.display_years`"
			start_year		= $start_year
			end_year		= "+5"}-->
	<!--{html_select_time
			display_seconds	=	false
			minute_interval	=	1
			use_24_hours	= 	true
			hour_extra = 'class="hours"'
			minute_extra = 'class="minutes"'
			field_array		= 	"_input_`$date_name`_time"
			time			= 	"`$date_value`"}-->
</div>
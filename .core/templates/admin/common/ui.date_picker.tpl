<div class="date-picker">
	<!--{if !isset($start_year)}--><!--{assign var=start_year value="-5"}--><!--{/if}-->
	<!--{html_select_date
			field_order		= "`$smarty.const.DATE_PICKER_ORDER`"
			time			= "`$date_value`"
			field_array		= "_input_`$date_name`"
			display_days	= "`$attrs.display_days`"
			day_extra		= 'class="day"'
			display_months	= "`$attrs.display_months`"
			month_extra		= 'class="month"'
			display_years	= "`$attrs.display_years`"
			year_extra		= 'class="year"'
			start_year		= $start_year
			end_year		= "+5"}-->
</div>
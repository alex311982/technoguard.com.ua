/* Russian (UTF-8) initialisation for the jQuery UI date picker plugin. */
/* Written by Andrew Stromnov (stromnov@gmail.com). */
jQuery(function($){
	$.datepicker.regional['uk'] = {
		closeText: 'гаразд',
		prevText: '&#x3c;Попер',
		nextText: 'Наст&#x3e;',
		currentText: 'Сьогодні',
		monthNames: ['01','02','03','04','05','06',
		'07','08','09','10','11','12'],
		monthNamesShort: ['01','02','03','04','05','06',
		'07','08','09','10','11','12'],
		dayNames: ['неділя','понеділок','вівторок','середа','четвер',"п'ятниця", 'субота'],
		dayNamesShort: ['нд','пн','вт','ср','чт','пт','сб'],
		dayNamesMin: ['Su','Mo','Tu','We','Th','Fr','Sa'],
		dateFormat: 'dd.mm.yy', firstDay: 1,
		isRTL: false};
	$.datepicker.setDefaults($.datepicker.regional['uk']);
});
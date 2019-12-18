(function ($)
{
    $.fn.emptySelect = function ()
    {
        return this.each(function ()
        {
            if (this.tagName == 'SELECT') this.options.length = 0;
        });
    };

    /**
     *
     * @param optionsDataArray - {name:'xx', value:'xx', 'is_selected': false}
     * @param {Boolean} useSimpleKeyValueFormat - if true -  optionsDataArray must be {4: 'xxxx', '5':'yyy'}
     * @return {*}
     */
    $.fn.loadSelect = function (optionsDataArray, useSimpleKeyValueFormat)
    {
        if(useSimpleKeyValueFormat === undefined){
            useSimpleKeyValueFormat = false;
        }

        return this.emptySelect().each(function ()
        {
            if (this.tagName == 'SELECT') {
                var selectElement = this;
                $.each(optionsDataArray, function (index, optionData)
                {
                    if(useSimpleKeyValueFormat){
                        var nameTxt = optionData;
                        optionData = {
                            name: nameTxt,
                            value: index
                        };
                    }

                    var isOptionSelected = (optionData.is_selected == "y") ? true : false;
                    //alert("optionData.is_selected = " + optionData.is_selected);

                    optionData.name = optionData.name.replace(/&#039;/g, "'");
                    optionData.name = optionData.name.replace(/&quot;/g, '"');

                    /**
                     *new Option([text], [value], [defaultSelected], [selected])
                     *text: String that sets the text of the option
                     *value: String that sets the value attribute of the option
                     *defaultSelected: Boolean that sets whether this option should be the default selected option (same effect as setting the  defaultSelected attribute).
                     *selected: Boolean that sets whether this option should be selected (same effect as setting the selected attribute).
                     */
                    var option = new Option(optionData.name,
                            optionData.value, false, isOptionSelected);
                    if ($.browser.msie) {
                        selectElement.add(option);
                    }
                    else {
                        selectElement.add(option, null);
                    }
                });
            }
        });
    }
})(jQuery);

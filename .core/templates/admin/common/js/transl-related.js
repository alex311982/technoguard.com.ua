var translRelated =
(function(){

    var o = {
        clickedClass: "transl-clicked",
        plusMinusClass: "transl-show-hide",
        minusClass: "transl-show-hide-minus"
    };

    o.tClick = function(clickedObj){
        var jqClickedObj = $(clickedObj);
        //jqClickedObj.toggleClass(o.clickedClass);

        var hadClickedClass = jqClickedObj.hasClass(o.clickedClass);
        var becameOpened = false;

        //--- clicked on minus ---//
        if(hadClickedClass){
            jqClickedObj.removeClass(o.clickedClass);
        }else{
            becameOpened = true;
            //--- remove clicked class from all (possibly previously opened) items ---//
            $("." + o.plusMinusClass).removeClass(o.clickedClass).removeClass(o.minusClass);
            jqClickedObj.addClass(o.clickedClass);
        }

//        var becameOpened = jqClickedObj.hasClass(o.clickedClass);

        if(!becameOpened){
            o.setTranslFieldsContents("", "", "");
        }else{
            var slidingDiv = jqClickedObj.next(".slidingDiv");
            slidingDiv.fadeToggle(200);

            var srcName = slidingDiv.find(".tRelSrcName").html();
            var srcShort = slidingDiv.find(".tRelSrcShort").html();
            var srcFull = slidingDiv.find(".tRelSrcFull").html();

            o.setTranslFieldsContents(srcName, srcShort, srcFull);
        }

        //--- handle +/- buttons ---//
//        jqClickedObj.css({backgroundPosition: '0 ' + (becameOpened ? -18 : 0) + 'px'});
        jqClickedObj.toggleClass(o.minusClass);
    };

    o.setTranslFieldsContents = function(srcName, srcShort, srcFull){
        $("#tRelName").html(srcName);
        $("#tRelShort").html(srcShort);
        $("#tRelFull").html(srcFull);

    };

    return o;
})();

function listenPopup(url, playLive)
{
    if(playLive === true){
        url += "&playLive=1";
    }
    var windowHandler = popupWindow(url, 1000, 400);
    setTimeout(function(){windowHandler.focus();}, 1);
    return windowHandler;
}

function redrawEtherNow(calledFromParam)
{
    var calledFromStr = "";
    if(calledFromParam == 'index'){
        calledFromStr = "&calledFrom="+calledFromParam;
    }

    $.getJSON(
        '/' + EKV_LANG + '/index/ajax.html?doAction=redrawEther'+calledFromStr,
        {},
        function(data){
            $("#idEtherLive").html(data.html_ether_side);

            if(calledFromParam == 'index'){
/*                $("#idFutureGrid").html(data.html_grid_future);*/
//                $("#idAjaxAnn").html(data.html_ann);
            }
        }
    );
}

function redrawEtherNowPeriodical(calledFromParam)
{
    setInterval(function(){
        return redrawEtherNow(calledFromParam);
    }, 30000);
}

var ekvPopup = (function(){

    function _popupWindow(wndURL, wndWidth, wndHeight, popupName)
    {
        if (document.all) var xMax = screen.width, yMax = screen.height;
        else
            if (document.layers) var xMax = window.outerWidth, yMax = window.outerHeight;
            else var xMax = 640, yMax=480;

        var xOffset = (xMax - wndWidth)/2, yOffset = (yMax - wndHeight)/2;

        var popupHandler;
        if(!popupName){
            popupName = "defaultPopupName";
        }

        popupHandler = window.open(wndURL, popupName,'width='+wndWidth+',height='+wndHeight+',screenX='+xOffset+',screenY='+yOffset+',top='+yOffset+',left='+xOffset+',copyhistory=no,directories=no, menubar=no,location=no,scrollbars=yes,resizable=yes');
        popupHandler = window.open(wndURL, popupName,'width='+wndWidth+',height='+wndHeight+',screenX='+xOffset+',screenY='+yOffset+',top='+yOffset+',left='+xOffset+',copyhistory=no,directories=no, menubar=no,location=no,scrollbars=yes,resizable=yes');
        setTimeout(function(){popupHandler.focus();}, 1);

        //popupHandler.focus();

        return popupHandler;
    }

    function _popupArchive(url)
    {
        return _popupWindow(url, 1000, 540);
    }

    function _popupLive(url)
    {
        return _popupWindow(url, 1000, 540);
    }

    return {
        popupArchive: _popupArchive,
        popupLive: _popupLive,
        popupWindow: _popupWindow
    };

}());
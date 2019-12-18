
function popupWindow(wndURL,wndWidth,wndHeight)
{
    if (document.all) var xMax = screen.width, yMax = screen.height;
    else
        if (document.layers) var xMax = window.outerWidth, yMax = window.outerHeight;
        else var xMax = 640, yMax=480;

    var xOffset = (xMax - wndWidth)/2, yOffset = (yMax - wndHeight)/2;

    window.open(wndURL,'','width='+wndWidth+',height='+wndHeight+',screenX='+xOffset+',screenY='+yOffset+',top='+yOffset+',left='+xOffset+',copyhistory=no,directories=no, menubar=no,location=no,scrollbars=yes,resizable=no');
}

// how to use
// initializing..
// <script language="JavaScript" type="text/javascript" src="/js/popup.js"></script>
// executing..
// <a href="#" onClick="popupWindow('/url',100,100);return false;";>popup</a>
// <input type=button onClick="popupWindow('ownpage.php?id=293472983047298347',100,100)">
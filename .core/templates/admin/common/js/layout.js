/*
if (navigator.userAgent.indexOf("Gecko") != -1)
{
	window.addEventListener("load", InitMenu, false);
}
else
{
	window.attachEvent("onload", InitMenu);
}


function InitMenu()
{
	var m = document.getElementById('menu');
	if(m)
	{
		var divs = m.getElementsByTagName('DIV');
		if(divs)
		{
			for(var i = 0; i < divs.length; ++i)
			{
				if('subitems' == divs[i].className)
				{
					divs[i].onMouseOver = Hold(this);
					divs[i].onMouseOut = Hide(this);
				}
			}
		}
	}
}*/


function initLayout()
{
	var content = document.getElementById('main-scroll');
	var footer = document.getElementById('footer');

	var clHeight = getClientHeight();
	var clWidth = getClientWidth();

	if(content && footer)
	{
		content.style.height = (clHeight - getElementY(content) - footer.offsetHeight) - 15 + "px";
	}
}

function getElementX(object) {return getElementC(object, true)}
function getElementY(object) {return getElementC(object, false)}
function getElementC(element, xAxis)
{
	var initialElement = element;
	var c = 0;

	while (element != null)
	{
		c += (xAxis) ? element.offsetLeft : element.offsetTop;
		if (element.style.position == "absolute")
			break;
		else
			element = element.offsetParent;
	}

	var elementWnd = document.window;
	if (!elementWnd) return c;

	if (!elementWnd.frameElement) return c;

	return c + getElementC(elementWnd.frameElement, xAxis);
}


var timer = null;

function ShowSubmenu(node)
{
	if (timer)
	{
		clearTimeout(timer);
		timer = null;
	}
	HideMenu();
	if(node)
	{
		var divs = node.parentNode.getElementsByTagName('DIV');
		if(divs[0])
		{
			divs[0].style.display = 'block';
		}
	}
}

function HideSubmenu(node)
{
	if(node) timer = setTimeout("HideMenu", 100);
}

function HideMenu()
{
	var m = document.getElementById("menu");
	if(m)
	{
		var divs = m.getElementsByTagName("DIV");
		for(var i = 0; i < divs.length; ++i)
		{
			if('subitems' == divs[i].className)
			{
				divs[i].style.display = 'none';
			}
		}
	}
}

function Hold(node)
{
	if(node) node.style.display = 'block';
}

function Hide(node)
{
	if(node) node.style.display = 'none';
}
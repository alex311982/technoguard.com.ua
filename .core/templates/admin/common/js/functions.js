function getClientWidth()
{
	if( typeof( window.innerWidth ) == 'number' )
	{
		//Non-IE
		clWidth = window.innerWidth;
	}
	else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) )
	{
		//IE 6+ in 'standards compliant mode'
		clWidth = document.documentElement.clientWidth;
	}
	else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) )
	{
		//IE 4 compatible
		clWidth = document.body.clientWidth;
	}
	return clWidth;
}

function getClientHeight()
{
	if( typeof( window.innerWidth ) == 'number' )
	{
		//Non-IE
		clHeight = window.innerHeight;
	}
	else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) )
	{
		//IE 6+ in 'standards compliant mode
		clHeight = document.documentElement.clientHeight;
	}
	else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) )
	{
		//IE 4 compatible
		clHeight = document.body.clientHeight;
	}
	return clHeight;
}

function Loading()
{
	var txtl = document.getElementById("txt-loading");
	var clHeight = getClientHeight();
	var clWidth = getClientWidth();
	if(clHeight && clWidth && txtl)
	{
		var h = txtl.style.height;
		var w = txtl.style.width;
		txtl.style.top	= clHeight/2 - 47 + "px";
		txtl.style.left	= clHeight/2 + 100 + "px";
		txtl.style.display = "block";
	}
}

function Loaded()
{
	var l = document.getElementById("loading");
	if(l)
	{
		l.style.display = "none";
	}
}
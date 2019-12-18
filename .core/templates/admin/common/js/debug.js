var debugWindow;

function ShowDebug(itemId)
{
	if(!itemId) return;
	var dbg = document.getElementById(itemId);
	if(dbg && (!debugWindow || debugWindow.closed))
	{
		var debugWindow = open("", "debug_window",
		"resizable=yes, scrollbars=yes, titlebar=no, location=no, menubar=no, toolbar=no, width=800, height=400");
		debugWindow.document.writeln('<TITLE>NRG Debug Console</TITLE><STYLE type="text/css">body {margin:0; padding:4px; font-family:Tahoma,Arial,sans-serif; font-size:12px}</STYLE>');
		debugWindow.document.writeln(dbg.innerHTML);
		debugWindow.focus();
		debugWindow.scrollBy(0, 600)
	}
}
function Go()
{
	var confirm = this.getAttribute("confirm");
	if (confirm)
	{
		if (!showConfirm(confirm))
			return;
	}
	var form = document.getElementById("main_form");
	var	action = this.getAttribute("action");
	var	entity = this.getAttribute("entity");
	var	module = this.getAttribute("module");
	var get    = this.getAttribute("_get");

	var ids = [];
	var t = '';
	var selection = getSelection();
	if(selection)
	{
		for(i=0;i<selection.length;i++)
		{
			ids.push(selection[i].value);
		}
	}

	if(form && action && entity && module)
	{
		form.action = '/admin/' + module + '/' + entity + '/' + action + '.html';
		if(!get) get = "";
		if(ids.length == 1)
			get = "id=" + ids[0] + "&" + get;
		else
			get = "ids=" + ids.join(',')  + "&" + get;
		if(get)
			form.action += '?' + get;
	}
	//alert(form.action);
	form.submit();
}

var toolbarButtons;
var checkboxes;

function initForm()
{
	checkboxes = [];
	toolbarButtons = [];
	var table;
	var form = document.getElementById('main_form');
	if(form)
	{
		var _inputs = form.getElementsByTagName("INPUT");
		for (var i=0; i<_inputs.length; i++)
		{
			table = _inputs[i].parentNode.parentNode.parentNode.parentNode;
			if(_inputs[i].type == "checkbox" && _inputs[i].id != "idSelectAllCheckBoxes")
			{
				_inputs[i].onclick = updateToolbar;
				checkboxes.push(_inputs[i]);
			}
			else if(_inputs[i].type == "button")
			{
				_inputs[i].onclick = Go;
				toolbarButtons.push(_inputs[i]);
			}
		}
	}
}

function updateToolbar()
{
	//alert ("updateToolbar()");
	var selection = getSelection();
	//alert(selection);
	for (var i=0; i<toolbarButtons.length; i++)
	{
		if(toolbarButtons[i].getAttribute("restriction") == "single" && selection.length == 1)
		{
			toolbarButtons[i].disabled = false;
			toolbarButtons[i].className = "";
		}
		else if(toolbarButtons[i].getAttribute("restriction") == "single" && selection.length != 1)
		{
			toolbarButtons[i].disabled = true;
			toolbarButtons[i].className = "disabled";
		}

		if(toolbarButtons[i].getAttribute("restriction") == "multi" && selection.length > 0)
		{
			toolbarButtons[i].disabled = false;
			toolbarButtons[i].className = "";
		}
		else if(toolbarButtons[i].getAttribute("restriction") == "multi" && selection.length == 0)
		{
			toolbarButtons[i].disabled = true;
			toolbarButtons[i].className = "disabled";
		}
	}
}

function showConfirm(confType)
{
	var s = "";
	switch (confType)
	{
		case "delete":
			s = "Підтвердіть видалення відповідних пунктів!";
			break;
		case "dispatch_perform":
			s = "Подтвердите отправку рассылки";
			break;
	}
	return confirm(s);
}

function getSelection()
{
	var result = [];
	if (checkboxes)
	{
		for (var i=0; i<checkboxes.length; i++)
		{
			if (checkboxes[i].checked)
				result.push(checkboxes[i]);
		}
	}
	return result;
}

function allCheckBoxesItemClick(obj)
{
	doCheckAllFormCheckboxes(document.getElementById("main_form"),
			 "__service_checkboxes[]",
		  	obj.checked
		);
}

function doCheckAllFormCheckboxes(oForm, cbName, checked)
{
	//alert("oForm[cbName].length = " + oForm[cbName].length);
	var len = oForm[cbName].length;
	for (var i=0; i < len; i++){
		//alert(oForm[cbName][i].value);
		oForm[cbName][i].checked = checked;
	}
	updateToolbar();
}

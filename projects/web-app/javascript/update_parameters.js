
function update_parameters(values)
{
	console.log('update');

	var ul = this.ul;
	
	for (var i = 0, n = ul.children.length; i < n; i++)
		ul.children.item(0).remove();
	
	var parameters = command_templates[this.selectedIndex].parameters;
	
	var i, n, param, li, label;
	for (i = 0, n = parameters.length; i < n; i++)
	{
		param = parameters[i];
		
		li = document.createElement("li");
		
		label = document.createElement("label");
		label.innerHTML = param.name + ": ";
		li.appendChild(label);

		//added
		//li.appendChild(input).name = "test"
		
		
		switch (param.datatype)
		{
			case "select":
			{
				var select = document.createElement("select");
				
				param.items.forEach((i) => {
					var option = document.createElement("option");
					option.innerHTML = i;
					select.appendChild(option);
				});
				
				select.value = values[param.name] ?? param.value;

				//added
				select.name = param.name;
				
				li.appendChild(select);
				
				break;
			}
			
			case "file":
			{
				var input = document.createElement("input");
				input.type = "file";
				//added
				input.name = param.name;
				li.appendChild(input);
				break;
			}
			
			case "number":
			{
				var input = document.createElement("input");
				input.type = "number";
				input.step = param.step;
				input.min = param.min;
				input.value = values[param.name] ?? param.value;
				input.max = param.max;
				//added
				input.name = param.name;
				li.appendChild(input);
				break;
			}
			
			case "bool":
			{
				var input = document.createElement("input");
				input.type = "checkbox";
				//added
				input.name = param.name;
				input.checked = values[param.name] ?? param.value;
				li.appendChild(input);
				break;
			}
			
			case "string":
			{
				var input = document.createElement("input");
				input.type = "text";
				input.value = values[param.name] ?? param.value;
				//added
				input.name = param.name;
				li.appendChild(input);
				break;
			}
		}
		
		ul.appendChild(li);
	}
}












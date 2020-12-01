
function myonload()
{
	console.log("onload")
	
	var commands = document.getElementById("commands");
	
	initial_commands.forEach((command) => 
	{
		var li = document.createElement("li");
		
		li.classList.add("ui-state-default");
		
		var select = document.createElement("select");
		var ul = select.ul = document.createElement("ul");
		
		ul.classList.add("parameter");
		
		select.onchange = select_onchange;
		select.update_parameters = update_parameters;
		
		command_templates.forEach((c) =>
		{
			var option = document.createElement("option");
			option.innerHTML = c.name;
			select.appendChild(option);
		});
		
		select.value = command.command;
		//maybe a name chage at some point
		select.name = "type"
		select.update_parameters(command);
		
		li.appendChild(select);
		li.appendChild(ul);

		commands.appendChild(li);
	});
	
	$( "#commands" ).sortable();
	$( "#commands" ).disableSelection();
}











































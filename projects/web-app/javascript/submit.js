
function submit(e)
{
	console.log("click!");

	input = $('form').serializeArray();

	var commands = {};
	var obj = [];
	var location = 0;
	var name;

	for (const i in input)
	{
		var item = input[i];
		
		if (item.name == "type")
		{
			switch (item.value)
			{
				case "set":
					set(input, obj, i);
					break;
				
				case "option":
					option(input, obj, i);
					break;
				
				case "mode":
					mode(input, obj, i);
					break;
				
				case "line":
					line(input, obj, i);
					break;
				
				case "region":
					region(input, obj, i);
					break;
				
				case "boundary":
					boundary(input, obj, i);
					break;
				
				case "initialize":
					initialize(input, obj, i);
					break;
				
				case "diffuse":
					diffuse(input, obj, i);
					break;
				
				case "structure":
					structure(input, obj, i);
					break;
				
				case "deposit":
					deposit(input, obj, i);
					break;
				
				case "implant":
					implant(input, obj, i);
					break;
				
				default:
					alert('whoops!');
					break;
			}
		}
	}
	
	console.log(input);
	console.log(obj);
	console.log(JSON.stringify(obj));
	
	var stdout_textarea  = document.getElementById('stdout');
	var strdata_textarea = document.getElementById('strdata');
	
	 $.ajax({
		type: "POST",
		url: 'suprem.py',
		data: { input: JSON.stringify(obj) },
		success: function(data)
		{
			console.log(data);
			
			if (data.error != undefined)
				alert("python had an error!");
			else if (data.exitcode != 0)
				alert("suprem had an error!");
			else
			{
				stdout_textarea.innerHTML = data.stdout;
				
				if (Object.keys(data.outfiles).length >= 1)
				{
					var strdata = data.outfiles[Object.keys(data.outfiles)[0]];
					strdata_textarea.innerHTML = strdata;
					update_chart(strdata);
				}
			}
		}
	 });

}

function reset(e) {
	console.log("click reset!")
	initial_commands = durbin_1;
	document.getElementById("commands").innerHTML = ""
	myonload();
}

function ex_one(e){
	console.log("click one!");
	initial_commands = ex_one_commands;
	document.getElementById("commands").innerHTML = ""
	myonload();
}

function ex_two(e){
	console.log("click two!")
	initial_commands = ex_two_commands;
	document.getElementById("commands").innerHTML = ""
	myonload();
}

function ex_three(e){
	console.log("click three!")
	initial_commands = ex_three_commands;
	document.getElementById("commands").innerHTML = ""
	myonload();
}

//maybe later
// function ex_five(e){
// 	console.log("click five!")
// 	initial_commands = ex_five_commands;
// 	document.getElementById("commands").innerHTML = ""
// 	myonload();
// }


function set(input, obj, i)
{
	var noexecute = false;
	var echo = false;
	var location = i;
	location++;
	var item = input[location];
	if (item.name == "echo") {
		noexecute = false;
		echo = true;
	}
	else if (input[location].name == "noexecute") {
		noexecute = true;
		location++;
		if (input[location].name == "echo") {
			echo = true;
		}
	}


	obj.push({
		"command": "set",
		"noexecute": noexecute,
		"echo": echo
	});

}

function option(input, obj, i)
{
	var location = i;
	location++;
	var item = input[location];
	obj.push({
		"command": "option",
		"print-level": item.value
	});

}

function mode(input, obj, i)
{
	var location = i;
	location++;
	var item = input[location];
	obj.push({
		"command": "mode",
		"dim": item.value
	});

}

function line(input, obj, i)
{
	i++;
	var direction = input[i++];
	var location_input = input[i++];
	var spacing = input[i++];
	var tag = input[i++];
	obj.push({
		"command": "line",
		"direction": direction.value,
		"location": parseFloat(location_input.value),
		"spacing": parseFloat(spacing.value),
		"tag": tag.value
	});
}

function region(input, obj, i)
{
	i++;
	var compound = input[i++];
	var xlo = input[i++];
	var xhi = input[i++];
	var ylo = input[i++];
	var yhi = input[i++];
	var zlo = input[i++];
	var zhi = input[i++];

	obj.push({
		"command": "region",
		"compound": compound.value,
		"xlo": xlo.value,
		"xhi": xhi.value,
		"ylo": ylo.value,
		"yhi": yhi.value,
		"zlo": zlo.value,
		"zhi": zhi.value
	});

}

function boundary(input, obj, i)
{
	i++;
	var operation = input[i++];
	var code = input[i++];
	var xlo = input[i++];
	var xhi = input[i++];
	var ylo = input[i++];
	var yhi = input[i++];
	var zlo = input[i++];
	var zhi = input[i++];

	obj.push({
		"command": "boundary",
		"operation": operation.value,
		"code": code.value,
		"xlo": xlo.value,
		"xhi": xhi.value,
		"ylo": ylo.value,
		"yhi": yhi.value,
		"zlo": zlo.value,
		"zhi": zhi.value
	});

}

function initialize(input, obj, i)
{
	i++;
	var flip_bool = false;
	var conc = input[i++];
	var element = input[i++];
	var interval_r = input[i++];
	var orientation = input[i++];
	var flip_y = input[i++];
	if (flip_y.name == "flip.y")
	{
		flip_y = true;
		location++;
		var scale = input[location];
	} else {
		scale = flip_y;
	}

	obj.push({
		"command": "initialize",
		"conc": parseFloat(conc.value),
		"element": element.value,
//		"interval.r": interval_r.value,
//		"orientation": orientation.value,
//		"flip.y": flip_bool,
//		"scale": scale.value
	});

}

function diffuse(input, obj, i)
{
	i++;
	var cont_bool = false;
	var time = input[i++];
	var temp = input[i++];
	var cont = input[i++];
	if (cont.name == "cont")
	{
		cont_bool = true;
		var gas = input[i++];
	} else {
		gas = cont;
	}
	var gas_conc = input[i++];
	var pressure = input[i++];
	var gold_surf = input[i++];
	var dump = input[i++];
	var movie = input[i++];

	obj.push({
		"command": "diffuse",
		"time": parseFloat(time.value),
		"temp": parseFloat(temp.value),
		"cont": cont_bool,
		"gas": gas.value,
		"gas.conc": parseFloat(gas_conc.value),
//		"pressure": pressure.value,
//		"gold.surf": gold_surf.value,
//		"dump": dump.value,
//		"movie": movie.value
	});

}

function structure(input, obj, i)
{
	var location = i;
	location++;
	var item = input[location];
	obj.push({
		"command": "structure",
		"outfile": item.value
	});
}

function deposit(input, obj, i)
{
	i++;
	var material = input[i++];
	var thicc = input[i++];
	var implant = input[i];
	
	if (implant.name == "implant")
		i++;
	else
		implant = {value: ""};
	
	var concentrat = input[i++];
	var space = input[i++];
	
	obj.push({
		"command": "deposit",
		"material": material.value,
		"thick": parseFloat(thicc.value),
		"implant": implant.value,
		"concentrat": parseFloat(concentrat.value),
		"space": parseFloat(space.value)
	});
}


function implant(input, obj, i)
{
	i++;
	var impurity = input[i];
	
	if (impurity.name == "impurity")
		i++;
	else
		impurity = {value: ""};
	
	var dose = input[i++];
	var energy = input[i++];
	var model = input[i++];
	
	obj.push({
		"command": "implant",
		"impurity": impurity.value,
		"dose": parseFloat(dose.value),
		"energy": parseFloat(energy.value),
		"model": model.value,
	});
}


















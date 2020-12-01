
var boron = {
	type: 'scatter',
	x: [1, 2, 3, 4],
	y: [2, 4, 8, 16],
	mode: 'lines+markers',
	name: 'Boron Concentration (cm^-3)'
};

var phosphorus = {
	type: 'scatter',
	x: [1, 2, 3, 4],
	y: [2, 4, 8, 16],
	mode: 'lines+markers',
	name: 'Phosphorus Concentration (cm^-3)'
};

var net_dope = {
	type: 'scatter',
	x: [1, 2, 3, 4],
	y: [2, 4, 8, 16],
	mode: 'lines+markers',
	name: 'Net Concentration (cm^-3)',
	marker: {
		color: 'rgb(128, 128, 128)',
		size: 8
	},
	line: {
		color: 'rgb(128, 128, 128)',
		width: 1
	}
};

var data = [boron, phosphorus, net_dope];

var layout_expo = {
	title:'Depth VS Concentration',
	xaxis: {
		title: 'Depth',
		range: [0, 2],
		type: 'scatter',
//		autorange: true
	},
	yaxis: {
		title: 'Concentration',
//		range: [0, 1e20]
		type: 'log',
		autorange: true
	},
};

var layout_norm = {
	title:'Depth VS Concentration (scaled)',
	xaxis: {
		title: 'Depth',
//		range: [0, 2],
		range: [0, 400],
		type: 'scatter',
//		autorange: true
	},
	yaxis: {
		title: 'Concentration',
//		range: [0, 1e18],
		type: 'log',
		autorange: true
	},
};

var config = {responsive: true};
	
function setup_chart()
{
	Plotly.newPlot('chart_expo', data, layout_expo, config);
	Plotly.newPlot('chart_norm', data, layout_norm, config);
};

function update_chart(strdata)
{
	var depth_from_surface = [];
	var boron_concentration = [];
	var phosphorous_concentration = [];
	var net_doping_concentration = [];

	strdata.split("\n").forEach((line) => {
		if (line.startsWith("c")) {
			var args = line.split(/ +/).slice(1).map(parseFloat);
			depth_from_surface.push(args[1]);
		}
		else if (line.startsWith("n")) {
			var args = line.split(/ +/).slice(1).map(parseFloat);
			boron_concentration.push(args[2]);
			phosphorous_concentration.push(args[4]);
			net_doping_concentration.push(args[4] - args[2]);
		}
	});
	
	boron.x = depth_from_surface;
	boron.y = boron_concentration;
	
	phosphorus.x = depth_from_surface;
	phosphorus.y = phosphorous_concentration;
	
	net_dope.x = depth_from_surface;
	net_dope.y = net_doping_concentration;
	
	Plotly.react('chart_expo', data, layout_expo);
	Plotly.react('chart_norm', data, layout_norm);
	
};





















var elements = [
	"boron", "antimony", "arsenic", "phosphorus", "gold",
	"gallium", "beryllium", "magnesium", "selenium", "isilicon",
	"tin", "germanium", "zinc", "carbon", "generic"
];

var bounds = [
	{name: "xlo", datatype: "string", value: ""},
	{name: "xhi", datatype: "string", value: ""},
	{name: "ylo", datatype: "string", value: ""},
	{name: "yhi", datatype: "string", value: ""},
	{name: "zlo", datatype: "string", value: ""},
	{name: "zhi", datatype: "string", value: ""},
];

var command_templates = [
	{name: "(do nothing)", parameters: []},
	
	{name: "echo", parameters: [
		{name: "text", datatype: "string", value: "hello!"},
	]},
	
//	{name: "man", parameters: [
//	]},
	
	{name: "initialize", parameters: [
		{name: "infile",      datatype: "file"},
		{name: "conc",        datatype: "number", value: "1.0e16"},
		{name: "element",     datatype: "select", items: elements},
		{name: "interval.r",  datatype: "number", value: "0.0"},
		{name: "orientation", datatype: "select", value: "100", items: ["100", "110", "111"]},
		{name: "flip.y",      datatype: "bool",   value: false},
		{name: "scale",       datatype: "number", value: "0.0"},
	]},
	
//	{name: "cpulog", parameters: [
//	]},
	
//	{name: "plot.2d", parameters: [
//	]},
	
	{name: "option", parameters: [
		{name: "print-level", datatype: "select",
			items: ["quiet", "normal", "chat", "barf"]},
	]},
	
	//added by Chris
	{name: "method", parameters: [
		{name: "impurity", datatype: "select",
			items: ["","vacancies", "interstitial", "arsenic", "phosphorus", "antimony", 
			"boron", "oxidant", "velocity", "traps", "gold", "psi", "cesium", "electron", "hole", "circuit", "gallium", "beryllium", "magnesium",
			 "selenium", "isilicon", "tin", "germanium", "zinc", "carbon", "generic"]},
		
				{name: "rel.error", datatype: "number", value: ""},
				{name: "abs.error", datatype: "number", value: ""},
				{name: "min.fill", datatype: "number", value: ""},
				{name: "min.freq", datatype: "number", value: ""},
				{name: "block", datatype: "select", items: ["","guass", "cg"]},
				{name: "factor", datatype: "select", items: ["","time", "err", "newton"]},
				{name: "fill", datatype: "select", items: ["","diag", "knot", "full.fac"]},
				{name: "back", datatype: "number", value: ""},
				{name: "init.time", datatype: "number", value: ""},
				{name: "timemeth", datatype: "select", items: ["","trbdf", "formula"]},
				{name: "defmodel", datatype: "select", items: ["","fermi", "two.d", "steady", "full.cpl"]},
				{name: "oxide_model", datatype: "select", items: ["","erfc", "erf1", "erf2", "erfg", "vertical", "compress", "viscous"]}

				
	]},
	
	{name: "diffuse", parameters: [
		{name: "time", datatype: "number", value: "24.00"},
		{name: "temp", datatype: "number", value: "1050.00"},
		{name: "cont", datatype: "bool", value: false},
		{name: "gas", datatype: "select",
			items: ["argon", "nitrogen", "amnoina", "dryo2", "weto2",
				"antimony", "arsenic", "boron", "phosphorus", "beryllium",
				"magnesium", "selenium", "isilicon", "tin", "germanium",
				"zinc", "carbon", "generic"]},
		{name: "gas.conc", datatype: "number", value: "1e20"},
		{name: "pressure", datatype: "number", value: "0.00"},
		{name: "gold.surf", datatype: "number", value: "0.00"},
		{name: "dump", datatype: "number", value: "0"},
		{name: "movie", datatype: "string", value: ""},
	]},
	

	//added person?
	{name: "implant", parameters: [
		{name: "impurity", datatype: "select",
			items: ["silicon", "arsenic", "phosphorus", "antimony", "boron", "bf2", "cesium", 
			"beryllium", "magnesium", "selenium", 
			"isilicon", "tin", "germanium", "zinc", "carbon", "generic"]},
		{name: "dose", datatype: "number", value: "1.0e16"},
		{name: "energy", datatype: "number", value: "0"},
		{name: "model", datatype: "select", items:["pearson", "gaussian"]},
		{name: "concentrat", datatype: "number", value: "0.00", step: 0.01}

	]},
	
//	{name: "select", parameters: [
//	]},
	
//	{name: "contour", parameters: [
//	]},
	
//	{name: "plot.1d", parameters: [
//	]},
	
//	{name: "print.1d", parameters: [
//	]},
	
//	{name: "vacancy", parameters: [
//	]},
	
//	{name: "interstitial", parameters: [
//	]},
	
//	{name: "antimony", parameters: [
//	]},
	
//	{name: "arsenic", parameters: [
//	]},
	
//	{name: "boron", parameters: [
//	]},
	
//	{name: "oxide", parameters: [
//	]},
	
//	{name: "phosphorus", parameters: [
//	]},
	
	{name: "region", parameters: [
		{name: "compound", datatype: "select", items: [
			"silicon", "oxide", "oxynitride", "nitride", "poly",
			"aluminum", "photoresist", "gaas"
		]},
	].concat(bounds)},
	
	{name: "boundary", parameters: [
		{name: "operation", datatype: "select", items: [
			"exposed", "backside", "reflecting"
		]},
		{name: "code",  datatype: "number", value: "0.00", step: 1},
	].concat(bounds)},
	
	{name: "line", parameters: [
		{name: "direction", datatype: "select", items: ["x", "y", "z"]},
		{name: "location",  datatype: "number", value: "0.00", step: 0.01},
		{name: "spacing",   datatype: "number", value: "0.00", step: 0.01},
		{name: "tag",       datatype: "string", value: ""},
	]},
	
	{name: "structure", parameters: [
		{name: "outfile", datatype: "string", value: "out.str"},
	]},
	
	//added by chris
	//needs to be checked
	{name: "deposit", parameters: [
		{name: "material", datatype: "select",
			items: ["silicon", "oxide", "oxynitride", "nitride", "poly",
			"aluminum", "photoresist", "gaas"]},
		{name: "thick", datatype: "number", value: "0.00", step: 0.001},
		{name: "implant", datatype: "select",
			items: ["arsenic", "phosphorus", "antimony", "boron", "gallium",
			"beryllium", "magnesium", "selenium", "isilicon", "tin", "germanium", "zinc", "carbon", "generic"]},
		{name: "concentrat", datatype: "number", value: "0.00", step: 0.01},
		{name: "space", datatype: "number", value: "0.00", step: 0.01}

	]},
	
//	{name: "etch", parameters: [
//	]},
	
//	{name: "gold", parameters: [
//	]},
	
//	{name: "stress", parameters: [
//	]},
	
//	{name: "material", parameters: [
//	]},
	
//	{name: "cesium", parameters: [
//	]},
	
//	{name: "pause", parameters: [
//	]},
	
//	{name: "trap", parameters: [
//	]},
	
//	{name: "printf", parameters: [
//	]},
	
	// //added also z?
	// //help
	// {name: "profile", parameters: [
	// 	{name: "select", datatype: ""}
	// ]},
	
	{name: "mode", parameters: [
		{name: "dim", datatype: "select", items: ["one.dim", "two.dim"]},
	]},
	
//	{name: "device", parameters: [
//	]},
	
//	{name: "beryllium", parameters: [
//	]},
	
//	{name: "magnesium", parameters: [
//	]},
	
//	{name: "selenium", parameters: [
//	]},
	
//	{name: "isilicon", parameters: [
//	]},
	
//	{name: "tin", parameters: [
//	]},
	
//	{name: "germanium", parameters: [
//	]},
	
//	{name: "zinc", parameters: [
//	]},
	
//	{name: "carbon", parameters: [
//	]},
	
//	{name: "generic", parameters: [
//	]},

	{name: "set", parameters: [
		{name: "noexecute", datatype: "bool", value: false},
		{name: "echo", datatype: "bool", value: true},
	]},
];


























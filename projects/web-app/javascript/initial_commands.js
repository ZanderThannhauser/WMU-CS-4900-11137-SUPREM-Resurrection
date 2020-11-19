
var initial_commands = [
	{command: "set", noexecute: false, echo: true},
	{command: "option", "print-level": "quiet"},
	{command: "mode", dim: "one.dim"},
	{command: "line", direction: "x", location: "0.0", spacing: "0.01", tag: "top"},
	{command: "line", direction: "x", location: "0.3", spacing: "0.01"},
	{command: "line", direction: "x", location: "0.6", spacing: "0.01"},
	{command: "line", direction: "x", location: "0.9", spacing: "0.01"},
	{command: "line", direction: "x", location: "380", spacing: "100",  tag: "bottom"},
	{command: "region", compound: "silicon", xlo: "top", xhi: "bottom"},
	{command: "boundary", operation: "exposed", xlo: "top", xhi: "top"},
	{command: "initialize", conc: "1.0e16", element: "boron"},
	{command: "diffuse", time: "24.0", temp: "1050", gas: "phosphorus", "gas.conc": "1e20"},
	{command: "structure", out: "out.str"},
];

var ex_one_commands = [
	{command: "set", noexecute: false, echo: true},
	{command: "option", "print-level": "quiet"},
	{command: "mode", dim: "one.dim"},
	{command: "line", direction: "x", location: "0.0", spacing: "0.02", tag: "top"},
	{command: "line", direction: "x", location: "0.50", spacing: "0.02"},
	{command: "line", direction: "x", location: "2.0", spacing: "0.25", tag: "bottom"},
	{command: "region", compound: "silicon", xlo: "top", xhi: "bottom"},
	{command: "boundary", operation: "exposed", xlo: "top", xhi: "top"},
	{command: "initialize", conc: "1.0e14", element: "boron"},
	{command: "deposit", material: "oxide", thick: "0.075"},
	{command: "implant", implant: "boron", dose: "3e14", energy: "70", model: "pearson"},
	{command: "diffuse", time: "30.0", temp: "1100", gas: "phosphorus"},
	{command: "structure", out: "out.str"},
];

var ex_two = [
	{command: "set", noexecute: false, echo: true},
	{command: "option", "print-level": "quiet"},
	{command: "mode", dim: "one.dim"},
	{command: "line", direction: "x", location: "0.0", spacing: "0.02", tag: "top"},
	{command: "line", direction: "x", location: "1.50", spacing: "0.05"},
	{command: "line", direction: "x", location: "5.0", spacing: "0.5"},
	{command: "line", direction: "x", location: "400.0", tag: "bottom"},
	{command: "region", compound: "silicon", xlo: "top", xhi: "bottom"},
	{command: "boundary", operation: "exposed", xlo: "top", xhi: "top"},
	{command: "initialize", conc: "1.0e14", element: "boron"},
	{command: "deposit", material: "oxide", thick: "0.075"},
	{command: "implant", impurity: "boron", dose: "3e14", energy: "70", model: "pearson"},
	{command: "method", "init.time": "1.0e-3", defmodel: "two.d" },
	{command: "diffuse", time: "30.0", temp: "1100", gas: "dryo2"},
	{command: "structure", out: "out.str"},
];



var ex_four = [
	{command: "set", noexecute: false, echo: true},
	{command: "option", "print-level": "quiet"},
	{command: "mode", dim: "one.dim"},
	{command: "line", direction: "x", location: "-2.0", tag: "left"},
	{command: "line", direction: "x", location: "0.0", spacing: "0.05"},
	{command: "line", direction: "x", location: "5.0", spacing: "0.01", tag: "right"},
	{command: "line", direction: "y", location: "0", spacing: "0.02", tag: "top"},
	{command: "line", direction: "y", location: "1.5", spacing: "0.05"},
	{command: "line", direction: "y", location: "400.0", tag: "bottom"},
	{command: "region", compound: "silicon", xlo: "left", xhi: "right", ylo: "top", yhi: "top"},
	{command: "boundary", operation: "exposed", xlo: "top", xhi: "top"},
	{command: "initialize", conc: "1.0e14", element: "boron"},
	{command: "deposit", material: "oxide", thick: "0.02"},
	{command: "deposit", material: "nitride", thick: "0.05"},
	{command: "implant", implant: "boron", dose: "3e14", energy: "40", model: "pearson"},
	{command: "diffuse", time: "30.0", temp: "1100", gas: "dryo2"},
	{command: "structure", out: "out.str"},
];

var ex_five = [
	{command: "set", noexecute: false, echo: false},
	{command: "option", "print-level": "quiet"},
	{command: "mode", dim: "one.dim"},
	{command: "line", direction: "x", location: "0.0", spacing: "0.02", tag: "top"},
	{command: "line", direction: "x", location: "1.50", spacing: "0.05"},
	{command: "line", direction: "x", location: "5.0", spacing: "0.5"},
	{command: "line", direction: "x", location: "28.0", tag: "bottom"},
	{command: "region", compound: "silicon", xlo: "top", xhi: "bottom"},
	{command: "boundary", operation: "exposed", xlo: "top", xhi: "top"},
	{command: "initialize", conc: "1.0e16", element: "boron"},
	{command: "diffuse", time: "24.0", temp: "1050", gas: "phosphorus"},
	{command: "structure", out: "out.str"},
];
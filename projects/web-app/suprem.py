#!/usr/bin/python3

import cgi;
import json;
import string;
import numbers;
import os;
from subprocess import Popen, PIPE, STDOUT;

num = numbers.Number;

from enum import Enum, auto;

#print("Content-Type: text/plain\n");
#print("Content-Type: text/html\n")
print("Content-Type: application/json\n")

form = cgi.FieldStorage();
data = json.loads(form["input"].value);

templates = {}
suprem_input = "";

def set_handler(noexecute, echo):
	global suprem_input;
	suprem_input += f"set {'noexecute ' if noexecute else ''}{'echo' if echo else ''}";

templates['set'] = {
	'noexecute': {'type': bool},
	'echo':      {'type': bool},
};


def option_handler(**args):
	global suprem_input;
	suprem_input += f"option {args['print-level']}";

templates['option'] = {
	'print-level': {'type': str, 'choices': ['quiet', 'normal', 'chat', 'barf']}
};


def mode_handler(dim):
	global suprem_input;
	suprem_input += f"mode {dim}";

templates['mode' ] = {
	'dim': {'type': str, 'choices': ['one.dim', 'two.dim']}
};


def line_handler(direction, location, spacing, tag = None):
	global suprem_input;
	suprem_input += f"line {direction} loc = {location} spacing = {spacing}{' tag = ' + tag if tag else ''}";

templates['line'] = {
	'direction': {'type': str, 'choices': ['x', 'y', 'z']},
	'location':  {'type': num},
	'spacing':   {'type': num},
	'tag':       {'type': str},
};


def region_handler(compound, xlo = "", xhi = "", ylo = "", yhi = "", zlo = "", zhi = ""):
	global suprem_input;
	if xlo != "" and xhi != "":
		suprem_input += f"region {compound} xlo = {xlo} xhi = {xhi}";
	elif ylo != "" and yhi != "":
		suprem_input += f"region {compound} ylo = {ylo} yhi = {yhi}";
	elif zlo != "" and zhi != "":
		suprem_input += f"region {compound} zlo = {zlo} zhi = {zhi}";
	else:
		raise RuntimeError('bad parameters for region!')

templates['region'] = {
	'compound': {'type': str, 'choices': ['silicon', 'oxide', 'oxynitride','nitride', 'poly', 'aluminum', 'photoresist', 'gaas']},
	'xhi':      {'type': str}, 'xlo': {'type': str},
	'yhi':      {'type': str}, 'ylo': {'type': str},
	'zhi':      {'type': str}, 'zlo': {'type': str},
};


def boundary_handler(operation, xlo = "", xhi = "", ylo = "", yhi = "", zlo = "", zhi = ""):
	global suprem_input;
	if xlo != "" and xhi != "":
		suprem_input += f"boundary {operation} xlo = {xlo} xhi = {xhi}";
	elif ylo != "" and yhi != "":
		suprem_input += f"boundary {operation} ylo = {ylo} yhi = {yhi}";
	elif zlo != "" and zhi != "":
		suprem_input += f"boundary {operation} zlo = {zlo} zhi = {zhi}";
	else:
		raise RuntimeError('bad parameters for region!')

templates['boundary'] = {
	'operation': {'type': str, 'choices': ['exposed', 'backside', 'reflecting']},
	'xhi':       {'type': str}, 'xlo': {'type': str},
	'yhi':       {'type': str}, 'ylo': {'type': str},
	'zhi':       {'type': str}, 'zlo': {'type': str},
};


def initialize_handler(conc, element):
	global suprem_input;
	suprem_input += f"initialize conc = {conc:e} {element}";

templates['initialize'] = {
	'conc':    {'type': num},
	'element': {'type': str, 'choices': ['boron', 'antimony', 'phosphorus', 'gold', 'gallium', 'beryllium', 'magnesium', 'selenium', 'isilicon', 'tin', 'germanium', 'zinc', 'carbon', 'generic']},
};


def diffuse_handler(time, temp, gas, **args):
	global suprem_input;
	suprem_input += f"diffuse time = {time} temp = {temp} {gas}";
	if 'gas.conc' in args:
		suprem_input += f" gas.conc = {args['gas.conc']:e}";

templates['diffuse'] = {
	'time': {'type': num},
	'temp': {'type': num},
	'gas': {'type': str, 'choices': ['argon', 'nitrogen', 'ammonia', 'dryo2', 'weto2', 'antimony', 'arsenic', 'boron', 'phosphorus', 'beryllium', 'magnesium', 'selenium', 'isilicon', 'tin', 'germanium', 'zinc', 'carbon', 'generic']},
	'gas.conc': {'type': num},
};

outfiles = [];

def structure_handler(outfile):
	global suprem_input;
	global outfiles;
	suprem_input += f"structure outfile = {outfile}";
	outfiles.append(outfile);

templates['structure'] = {
	'outfile': {'type': str},
};


def deposit_handler(material, thick, implant = None, concentrat = None, space = None):
	global suprem_input;
	suprem_input += f"deposit {material} thick = {thick}";
	if implant:
		suprem_input += f" {implant}";
	if concentrat:
		suprem_input += f" concentrat = {concentrat}";
	if space:
		suprem_input += f" space = {space}";

templates['deposit'] = {
	'material': {'type': str, 'choices': ['silicon', 'poly', 'oxide', 'nitride', 'oxynitride', 'photoresist', 'aluminum', 'gaas']},
	'thick': {'type': num},
	'implant': {'type': str, 'choices': ['arsenic', 'phosphorus', 'antimony', 'boron', 'gallium', 'beryllium', 'magnesium', 'selenium', 'isilicon', 'tin', 'germanium', 'zinc', 'carbon', 'generic']},
	'concentrat': {'type': num},
	'space': {'type': num},
};


def implant_handler(impurity, dose, energy, model = None):
	global suprem_input;
	suprem_input += f"implant {impurity} dose = {dose} energy = {energy}";
	if model:
		suprem_input += f" {model}";

templates['implant'] = {
	'impurity': {'type': str, 'choices': ['silicon', 'arsenic', 'phosphorus', 'antimony', 'boron', 'bf2', 'cesium', 'beryllium', 'magnesium', 'selenium', 'isilicon', 'tin', 'germanium', 'zinc', 'carbon', 'generic']},
	'dose': {'type': num},
	'energy': {'type': num},
	'model': {'type': str, 'choices': ['pearson', 'gaussian']},
};


def method_handler(impurity = None, block = None, factor = None, fill = None,
		back = None, timemeth = None, defmodel = None, oxide_model = None, **etc):
	global suprem_input;
	suprem_input += f"method";
	if (impurity):
		suprem_input += f" {impurity}";
	if ('init.time' in etc):
		suprem_input += f" init.time = {etc['init.time']}";
	if (defmodel):
		suprem_input += f" {defmodel}";
	if (oxide_model):
		suprem_input += f" {oxide_model}";
	if ('grid.oxide' in etc):
		suprem_input += f" grid.oxide = {etc['grid.oxide']}";

templates['method'] = {
	'impurity':    {'type': str, 'choices': ['vacancies', 'interstitial', 'arsenic', 'phosphorus', 'antimony', 'boron', 'oxidant', 'velocity', 'traps', 'gold', 'psi', 'cesium', 'electron', 'hole', 'circuit', 'gallium', 'beryllium', 'magnesium', 'selenium', 'isilicon', 'tin', 'germanium', 'zinc', 'carbon', 'generic']},
	'rel.error':   {'type': num},
	'abs.error':   {'type': num},
	'min.fill':    {'type': num},
	'min.freq':    {'type': num},
	'block':       {'type': str, 'choices': ['guass', 'cg']},
	'factor':      {'type': str, 'choices': ['time', 'err', 'newton']},
	'fill':        {'type': str, 'choices': ['diag', 'knot', 'full.fac']},
	'back':        {'type': num},
	'init.time':   {'type': num},
	'timemeth':    {'type': str, 'choices': ['trbdf', 'formula']},
	'defmodel':    {'type': str, 'choices': ['fermi', 'two.d', 'steady', 'full.cpl']},
	'oxide_model': {'type': str, 'choices': ['erfc', 'erf1', 'erf2', 'erfg', 'vertical', 'compress', 'viscous']},
	'grid.oxide':  {'type': num},
};


valid_string_characters = string.ascii_lowercase + "123456789._";

for c in data:
	command = c['command'];
	# print("command == ", command);
	if (command in templates):
		template = templates[command];
		O = {};
		for k in template.keys():
			if k in c and c[k] != '':
				v = c[k];
				# it better be the right type:
				if not isinstance(v, template[k]['type']):
					raise RuntimeError('incorrect datatype for parameter')
				# If it's a string, it better not have anything bad in it:
				if isinstance(v, str):
					if not all([w in valid_string_characters for w in v]):
						raise RuntimeError('bad string')
				# if there is a limited set of possibilities, it better be
				# on that list:
				if 'choices' in template[k]:
					if not v in template[k]['choices']:
						raise RuntimeError('value not valid option')
				# add to object:
				O[k] = v;
		eval(command+"_handler")(**O);
	else:
		raise RuntimeError('unknown command')
	suprem_input += "\n";

#print("suprem_input:");
#print(suprem_input);
#print("happy.");

f = open('infile', 'w');
f.write(suprem_input);
f.close();

process = Popen(["./suprem", "./infile"], stdout=PIPE, stderr=STDOUT)
(output, err) = process.communicate(timeout=15);
exitcode = process.wait();

O = {'stdout': output.decode("utf-8"), 'exitcode': exitcode, 'outfiles': {}};

#for line in output.split(b'\n'):
#	print(line);

if (exitcode == 0):
	for of in outfiles:
		f = open(of, 'r');
		O['outfiles'][of] = f.read();
		f.close();

print(json.dumps(O));






























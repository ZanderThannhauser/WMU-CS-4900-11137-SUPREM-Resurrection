/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See ../Copyright for complete rights and liability information.
 */

#include "Graph.h" // 2020: for GraphWidget
#include "DataRec.h" // 2020: for GraphWin

XtAppContext app_context;
XtAppContext help_context;
GraphWidget graph;

GraphWin *newwin;

DataSet *curspot;
PointList *curpt;

double xg_xscl;
double xg_yscl;

int newGroup;

Widget global;

int toolkitinit;

Display * dpy;

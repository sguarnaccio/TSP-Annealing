========================================================================
    CONSOLE APPLICATION : Annealing Project Overview
========================================================================

AppWizard has created this Annealing application for you.

This file contains a summary of what you will find in each of the files that
make up your Annealing application.


Annealing.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Annealing.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

Annealing.cpp
    This is the main application source file.

Annealing.h
	This is the main application header file. Contains all the function declarations and structures needed.
	This file also contain some defines used to change the behavior of the algorithm

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Annealing.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

# TP1 Introduccion a Inteligencia Artifical 2017 UTNFRBA - TSP - Recocido simulado

The application recives as argument a .txt file containing the information for the TSP problem and solves it by using simulated annealing.

## Files included

- Annealing.cpp
- Annealing.h
- 15 archivos .txt con datos de distintas configuraciones de ciudades.


### Compiling

For compiling access the folder location on console to proced with the makefile:

	make


On **Windows** without full support for C99 starting from VisualStudio 2015:
inside Annealing.h uncomment
	#define WINDOWS
Then execute on the Developer Commmand Prompt for Visual Studio:

      cl Annealing.cpp


### Running the software

When running the software one of the .txt files containing the information of the TSP problem is needed

Ex:

	./TSP Inputs/TSP_IN_03.txt

On **WINDOWS**
Execute on the Developer Commmand Prompt:

      Annealing.exe Inputs\TSP_IN_01.txt

On screen all the states information and the results of the algorithm applied with some extra information for debuging will be displayed.

/////////////////////////////////////////////////////////////////////////////

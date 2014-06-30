
_____________________________________________________________________________________________


************************ SIMulation to EXPeriment comparison ********************************
								(C) Norbert.Kucerka@nrc.gc.ca

_____________________________________________________________________________________________



*******
*ABOUT*
*******

Program 'SIMtoEXP' (originally 'VolProb') is for converting SIMulated data to scattering form
factors and comparing those to experimental data.

It also calculates the Volume Probability Distributions according to space filling volumetric
model.

_____________________________________________________________________________________________


**************
*INSTALLATION*
**************

Download the SIMtoEXP.tgz package and unpack it, or download the ready for use directory
SIMtoEXP while keeping the relative paths of all files.


Within the files:

Manual.doc is a walk-through user manual.

Executable files are located in bin directory. There is a windows version SIMtoEXP.exe
together with Mac OS X version SIMtoEXP.mac (compiled under Leopard) and linux version
SIMtoEXP.lnx (compiled under Fedora Core 11). Note, that only
windows version is provided as a stand-alone package, while other two executables
require X-windows framework and Tcl/Tk/Blt v8.4 to be installed by the user
(e.g. through MacPorts or Fink on Mac OS, and yum on Linux distros).

lib and bin directories include tcl/tk related libraries and dependencies for windows version.
The minimal package was extracted from version 8.4.13, however any patch to version 8.4 can be
used (NK can provide windows installation packages).

test directory includes the examples of input files as explained in user manual.

_____________________________________________________________________________________________


*********
*History*
*********

11/2012	 The parameters for X-ray atomic form factors of united atoms CH2, CH and CH3 were devised
	 by John Nagle as a reasonable combinations of the individual atoms.

12/2011	 Improved control of manipulating the scattering densities of water. While "set xWat (or nWat)"
	 changes them at every re-calculation, new "setRHO_wat X (or N)" changes them only when it is
	 not fixed in GUI.
	 !!! change the experimental data files accordingly (i.e., "set xWat" -> setRHO_wat X") !!!

10/2011	 Additional window that allows to perform some analysis has been added (GUI-->Tools-->Analysis).
	 One can combine volume probabilities of various components, and then canculate the Gibb's
	 dividing surface of a resulting distribution.

10/2011	 Checking 'symmetrized' box inside FT subpanel results now in symmetrizing the simulation too,
	 in addition to form factors only as done previously.

10/2011	 Additional 2 parameters have been added to manipulate the simulation, Z axis in particular.
	 These are 'Zcentre' for shifting off-centred simulations, and 'Zdirection' for arranging
	 the Z values in ascending order. Note that Zdirection may also be used to rescale the Z axis,
	 e.g. transforming units between Angstroems and nm. Both of the parameters can be entered
	 in the header of *.sim file or in GUI Simulation Specifics window.

05/2011  Clean-up.

03/2011  SIMtoEXP now includes partial charges correction in a calculation of x-ray form factors,
	 which can be defined for each atom in the second row (starting with “partial”) of *.sim file.

03/2011  The option to select a symmetrized Fourier Transform (utilizing cosine term only)
	 has been added.

03/2011  Various *.tcl and *.txt files were moved to the separate directory called tcl.

01/2010	 Margins delimiter was introduced (another good idea of Frank).

01/2010	 Option of loading 2 experimental data files (ULV and ORI) was expanded to 5 different
	 data files (SAM1 to SAM5).

01/2010	 Incomplete and/or overloaded space filling volumetric model can be loaded (i.e.,
	 model that does not comprise all of the atoms and/or uses some atoms in several
	 component groups). The warning messages will pop up for such models.
	 Further clean up of GUI's menu, which now includes advanced options to configurate
	 the axis of various graphs.

12/2009	 GUI was rearranged to emphesize following order of importance:
	 simulated scattering densities -> Fourier transforms -> comparison to exp. profiles
	 -> volumetric calculation

12/2009	 The issue of loading files with empty lines throughout the file and not at the end
	 was fixed (thanks to Frank Heinrich for pointing this out).
	 Option to manipulate NumEl/gr and NeutSl/gr was disabled to avoid confusion. These
	 quantities are calculated based on the NumEl and NeutSl of each atom automatically.
	 All the shown curves are now recalculated after changing any of these parameters.

11/2009	 **** BETA version ****
	 X-ray form factor calculation now includes atomic form factors.

11/2009  Further clean-up. The user can now define their own atom types (identifier and
	 scattering length densities).

06/2009  **** SIMtoEXP - GAMMA version ****
	 Data loading and manipulation was cleaned up and program was renamed to SIMtoEXP

05/2009  Program was expanded to calculate Fourier transform and to be capable to load EDP,
	 NSLDP, eFF, and nFF from external files

03/2009  **** SIMtoSDP ****
	 Program was expanded to include handling all-atom simulation files and renamed to
	 SIMulation to SDP

04/2008  **** VolumeProb ****
	 Volume analysis program by Norbert.Kucerka@nrc.gc.ca

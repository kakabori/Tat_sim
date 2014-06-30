package require BLT
namespace import blt::*
set zoomMod "Control-"
set tcl_precision 5
#wm geometry . =800x600+472+0
wm geometry . =830x700+400+20
########################
#general setup         #
########################
set commonusefont {Times 10}
set colors {#ff0000 #00ff00 #0000ff #00ffff #ff00ff #888800 #ff8800 #88ff00 #8800ff #aaaaaa black}

########################
#setup for .t BLT graph#
########################
set w .t
set numbw $w.n
set probw $w.p
set edw $w.ed
set nsldw $w.nd
set xftw $w.xft
set nftw $w.nft

set numbt "Number Densities"
set probt "Volume Probabilities"
set edt "Electron Densities"
set nsldt "Neutron SL Densities"
set xftt "X-ray FFs"
set nftt "Neutron FFs"

tabset $w -side top -borderwidth 0 -selectpad 0 -tabborderwidth 1 -highlightthickness 6
$w configure -font "Times 10"
graph $numbw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
graph $probw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
graph $edw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
graph $nsldw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
graph $xftw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
graph $nftw -width 1604 -height 1504 -plotrelief flat -plotpadx {0 0} -plotpady {0 0} -plotborderwidth 0
$numbw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize 5 -logscale no
$numbw axis configure y -min "" -max "" -loose 0 -subdivisions 5 -logscale no
$probw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize 5 -logscale no
$probw axis configure y -min "-.05" -max "1.05" -loose 0 -subdivisions 4 -stepsize .2 -logscale no
$edw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize 5 -logscale no
$edw axis configure y -min "" -max "" -loose 0 -subdivisions 5 -logscale no
$nsldw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize 5 -logscale no
$nsldw axis configure y -min "" -max "" -loose 0 -subdivisions 5 -logscale no
$xftw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize .2 -logscale no
$xftw axis configure y -min "" -max "" -loose 0 -subdivisions 5 -logscale no
$nftw axis configure x -min "" -max "" -loose 0 -subdivisions 5 -stepsize .1 -logscale no
$nftw axis configure y -min "" -max "" -loose 0 -subdivisions 5 -logscale no

$w insert 4 $numbt -window $numbw -padx 0 -pady 0
$w insert 0 $edt -window $edw -padx 0 -pady 0
$w insert 1 $nsldt -window $nsldw -padx 0 -pady 0
$w insert 5 $probt -window $probw -padx 0 -pady 0
$w insert 2 $xftt -window $xftw -padx 0 -pady 0
$w insert 3 $nftt -window $nftw -padx 0 -pady 0

###################################################################################################

proc create {type label i} {
	global numbw probw edw nsldw xftw nftw
	global colors
	global fileExpX(0)
#	set lab {ULV ORI MLV}
	if {$type=="sim"} {
		if {$label != "total"} {
			$numbw element create numb$i -xdata xsim -ydata ysim$i -pixels 0 -linewidth 1.5 -color [lindex $colors $i]
			$numbw element configure numb$i -label $label
#			$numbw legend activate numb$i
		}
		$edw element create ed$i -xdata xsim -ydata yed$i -pixels 0 -linewidth 1.5 -color [lindex $colors $i]
		$edw element configure ed$i -label $label
#		$edw legend activate ed$i
		$nsldw element create nsld$i -xdata xsim -ydata ynsld$i -pixels 0 -linewidth 1.5 -color [lindex $colors $i]
		$nsldw element configure nsld$i -label $label
#		$nsldw legend activate nsld$i
	} elseif {$type=="vol"} {
		$probw element create prob$i -xdata xsim -ydata yprob$i -pixels 0 -linewidth 1.5 -color [lindex $colors $i]
		$probw element configure prob$i -label $label
#		$probw legend activate prob$i
	} elseif {$type=="fftX"} {
		$xftw element create xft -xdata qFFx -ydata xFF -pixels 0 -linewidth 1.5 -color black
		$xftw element configure xft -label $label
#		$xftw legend activate xft
	} elseif {$type=="fftN"} {
		$nftw element create nft -xdata qFFn -ydata nFF -pixels 0 -linewidth 1.5 -color black
		$nftw element configure nft -label $label
#		$nftw legend activate nft
	} elseif {$type=="X"} {
		$xftw element create xftpExp$i -xdata qFEx$i -ydata xFE$i -pixels 1.5 -linewidth 0 -color [lindex $colors $i]
#		set label2 [lindex $lab $i]
		$xftw element configure xftpExp$i -label [file rootname [file tail $label]]
#		$xftw legend activate xftpExp$i
	} elseif {$type=="N"} {
		$nftw element create nftpExp$i -xdata qFEn$i -ydata nFE$i -pixels 1.5 -linewidth 0 -color [lindex $colors $i]
#		set label2 [lindex $lab $i]
		$nftw element configure nftpExp$i -label [file rootname [file tail $label]]
#		$nftw legend activate nftpExp$i
	} elseif {$type=="edp"} {
		$edw element create edpExp$i -xdata xedEXP -ydata yedEXP$i -pixels 1.5 -linewidth 0 -color [lindex $colors $i]
		$edw element configure edpExp$i -label $label
#		$edw legend activate edpExp$i
	} elseif {$type=="nsldp"} {
		$nsldw element create nsldpExp$i -xdata xnsldEXP -ydata ynsldEXP$i -pixels 1.5 -linewidth 0 -color [lindex $colors $i]
		$nsldw element configure nsldpExp$i -label $label
#		$nsldw legend activate nsldpExp$i
	}
}

proc delete {type i} {
	global numbw probw edw nsldw xftw nftw
	if {$type=="sim"} {
		catch {$numbw element delete numb$i}
		catch {$probw element delete prob$i}
		catch {$edw element delete ed$i}
		catch {$nsldw element delete nsld$i}
#		catch {$xftw element delete xft}
#		catch {$nftw element delete nft}
	} elseif {$type=="X"} {
		catch {$xftw element delete xftpExp$i}
	} elseif {$type=="N"} {
		catch {$nftw element delete nftpExp$i}
	} elseif {$type=="edp"} {
		catch {$edw element delete edpExp$i}
	} elseif {$type=="nsldp"} {
		catch {$nsldw element delete nsldpExp$i}
	}
}
proc delete_all {type n} {
	for {set i 0} {$i<$n} {incr i} {
		delete $type $i
	}
}

proc is_it_NKactive {graph entry} {
	set NKactive [$graph legend activate]
	if { [$graph element exists $entry] && [lsearch $NKactive $entry]<0 } {
		return 1
	} else {
		return 0
	}
}

foreach win {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {
	bind $win <B1-Motion> {set ycord [%W axis invtransform y %y]; set xcord [%W axis invtransform x %x]}
	bind $win <1> {set ycord [%W axis invtransform y %y]; set xcord [%W axis invtransform x %x]}
}

set left_margin 0
set right_margin 0
proc reset_margins {} {
	global left_margin right_margin xsim
	global qleft_margin qright_margin
	set left_margin 0
	set right_margin [expr [xsim length]-1]
	set qleft_margin $xsim($left_margin)
	set qright_margin $xsim($right_margin)
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name left_edge -coords {$xsim($left_margin) -999999 $xsim($left_margin) 999999} -outline blue
	}
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name right_edge -coords {$xsim($right_margin) -999999 $xsim($right_margin) 999999} -outline blue
	}
}

########################
#setup for .f          #
#menu, text widget     #
#xy coords             #
########################
# FILE ##################################################################################################

set w .f
frame $w
set m $w.file.m
menubutton $w.file -text File -menu $m -activebackground gray -activeforeground white
menu $m -tearoff 0
$m add command -label "OpenSIM" -command {
   global fileSIM
   set types {
	{{inp Files}       {.dat .sim}        }
	{{All Files}        *             }
    }
    set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
    cd [file dirname $inpfilename]
    update
    if {$inpfilename != ""} {
		LoadDataNew new $inpfilename 1
		set fileSIM $inpfilename
		normalize
		reset_margins
		if {[is_it_NKactive .t.xft xft]} {
			fourierX
			for {set i 0} {$i<$MaxEXP} {incr i} {
				if {[is_it_NKactive .t.xft xftpExp$i]} {LoadDataX $fileExpX($i) $i; scaling yes x $i}
			}
		}
		if {[is_it_NKactive .t.nft nft]} {
			fourierN
			for {set i 0} {$i<$MaxEXP} {incr i} {
				if {[is_it_NKactive .t.nft nftpExp$i]} {LoadDataN $fileExpN($i) $i; scaling yes n $i}
			}
		}
	}
}
$m add cascade -label "OpenEXPx" -menu $m.expx
menu $m.expx -tearoff 0
	$m.expx add command -label "SAM1" -command {
		global fileExpX(0)
		set types {
			{{inp Files}       {.dat .xff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete X 0
			LoadDataX $inpfilename 0
			set fileExpX(0) $inpfilename
			if {[is_it_NKactive .t.xft xft]} {scaling yes x 0}
		}
	}
	$m.expx add command -label "SAM2" -command {
		global fileExpX(1)
		set types {
			{{inp Files}       {.dat .xff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete X 1
			LoadDataX $inpfilename 1
			set fileExpX(1) $inpfilename
			if {[is_it_NKactive .t.xft xft]} {scaling yes x 1}
		}
	}
	$m.expx add command -label "SAM3" -command {
		global fileExpX(2)
		set types {
			{{inp Files}       {.dat .xff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete X 2
			LoadDataX $inpfilename 2
			set fileExpX(2) $inpfilename
			if {[is_it_NKactive .t.xft xft]} {scaling yes x 2}
		}
	}
	$m.expx add command -label "SAM4" -command {
		global fileExpX(3)
		set types {
			{{inp Files}       {.dat .xff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete X 3
			LoadDataX $inpfilename 3
			set fileExpX(3) $inpfilename
			if {[is_it_NKactive .t.xft xft]} {scaling yes x 3}
		}
	}
	$m.expx add command -label "SAM5" -command {
		global fileExpX(4)
		set types {
			{{inp Files}       {.dat .xff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete X 4
			LoadDataX $inpfilename 4
			set fileExpX(4) $inpfilename
			if {[is_it_NKactive .t.xft xft]} {scaling yes x 4}
		}
	}
	
$m add cascade -label "OpenEXPn" -menu $m.expn
menu $m.expn -tearoff 0
	$m.expn add command -label "SAM1" -command {
		global fileExpN(0)
		set types {
			{{inp Files}       {.dat .nff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete N 0
			LoadDataN $inpfilename 0
			set fileExpN(0) $inpfilename
			if {[is_it_NKactive .t.nft nft]} {scaling yes n 0}
		}
	}
	$m.expn add command -label "SAM2" -command {
		global fileExpN(1)
		set types {
			{{inp Files}       {.dat .nff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete N 1
			LoadDataN $inpfilename 1
			set fileExpN(1) $inpfilename
			if {[is_it_NKactive .t.nft nft]} {scaling yes n 1}
		}
	}
	$m.expn add command -label "SAM3" -command {
		global fileExpN(2)
		set types {
			{{inp Files}       {.dat .nff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete N 2
			LoadDataN $inpfilename 2
			set fileExpN(2) $inpfilename
			if {[is_it_NKactive .t.nft nft]} {scaling yes n 2}
		}
	}
	$m.expn add command -label "SAM4" -command {
		global fileExpN(3)
		set types {
			{{inp Files}       {.dat .nff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete N 3
			LoadDataN $inpfilename 3
			set fileExpN(3) $inpfilename
			if {[is_it_NKactive .t.nft nft]} {scaling yes n 3}
		}
	}
	$m.expn add command -label "SAM5" -command {
		global fileExpN(4)
		set types {
			{{inp Files}       {.dat .nff}        }
			{{All Files}        *             }
		}
		set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
		cd [file dirname $inpfilename]
		update
		if {$inpfilename != ""} {
			delete N 4
			LoadDataN $inpfilename 4
			set fileExpN(4) $inpfilename
			if {[is_it_NKactive .t.nft nft]} {scaling yes n 4}
		}
	}


$m add command -label "OpenEDP" -command {
   set types {
	{{inp Files}       {.dat .edp}        }
	{{All Files}        *             }
    }
    set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
    cd [file dirname $inpfilename]
    update
    if {$inpfilename != ""} {
    delete_all edp [expr $MaxComponent+1]
	LoadSDPx $inpfilename
    }
}
$m add command -label "OpenNSLD" -command {
   set types {
	{{inp Files}       {.dat .nsld}        }
	{{All Files}        *             }
    }
    set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
    cd [file dirname $inpfilename]
    update
    if {$inpfilename != ""} {
    delete_all nsldp [expr $MaxComponent+1]
	LoadSDPn $inpfilename
    }
}
$m add command -label "OpenCMP" -command {
   global LoadData fileSIM fileCMP
   set types {
	{{inp Files}       {.cmp}        }
	{{All Files}        *             }
    }
    set inpfilename [tk_getOpenFile -initialdir "./" -filetypes $types]
    cd [file dirname $inpfilename]
    update
    if {$inpfilename != ""} {
#	source $inpfilename
		LoadCmd $inpfilename 1
		set fileCMP $inpfilename
#		if {$LoadData==1} {LoadDataNew re $fileSIM}
		if {$LoadData==1} {LoadDataNew new $fileSIM 0}
		normalize
    }
}
$m add command -label "Exit" -command {exit}

# TOOLS ##################################################################################################

set m $w.tls.m
menubutton $w.tls -text Tools -menu $m -activebackground gray -activeforeground white
menu $m -tearoff 0
$m add command -label "Scattering Lengths" -command {SLwin}
$m add command -label "Atomic Form Factors" -command {AFFwin}
$m add cascade -label "Export" -menu $m.exp
menu $m.exp -tearoff 0
$m.exp add command -label "X-ray FFs" -command {
   set types {
	{{out Files}       {.xff}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export 0xff $outfilename
    }
}
$m.exp add command -label "Neutron FFs" -command {
   set types {
	{{out Files}       {.nff}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export 1nff $outfilename
    }
}
$m.exp add command -label "El Density" -command {
   set types {
	{{out Files}       {.edp}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export edp $outfilename
    }
}
$m.exp add command -label "NSL Density" -command {
   set types {
	{{out Files}       {.nsld}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export nsld $outfilename
    }
}
$m.exp add command -label "Volumetric fit" -command {
   set types {
	{{out Files}       {.vpp .otp}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export fit $outfilename
    }
}
$m.exp add command -label "Numb Density" -command {
   set types {
	{{out Files}       {.sim}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export sim $outfilename
    }
}
$m.exp add command -label "ALL" -command {
   set types {
	{{out Files}       {.xff .nff .edp .nsld .otp .vpp .sim}        }
	{{All Files}        *             }
    }
    set outfilename [tk_getSaveFile -initialdir "./" -filetypes $types]
    cd [file dirname $outfilename]
    update
    if {$outfilename != ""} {
	export fit $outfilename
	export edp $outfilename
	export nsld $outfilename
	export sim $outfilename
	export 0xff $outfilename
	export 1nff $outfilename
    }
}
$m add cascade -label "Print" -menu $m.pr
menu $m.pr -tearoff 0
$m.pr add command -label "X-ray FFs" -command "Blt_PostScriptDialog $xftw"
$m.pr add command -label "Neutron FFs" -command "Blt_PostScriptDialog $nftw"
$m.pr add command -label "El Density" -command "Blt_PostScriptDialog $edw"
$m.pr add command -label "NSL Density" -command "Blt_PostScriptDialog $nsldw"
$m.pr add command -label "Vol Probability" -command "Blt_PostScriptDialog $probw"
$m.pr add command -label "Numb Density" -command "Blt_PostScriptDialog $numbw"

$m add command -label "Simulation Specifics" -command {SIMwin}

$m add command -label "Analysis" -command {ANALYSISwin}

# AXIS ##################################################################################################

proc showmore {w ax} {
  set limits [$w axis limits $ax]
  set minold [lindex $limits 0]
  set maxold [lindex $limits 1]
  set min [expr $minold-($maxold-$minold)/10]
  set max [expr $maxold+($maxold-$minold)/10]
  if { [catch {$w axis configure $ax -min $min -max $max}]!=0 } {
	set min [expr [lindex $limits 0]/10]
	catch {$w axis configure $ax -min $min -max $max}
  }
}

proc showall {w} {
  $w axis configure x -max "" -min ""
  $w axis configure y -max "" -min ""
}

set legend 1
proc hide_legend {} {
	global legend
	foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {
		$graph legend configure -hide [expr $legend?"yes":"no"]
	}
	set legend [expr $legend ? 0 : 1]
}

proc advanced_axis {grph titl} {
	global graf
	global xmin xmax xstp xdiv xlog
	global ymin ymax ystp ydiv ylog
	set graf $grph
	set w .axs
	toplevel $w
	wm title $w "axis: $titl"	
	
	set xmin [$graf axis cget x -min]
	set xmax [$graf axis cget x -max]
	set xstp [$graf axis cget x -stepsize]
	set xdiv [$graf axis cget x -subdivision]
	set xlog [$graf axis cget x -log]
	set ymin [$graf axis cget y -min]
	set ymax [$graf axis cget y -max]
	set ystp [$graf axis cget y -stepsize]
	set ydiv [$graf axis cget y -subdivision]
	set ylog [$graf axis cget y -log]
	
	label $w.lconf -text "configure"
	label $w.lmin -text "min"
	label $w.lmax -text "max"
	label $w.lstp -text "step"
	label $w.ldiv -text "subdivision"
	label $w.llog -text "log"
	
	label $w.lx -text "x axis"
	entry $w.exmin -textvariable xmin -borderwidth 1 -width 10
	entry $w.exmax -textvariable xmax -borderwidth 1 -width 10	
	entry $w.exstp -textvariable xstp -borderwidth 1 -width 10	
	entry $w.exdiv -textvariable xdiv -borderwidth 1 -width 10	
	checkbutton $w.cbxlog -variable xlog -padx 0 -pady 0 -borderwidth 1 -command { runTCLcommand "$graf axis configure x -log $xlog" }
	label $w.ly -text "y axis"
	entry $w.eymin -textvariable ymin -borderwidth 1 -width 10
	entry $w.eymax -textvariable ymax -borderwidth 1 -width 10
	entry $w.eystp -textvariable ystp -borderwidth 1 -width 10	
	entry $w.eydiv -textvariable ydiv -borderwidth 1 -width 10	
	checkbutton $w.cbylog -variable ylog -padx 0 -pady 0 -borderwidth 1 -command { runTCLcommand "$graf axis configure y -log $ylog" }

	grid $w.lconf $w.lmin $w.lmax $w.lstp $w.ldiv $w.llog
	grid $w.lx $w.exmin $w.exmax $w.exstp $w.exdiv $w.cbxlog
	grid $w.ly $w.eymin $w.eymax $w.eystp $w.eydiv $w.cbylog

	bind $w.exmin <Return> { runTCLcommand "$graf axis configure x -min $xmin" }
	bind $w.exmax <Return> { runTCLcommand "$graf axis configure x -max $xmax" }
	bind $w.exstp <Return> { runTCLcommand "$graf axis configure x -step $xstp" }
	bind $w.exdiv <Return> { runTCLcommand "$graf axis configure x -subdivision $xdiv" }
	bind $w.eymin <Return> { runTCLcommand "$graf axis configure y -min $ymin" }
	bind $w.eymax <Return> { runTCLcommand "$graf axis configure y -max $ymax" }
	bind $w.eystp <Return> { runTCLcommand "$graf axis configure y -step $ystp" }
	bind $w.eydiv <Return> { runTCLcommand "$graf axis configure y -subdivision $ydiv" }
}

set m $w.ax.m
menubutton $w.ax -text Axis -menu $m -activebackground gray -activeforeground white
menu $m -tearoff 0
$m add command -label "show more x" -command {
	foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {showmore $graph x}
}
$m add command -label "show more y" -command {
	foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {showmore $graph y}
}
$m add command -label "show all" -command {
	foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {showall $graph}
}
$m add command -label "show/hide legend" -command {hide_legend}
$m add cascade -label "advanced" -menu $m.adv
menu $m.adv -tearoff 0
$m.adv add command -label "Electron Densities" -command {advanced_axis $edw $edt}
$m.adv add command -label "Neutron SL Densities" -command {advanced_axis $nsldw $nsldt}
$m.adv add command -label "X-ray FFs" -command {advanced_axis $xftw $xftt}
$m.adv add command -label "Neutron FFs" -command {advanced_axis $nftw $nftt}
$m.adv add command -label "Number Densities" -command {advanced_axis $numbw $numbt}
$m.adv add command -label "Volume Probabilities" -command {advanced_axis $probw $probt}

# HELP ##################################################################################################

proc show_text {file ext data} {
	toplevel .rdm
    set w .rdm
    scrollbar $w.scroll -command "$w.text yview"
    text $w.text -state normal -width 100 -yscrollcommand "$w.scroll set" -cursor arrow
	pack $w.scroll $w.text -side right -fill y
	
#	set filename [concat $file.$ext]
#	set fileid [open $filename r]
#	set data [read $fileid]
#	close $fileid
	$w.text insert end $data
	wm title .rdm $file
}
proc read_text {file ext} {
	set nameoffile [file tail $file]
	global data$nameoffile$ext
	set filename [concat $file.$ext]
	set fileid [open $filename r]
	set data$nameoffile$ext [read $fileid]
	close $fileid
}

set m $w.help.m
menubutton $w.help -text Help -menu $m -activebackground gray -activeforeground white
menu $m -tearoff 0
$m add command -label "About SIMtoEXP" -command {show_text About txt $dataAbouttxt}
$m add command -label "User manual" -command {show_text Manual txt $dataManualtxt}
#$m add command -label "Tutorial" -command {show_text Tutorial txt $dataTutorialtxt}
$m add command -label "License" -command {show_text License txt $dataLicensetxt}

catch {read_text tcl/About txt}
catch {read_text tcl/Manual txt}
#catch {read_text Tutorial txt}
catch {read_text tcl/License txt}

pack $w.file $w.tls $w.ax $w.help -side left

# COORDS ##################################################################################################

label $w.lx -text "x:" -fg red
label $w.x -textvariable xcord -width 10 -padx 2 -pady 2
label $w.ly -text "y:" -fg red -width 4
label $w.y -textvariable ycord -width 10 -padx 2 -pady 2
pack $w.y $w.ly $w.x $w.lx -side right

# MARGINS ##################################################################################################
bind . <Control-Key-Right> {
	global left_margin xsim qleft_margin
	if {$left_margin<[expr [xsim length]-1]} {incr left_margin}
	set qleft_margin $xsim($left_margin)
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name left_edge -coords {$xsim($left_margin) -999999 $xsim($left_margin) 999999} -outline blue
	}
}
bind . <Control-Key-Left> {
	global left_margin xsim qleft_margin
	if {$left_margin>0} {set left_margin [expr $left_margin-1]}
	set qleft_margin $xsim($left_margin)
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name left_edge -coords {$xsim($left_margin) -999999 $xsim($left_margin) 999999} -outline blue
	}
}
bind . <Shift-Control-Key-Right> {
	global right_margin xsim qright_margin
	if {$right_margin<[expr [xsim length]-1]} {incr right_margin}
	set qright_margin $xsim($right_margin)
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name right_edge -coords {$xsim($right_margin) -999999 $xsim($right_margin) 999999} -outline blue
	}
}
bind . <Shift-Control-Key-Left> {
	global right_margin xsim qright_margin
	if {$right_margin>0} {set right_margin [expr $right_margin-1]}
	set qright_margin $xsim($right_margin)
	foreach graph {.t.n .t.p .t.ed .t.nd} {
		$graph marker create line -name right_edge -coords {$xsim($right_margin) -999999 $xsim($right_margin) 999999} -outline blue
	}
}

set w .m
frame $w
label $w.lmarl -text "left:" -fg blue
label $w.marl -textvariable qleft_margin -width 10 -padx 2 -pady 2
label $w.lmarr -text "right:" -fg blue
label $w.marr -textvariable qright_margin -width 10 -padx 2 -pady 2
pack $w.marr $w.lmarr $w.marl $w.lmarl -side right

# MISC ##################################################################################################

foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {
	Blt_ZoomStack $graph
	Blt_Crosshairs $graph
	Blt_PrintKey $graph
}

foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {
	$graph legend bind all <1> { Insignify %W }
	$graph legend bind all <Control-1> { delete_click %W }
	$graph legend bind all <3> { raiselement_click %W }
}
    
proc axisshift {w d a} {
	set limits [$w axis limits $a]
	set min [lindex $limits 0]
	set max [lindex $limits 1]
	set dif [expr $max-$min]
	set max [expr $max+$dif/$d]
	set min [expr $min+$dif/$d]
	$w axis configure $a -min $min -max $max
}

bind . <Alt-Key-Up> {foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {axisshift $graph 10 y; axisshift $graph 10 y}}
bind . <Alt-Key-Down> {foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {axisshift $graph -10 y; axisshift $graph -10 y}}
bind . <Alt-Key-Left> {foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {axisshift $graph -10 x; axisshift $graph -10 x}}
bind . <Alt-Key-Right> {foreach graph {.t.n .t.p .t.ed .t.nd .t.xft .t.nft} {axisshift $graph 10 x; axisshift $graph 10 x}}

proc Insignify { graph } {
    global numbw probw edw nsldw xftw nftw
    set entry [$graph legend get current]
    set NKactive [$graph legend activate]
    if { [lsearch $NKactive $entry] < 0 } {
		$graph legend activate $entry
		$graph element configure $entry -pixels 0 -linewidth 0
    } else {
		if { [lindex [split $entry p] 1] == "Ex"} {
			$graph element configure $entry -pixels 1.5 -linewidth 0
		} else {
			$graph element configure $entry -pixels 0 -linewidth 1.5
		}
        $graph legend deactivate $entry       
    }
}

proc raiselement { graph i } {
    set ori [$graph element show]
    set tgt [lsearch $ori $i]
    set ori [lreplace $ori $tgt $tgt]
    set ori [linsert $ori end $i]
    $graph element show $ori
}

proc raiselement_click { graph } {
    set i [$graph legend get current]
    set ori [$graph element show]
    set tgt [lsearch $ori $i]
    set ori [lreplace $ori $tgt $tgt]
    set ori [linsert $ori end $i]
    $graph element show $ori
}

proc delete_click { graph } {
	set entry [$graph legend get current]
	$graph element delete $entry
}

###################################################################################################
# PARAMETERS AND BUTTONS/COMMANDS #################################################################
###################################################################################################

set w .wins
frame $w

set w .wins.params
frame $w
# MARGINS ##################################################################################################
  set w .wins.params.marg
  frame $w
	label $w.lmarl -text "margins: left" -fg blue
	label $w.marl -textvariable qleft_margin -width 10 -padx 2 -pady 2
	label $w.lmarr -text "right" -fg blue
	label $w.marr -textvariable qright_margin -width 10 -padx 2 -pady 2
	pack $w.lmarl $w.marl $w.lmarr $w.marr -side left

# FOURIER #########################################################################################
proc setRHO_wat {type rho} {
	global FIXxWat FIXnWat
	global xWat nWat
	if {$type=="X" && $FIXxWat!=1} {set xWat $rho}
	if {$type=="N" && $FIXnWat!=1} {set nWat $rho}
}

  set w .wins.params.ff
  frame $w -bd 1 -relief groove
    set w .wins.params.ff.int
    frame $w
    label $w.lqint -text "q interval"
    label $w.lq0 -text low
    label $w.lq1 -text high
    label $w.lqstep -text step
    label $w.lx -text x-rays
    label $w.ln -text neutrons
    label $w.lwatx -text ED_wat
    label $w.lwatn -text NSLD_wat
    entry $w.eq0x -textvariable xq0 -width 9
    entry $w.eq1x -textvariable xq1 -width 9
    entry $w.eqsx -textvariable xqstep -width 9
    entry $w.eq0n -textvariable nq0 -width 9
    entry $w.eq1n -textvariable nq1 -width 9
    entry $w.eqsn -textvariable nqstep -width 9
    entry $w.ewatx -textvariable xWat -width 9
    entry $w.ewatn -textvariable nWat -width 9
    checkbutton $w.cbwatx -variable FIXxWat -offvalue 0 -onvalue 1
    label $w.lfixxwat -text fix
    checkbutton $w.cbwatn -variable FIXnWat -offvalue 0 -onvalue 1
    label $w.lfixnwat -text fix
    grid $w.lqint $w.lq0 $w.lq1 $w.lqstep
    grid $w.lx $w.eq0x $w.eq1x $w.eqsx
    grid $w.ln $w.eq0n $w.eq1n $w.eqsn
    grid $w.lwatx $w.ewatx $w.cbwatx $w.lfixxwat
    grid $w.lwatn $w.ewatn $w.cbwatn $w.lfixnwat    

      set w .wins.params.ff.but
      frame $w
      button $w.fourier -text "Fourier Transform" -borderwidth 3 -command {
		if {$LoadData!=1} {
			set err "Load a simulation first!"
			puts $err
			tk_messageBox -title warning -message $err -type ok -icon warning
		}
		if {[is_it_NKactive .t.ed ed$MaxComponent]} {
			fourierX
			for {set i 0} {$i<$MaxEXP} {incr i} {
				if {[is_it_NKactive .t.xft xftpExp$i]} {LoadDataX $fileExpX($i) $i; scaling yes x $i}
			}
		}
		if {[is_it_NKactive .t.nd nsld$MaxComponent]} {
			fourierN
			for {set i 0} {$i<$MaxEXP} {incr i} {
				if {[is_it_NKactive .t.nft nftpExp$i]} {LoadDataN $fileExpN($i) $i; scaling yes n $i}
			}
		}
      }
      checkbutton $w.symm -variable symmetrized -offvalue 0 -onvalue 1
      label $w.symmlab -text symmetrized

    pack $w.fourier $w.symm $w.symmlab -side left
    set w .wins.params
    pack $w.ff.int $w.ff.but -side bottom

# SCALING #########################################################################################
  set w .wins.params.sc
  frame $w -bd 1 -relief groove
# X-RAY ###############################
	set w .wins.params.sc.x
	frame $w
	label $w.lscal -text Scaling
    label $w.lx -text X-ray
    button $w.lsc -text scale -borderwidth 3
    bind $w.lsc <1> {
	    for {set i 0} {$i<$MaxEXP} {incr i} {
			if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$i]} {LoadDataX $fileExpX($i) $i; scaling yes x $i}
		}
	}
    label $w.lfxsc -text fix
    label $w.lbg -text bckgrnd
    label $w.lfxbg -text fix
    label $w.lchi2 -text CHI^2
    for {set i 0} {$i<$MaxEXP} {incr i} {
		button $w.but$i -text SAM[expr $i+1] -borderwidth 0
#		bind $w.but$i <1> {if {[is_it_NKactive .t.xft xft]} {LoadDataX $fileExpX($i) $i; scaling yes x $i}}
	}
	grid $w.lx $w.but0 $w.but1 $w.but2 $w.but3 $w.but4
	bind $w.but0 <1> {if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp0]} {LoadDataX $fileExpX(0) 0; scaling yes x 0}}
	bind $w.but1 <1> {if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp1]} {LoadDataX $fileExpX(1) 1; scaling yes x 1}}
	bind $w.but2 <1> {if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp2]} {LoadDataX $fileExpX(2) 2; scaling yes x 2}}
	bind $w.but3 <1> {if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp3]} {LoadDataX $fileExpX(3) 3; scaling yes x 3}}
	bind $w.but4 <1> {if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp4]} {LoadDataX $fileExpX(4) 4; scaling yes x 4}}
	for {set i 0} {$i<$MaxEXP} {incr i} {
		entry $w.escx$i -textvariable xscale($i) -width 6 -fg [lindex $colors $i]
#		entry $w.ebgx$i -textvariable xbckgrnd($i) -width 5
		entry $w.echix$i -textvariable xCHI2($i) -width 6 -fg [lindex $colors $i]
#		checkbutton $w.scfixx$i -variable xSCfix($i) -offvalue 0 -onvalue 1
#		checkbutton $w.bgfixx$i -variable xBGfix($i) -offvalue 0 -onvalue 1
#		bind $w.escx$i <Return> {if {[is_it_NKactive .t.xft xftpExp$i]} {LoadDataX $fileExpX($i) $i; scaling no x $i}}
	}
	grid $w.lsc $w.escx0 $w.escx1 $w.escx2 $w.escx3 $w.escx4
	bind $w.escx0 <Return> {if {[is_it_NKactive .t.xft xftpExp0]} {LoadDataX $fileExpX(0) 0; scaling no x 0}}
	bind $w.escx1 <Return> {if {[is_it_NKactive .t.xft xftpExp1]} {LoadDataX $fileExpX(1) 1; scaling no x 1}}
	bind $w.escx2 <Return> {if {[is_it_NKactive .t.xft xftpExp2]} {LoadDataX $fileExpX(2) 2; scaling no x 2}}
	bind $w.escx3 <Return> {if {[is_it_NKactive .t.xft xftpExp3]} {LoadDataX $fileExpX(3) 3; scaling no x 3}}
	bind $w.escx4 <Return> {if {[is_it_NKactive .t.xft xftpExp4]} {LoadDataX $fileExpX(4) 4; scaling no x 4}}
#	grid $w.lfxsc $w.scfixx0 $w.scfixx1 $w.scfixx2
	grid $w.lchi2 $w.echix0 $w.echix1 $w.echix2 $w.echix3 $w.echix4
    pack $w
    
# NEUTRONS ############################    
	set w .wins.params.sc.n
	frame $w
	label $w.lscal -text Scaling
    label $w.lx -text Neutron
    button $w.lsc -text scale -borderwidth 3
    bind $w.lsc <1> {
	    for {set i 0} {$i<$MaxEXP} {incr i} {
			if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp$i]} {LoadDataN $fileExpN($i) $i; scaling yes n $i}
		}
	}
    label $w.lfxsc -text fix
    label $w.lbg -text bckgrnd
    label $w.lfxbg -text fix
    label $w.lchi2 -text CHI^2
    for {set i 0} {$i<$MaxEXP} {incr i} {
		button $w.but$i -text SAM[expr $i+1] -borderwidth 0
#		bind $w.but$i <1> {if {[is_it_NKactive .t.nft nft]} {LoadDataN $fileExpN($i) $i; scaling yes n $i}}
	}
    grid $w.lx $w.but0 $w.but1 $w.but2 $w.but3 $w.but4
    bind $w.but0 <1> {if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp0]} {LoadDataN $fileExpN(0) 0; scaling yes n 0}}
    bind $w.but1 <1> {if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp1]} {LoadDataN $fileExpN(1) 1; scaling yes n 1}}
    bind $w.but2 <1> {if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp2]} {LoadDataN $fileExpN(2) 2; scaling yes n 2}}
    bind $w.but3 <1> {if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp3]} {LoadDataN $fileExpN(3) 3; scaling yes n 3}}
    bind $w.but4 <1> {if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp4]} {LoadDataN $fileExpN(4) 4; scaling yes n 4}}
	for {set i 0} {$i<$MaxEXP} {incr i} {
		entry $w.escx$i -textvariable nscale($i) -width 6 -fg [lindex $colors $i]
#		entry $w.ebgx$i -textvariable nbckgrnd($i) -width 5
		entry $w.echix$i -textvariable nCHI2($i) -width 6 -fg [lindex $colors $i]
#		checkbutton $w.scfixx$i -variable nSCfix($i) -offvalue 0 -onvalue 1
#		checkbutton $w.bgfixx$i -variable nBGfix($i) -offvalue 0 -onvalue 1
#		bind $w.escx$i <Return> {if {[is_it_NKactive .t.nft nftpExp$i]} {LoadDataN $fileExpN($i) $i; scaling no n $i}}
	}
	grid $w.lsc $w.escx0 $w.escx1 $w.escx2 $w.escx3 $w.escx4
	bind $w.escx0 <Return> {if {[is_it_NKactive .t.nft nftpExp0]} {LoadDataN $fileExpN(0) 0; scaling no n 0}}
	bind $w.escx1 <Return> {if {[is_it_NKactive .t.nft nftpExp1]} {LoadDataN $fileExpN(1) 1; scaling no n 1}}
	bind $w.escx2 <Return> {if {[is_it_NKactive .t.nft nftpExp2]} {LoadDataN $fileExpN(2) 2; scaling no n 2}}
	bind $w.escx3 <Return> {if {[is_it_NKactive .t.nft nftpExp3]} {LoadDataN $fileExpN(3) 3; scaling no n 3}}
	bind $w.escx4 <Return> {if {[is_it_NKactive .t.nft nftpExp4]} {LoadDataN $fileExpN(4) 4; scaling no n 4}}
#	grid $w.lfxsc $w.scfixx0 $w.scfixx1 $w.scfixx2
	grid $w.lchi2 $w.echix0 $w.echix1 $w.echix2 $w.echix3 $w.echix4
    pack $w
        
  set w .wins.params
  pack $w.marg $w.ff $w.sc -side top
pack $w -side left

set w .wins.vol
frame $w
# VOLUMES #########################################################################################
  set w .wins.vol.calc
  frame $w -borderwidth 1 -relief groove
  button $w.start -text "Calculate Volumes" -borderwidth 3 -command {
	if {$LoadCmd!=0} {normalize; fit}
	if {$LoadCmd==0} {
		set err "Load a component file first!"
		puts $err
		tk_messageBox -title warning -message $err -type ok -icon warning
	}
  }
  bind $w.start <3> {LoadDataNew new $fileSIM 0}
#  grid x $w.start
  label $w.lrms -text "rms ="
  entry $w.erms -textvariable rms -width 10 -relief flat
#  grid $w.lrms $w.erms
  pack $w.start $w.lrms $w.erms -side left

# COMPONENTS ######################################################################################
set w .wins.vol.components
frame $w -borderwidth 1 -relief groove
label $w.lpar -text "Component" -width 20
label $w.lnat -text "nAtom" -width 8
label $w.lnatpgr -text "nAt/group" -width 8
label $w.lngr -text "ngr" -width 8
label $w.lnumel -text "NumEl/gr" -width 8
label $w.lneutsl -text "NeutSL/gr" -width 8
label $w.lval -text "Volume/gr" -width 12
label $w.lfix -text "fix" -width 5
grid $w.lpar $w.lnat $w.lnatpgr $w.lngr $w.lnumel $w.lneutsl $w.lval $w.lfix
for {set i 0} {$i<$MaxComponent} {incr i} {
  entry $w.nam$i -textvariable name($i) -width 24 -fg [lindex $colors $i]
  entry $w.nat$i -textvariable nat($i) -width 6 -fg [lindex $colors $i] -state readonly
  entry $w.natpgr$i -textvariable natpgr($i) -width 6 -fg [lindex $colors $i]
  entry $w.ngr$i -textvariable ngr($i) -width 6 -fg [lindex $colors $i] -state readonly
  entry $w.nel$i -textvariable nelpgr($i) -width 6 -fg [lindex $colors $i] -state readonly
#  bind $w.nel$i <Return> {catch {LoadDataNew re $fileSIM}; normalize}
  entry $w.nsl$i -textvariable nslpgr($i) -width 10 -fg [lindex $colors $i] -state readonly
#  bind $w.nsl$i <Return> {catch {LoadDataNew re $fileSIM}; normalize}
  entry $w.vol$i -textvariable vol($i) -width 10 -fg [lindex $colors $i]
  checkbutton $w.fix$i -variable fix($i) -offvalue 0 -onvalue 1
  grid $w.nam$i $w.nat$i $w.natpgr$i $w.ngr$i $w.nel$i $w.nsl$i $w.vol$i $w.fix$i
  bind $w.natpgr$i <Return> [list renormalize $i]
  bind $w.natpgr$i <Return> +fit
  bind $w.nam$i <Return> [list rename $i]
}

set w .wins.vol
pack $w.calc $w.components -side top -anchor w
pack $w -side left
pack .wins -side bottom -fill x
pack .f .t -side top -anchor nw -expand yes -fill both

proc renormalize {i} {
	global ngr nat natpgr nelpgr nel nslpgr nsl
	normalize
	set ngr($i) [expr $nat($i)/$natpgr($i)]
	set nelpgr($i) [expr $nel($i)/$ngr($i)]
	set nslpgr($i) [expr $nsl($i)/$ngr($i)]
#	fit
}	

proc rename {i} {
	global numbw edw nsldw probw name
	global numb0 numb1 numb2 numb3 numb4 numb5 numb6 numb7 numb8 numb9
	global ed0 ed1 ed2 ed3 ed4 ed5 ed6 ed7 ed8 ed9
	global nsld0 nsld1 nsld2 nsld3 nsld4 nsld5 nsld6 nsld7 nsld8 nsld9
	global prob0 prob1 prob2 prob3 prob4 prob5 prob6 prob7 prob8 prob9
	$numbw element configure numb$i -label $name($i)
	$edw element configure ed$i -label $name($i)
	$nsldw element configure nsld$i -label $name($i)
	$probw element configure prob$i -label $name($i)
}

###################################################################################################
proc SIMwin {} {
	toplevel .simw
    wm title .simw "SIM Specifics"
	set w .simw.val
	frame $w
	label $w.lZc -text "Zcentre" -width 20
	label $w.lZd -text "Zdirection" -width 20
	label $w.lA -text "Area per Lipid" -width 20
	label $w.lNL -text "Number of Lipids" -width 20
	label $w.lbs -text "bin step size" -width 20
	label $w.lbv -text "total bin volume" -width 20
	label $w.ldens -text "densities" -width 20
	label $w.lhis -text "histograms" -width 20
	entry $w.eZc -textvariable Zcentre -width 10
	entry $w.eZd -textvariable Zdirection -width 10
	entry $w.eA -textvariable area -width 10
	entry $w.eNL -textvariable Nlip -width 10
	entry $w.ebs -textvariable binstep -width 10
	entry $w.ebv -textvariable binsize -width 10 -state readonly
	radiobutton $w.cdens -variable norm -value 1 -command {normalize}
	radiobutton $w.chis -variable norm -value 0 -command {normalize}
	grid $w.lZc $w.eZc
	grid $w.lZd $w.eZd
	grid $w.lA $w.eA
	grid $w.lNL $w.eNL
	grid $w.lbs $w.ebs
	grid $w.lbv $w.ebv
	grid $w.ldens $w.cdens
	grid $w.lhis $w.chis
	pack $w
	bind $w.eA <Return> normalize
	bind $w.eNL <Return> normalize
	bind $w.ebs <Return> normalize
	bind $w.eZc <Return> {updateSIM; reset_margins}
	bind $w.eZd <Return> {updateSIM; reset_margins}
}

###################################################################################################
proc SLwin {} {
	global MaxComponent MaxEXP
	global nid id b e fileSIM fileCMP fileExpN fileExpX
	toplevel .slw
    wm title .slw "Scatt. Lengths"
	set w .slw.val
	frame $w
	label $w.lAt -text "Atom" -width 16
	label $w.l0 -text "Hydrogen (H)" -width 16
	label $w.l1 -text "Deuterium (D)" -width 16
	label $w.l2 -text "Carbon (C)" -width 16
	label $w.l3 -text "Nitrogen (N)" -width 16
	label $w.l4 -text "Oxygen (O)" -width 16
	label $w.l5 -text "Phosphorus (P)" -width 16
	label $w.l6 -text "Methyl (T)" -width 16
	label $w.l7 -text "Methylene (M)" -width 16
	label $w.l8 -text "Methine (B)" -width 16
	label $w.l9 -text "Water (W)" -width 16
	label $w.l10 -text "Heavy Water (V)" -width 16
	label $w.l11 -text "Methine (X)" -width 16
	for {set i 12} {$i<$nid} {incr i} {
		label $w.l$i -text "user defined" -width 16
	}
	label $w.lid -text "ID" -width 10
	for {set i 0} {$i<$nid} {incr i} {
		entry $w.eid$i -textvariable id($i) -width 2
	}
	label $w.lNSL -text "NSL" -width 10
	for {set i 0} {$i<$nid} {incr i} {
		entry $w.eb$i -textvariable b($i) -width 10
	}
	label $w.lnEl -text "nEl" -width 10
	for {set i 0} {$i<$nid} {incr i} {
		entry $w.ee$i -textvariable e($i) -width 10
	}
	grid $w.lAt $w.lid $w.lNSL $w.lnEl
	for {set i 0} {$i<$nid} {incr i} {
		grid $w.l$i $w.eid$i $w.eb$i $w.ee$i
	}
	pack $w
	for {set i 0} {$i<$nid} {incr i} {
		bind $w.eb$i <Return> {
			updateSIM
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.nft nft] && [is_it_NKactive .t.nft nftpExp$j]} {LoadDataN $fileExpN($j) $j; scaling yes n $j}
			}

		}
		bind $w.ee$i <Return> {
			updateSIM
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}

		}
	}
}

proc updateSIM {} {
	global fileSIM MaxComponent
	catch {LoadDataNew new $fileSIM 0}
	normalize
	updatevar
	if {[is_it_NKactive .t.nd nsld$MaxComponent] && [is_it_NKactive .t.nft nft]} {fourierN}
	if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
}

proc updateSLwin {} {
	global nid ffa1 ffa2 ffa3 ffa4 ffc
	global e
	for {set k 0} {$k<$nid} {incr k} {
		set e($k) [expr $ffa1($k) + $ffa2($k) + $ffa3($k) + $ffa4($k) + $ffc($k)]
	}
}

###################################################################################################
proc ANALYSISwin {} {
	global component MaxComponent Z_gibbs
	toplevel .analw
    wm title .analw "Analysis"
    set w .analw.comb
    frame $w
	button $w.bcomb -text "Combine Components" -borderwidth 3 -command {combine}
	pack $w.bcomb
	for {set i 0} {$i<$MaxComponent} {incr i} {
		entry $w.ename$i -textvariable component($i) -width 24
		pack $w.ename$i
		bind $w.ename$i <Return> { combine }
	}
#	pack $w
	
	set w .analw.gibbs
	frame $w
	set w0 $w.butt
	frame $w0
	button $w0.bgibbs -text "Gibb's Dividing Surface" -borderwidth 3 -command {gibbs $min $max $direction}
	pack $w0.bgibbs
	set w1 $w.range
	frame $w1
	label $w1.lrange -text "range:"
	entry $w1.emin -textvariable min -width 12
	entry $w1.emax -textvariable max -width 12
	pack $w1.lrange $w1.emin $w1.emax
	set w2 $w.direct
	frame $w2
	label $w2.ldirect -text "direction:"
	label $w2.ldirect1 -text "(+1 for increasing from 0 to 1)"
	label $w2.ldirect2 -text "(-1 for decreasing from 1 to 0)"
	entry $w2.edirect -textvariable direction -width 12
	label $w2.lgibbs -text "Z_gibbs = "
	entry $w2.egibbs -textvariable Z_gibbs -width 12 -state readonly
	pack $w2.ldirect $w2.ldirect1 $w2.ldirect2 $w2.edirect $w2.lgibbs $w2.egibbs
	pack $w0 $w1 $w2 -side top -anchor nw -expand yes -fill both
	bind $w1.emin <Return> {gibbs $min $max $direction}
	bind $w1.emax <Return> {gibbs $min $max $direction}
	bind $w2.edirect <Return> {gibbs $min $max $direction}
	
	pack .analw.comb .analw.gibbs -side left -anchor nw
}

proc combine {} {
	global component probw xsim Ncomponent MaxComponent ycombine
#	vector destroy ycombine
	vector create ycombine([yprob0 length])
	ycombine expr ycombine*0
	if {[$probw element exist combineel]==0} {
		$probw element create combineel -xdata xsim -ydata ycombine -pixels 0 -linewidth 1.5 -color navy
		$probw element configure combineel -label combine
		raiselement $probw combineel
	}
	for {set i 0} {$i<$MaxComponent} {incr i} {
		for {set j 0} {$j<$Ncomponent} {incr j} {
			if {[$probw element cget prob$j -label]==$component($i)} {ycombine expr ycombine+yprob$j}
		}
	}
}

###################################################################################################
proc AFFwin {} {
	global fileSIM fileExpX
	global MaxComponent MaxEXP
	global nid id ffa1 ffa2 ffa3 ffa4 ffb1 ffb2 ffb3 ffb4 ffc
	toplevel .affw
	wm title .affw "Atomic Form Factors"
	
	set w .affw.use
	frame $w
	label $w.labuse -text "use:"
	checkbutton $w.butAFF -variable atomf -offvalue 0 -onvalue 1
	label $w.labAFF -text "Atomic Form Factors"
	checkbutton $w.butPC -variable partialc -offvalue 0 -onvalue 1
	label $w.labPC -text "Partial Charges"
	pack $w.labuse $w.butAFF $w.labAFF $w.butPC $w.labPC -side left -anchor w
	
	set w .affw.tab
	tabset $w -side top -borderwidth 0 -selectpad 0 -tabborderwidth 1 -highlightthickness 0
	
	for {set i 0} {$i<$nid} {incr i} {
		set w .affw.tab.$i
		frame $w
		label $w._ -text ""
		label $w.1 -text "1"
		label $w.2 -text "2"
		label $w.3 -text "3"
		label $w.4 -text "4"
		label $w.a -text "a" -width 6
		label $w.b -text "b" -width 6
		label $w.c -text "c" -width 6
		entry $w.a1 -textvariable ffa1($i) -width 12
		entry $w.a2 -textvariable ffa2($i) -width 12
		entry $w.a3 -textvariable ffa3($i) -width 12
		entry $w.a4 -textvariable ffa4($i) -width 12
		entry $w.b1 -textvariable ffb1($i) -width 12
		entry $w.b2 -textvariable ffb2($i) -width 12
		entry $w.b3 -textvariable ffb3($i) -width 12
		entry $w.b4 -textvariable ffb4($i) -width 12
		entry $w.ec -textvariable ffc($i) -width 12
		grid $w._ $w.1 $w.2 $w.3 $w.4
		grid $w.a $w.a1 $w.a2 $w.a3 $w.a4
		grid $w.b $w.b1 $w.b2 $w.b3 $w.b4
		grid $w.c $w.ec
	}
	set w .affw.tab
	for {set i 0} {$i<$nid} {incr i} {
		$w insert $i $id($i) -window $w.$i -padx 0 -pady 0
	}
	pack .affw.use .affw.tab -side top
	for {set i 0} {$i<$nid} {incr i} {
		set w .affw.tab.$i
		bind $w.a1 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.a2 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.a3 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.a4 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.b1 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.b2 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.b3 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.b4 <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}
		bind $w.ec <Return> {
			if {$atomf} updateSLwin
			catch {LoadDataNew new $fileSIM 0}
			normalize
			updatevar
			if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
			for {set j 0} {$j<$MaxEXP} {incr j} {
				if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
			}
		}		
	}
}
###########################################################################
proc updateFF {bvector position operation} {
	global MaxComponent MaxEXP fileExpX
	updateSLwin
	updateSIM
	if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
	for {set j 0} {$j<$MaxEXP} {incr j} {
		if {[is_it_NKactive .t.xft xft] && [is_it_NKactive .t.xft xftpExp$j]} {LoadDataX $fileExpX($j) $j; scaling yes x $j}
	}
}
trace add variable atomf write updateFF
trace add variable partialc write updateFF

###########################################################################
proc updateSYM {bvector position operation} {
	global fileSIM MaxComponent LoadCmd
	catch {LoadDataNew new $fileSIM 0}
	normalize
	updatevar
	if {[is_it_NKactive .t.nd nsld$MaxComponent] && [is_it_NKactive .t.nft nft]} {fourierN}
	if {[is_it_NKactive .t.ed ed$MaxComponent] && [is_it_NKactive .t.xft xft]} {fourierX}
	if {$LoadCmd!=0} {fit}
}
trace add variable symmetrized write updateSYM

###########################################################################
############### initialize different variables ############################
###########################################################################

#set area 72.4
#set Nlip 288
#set binstep 0.2
set area 1
set Nlip 2
set binstep 1
set norm 1
set Zcentre 0
set Zdirection +1

for {set i 0} {$i<10} {incr i} {
  set natpgr($i) 0
  set nat($i) 0
  set name($i) "                "
}

set MAXnid 20
set nid 16
#set atomf 1
#set partialc 1

for {set i 0} {$i<$MAXnid} {incr i} {
	set id($i) [expr $i-10]
	set b($i) 0
	set e($i) 0
	set ffa1($i) 0.0
	set ffa2($i) 0.0
	set ffa3($i) 0.0
	set ffa4($i) 0.0
	set ffb1($i) 0.0
	set ffb2($i) 0.0
	set ffb3($i) 0.0
	set ffb4($i) 0.0
	set ffc($i) 0.0
}

for {set i 0} {$i<$MaxEXP} {incr i} {
	set xSCfix($i) 0
	set xBGfix($i) 1
	set xscale($i) 1.0
	set xbckgrnd($i) 0.0
	set xCHI2($i) 0.0
	set nSCfix($i) 0
	set nBGfix($i) 1
	set nscale($i) 1.0
	set nbckgrnd($i) 0.0
	set nCHI2($i) 0.0
}

set LoadCmd 0
set LoadData 0
set Ncomponent 0

source tcl/init.tcl

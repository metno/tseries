#!/bin/sh

##############################################################################
#
# Script for meteogram postscript files.
#
# DNMI/EDB 03.12.96 Rebecca Rudsar
# DNMI/FOU 09.12.03 Audun Christoffersen
#
##############################################################################
set +x

if test $# -ne 8 ; then
   echo "usage:"
   echo "$0 btseries /opdata/autofax autofax_ec120_054.datfax 50s 2 4 landscape a3"
   echo "or"
   echo "$0 btseries /opdata/autofax autofax_ec120_054.datfax ec  2 4 portrait  a4"
   exit
fi

program=$1
auto=$2'/'$3
filename=`echo $3 | cut -f1 -d"."`
model=$4
nx=$5
ny=$6
paper=$7
printer=$8

error=0

echo " "
echo " Program     = $program "
echo " Input file  = $auto  "   
echo " Output file = $filename (ext.: .ps)"
echo " nx          = $nx    ny        = $ny "
echo " Paper       = $paper       Printer     = $printer "
echo " "

tmpdir='tmp.'$$
mkdir $tmpdir
cd $tmpdir

# multiple plots on one page?
multiplot=0
smallplot=0
test $nx -ne 1 && multiplot=1
test $ny -ne 1 && multiplot=1
test $nx -ne 1 && smallplot=1

jobdir='/metno/local/tseries/batch'
cbindir='/metno/local/bin'
printfile='tmp.ps'
poseinput="$jobdir/pose.input"
modelname='AUTO'
diatype='metdat'
parameter="$jobdir/parameters.def"
xvfbp="/usr/bin/X11/Xvfb"

# Make btseries.input file.
input='btseries.input'

rm -f $input 2> /dev/null

echo "setupfile=$jobdir/tseries.setup"    > $input
echo "output=POSTSCRIPT"            >> $input
echo "colour=GREYSCALE"             >> $input
#echo "colour=COLOUR"                >> $input
#echo "printer=fou3e1"              >> $input
echo "drawbackground=NO"            >> $input

if test $paper = 'landscape'
then
    echo "orientation=LANDSCAPE"    >> $input
    if test $smallplot -ne 0
    then
	echo "buffersize=1696x1200" >> $input
    else
	echo "buffersize=800x600"   >> $input
    fi
elif test $paper = 'portrait'
then
    echo "orientation=PORTRAIT"     >> $input
    if test $smallplot -ne 0
    then
	echo "buffersize=1200x1696" >> $input
    else
	echo "buffersize=600x800"   >> $input
    fi
else
    error=1
fi

if test $printer = 'a3'
then
    echo "PAPERSIZE=A3"             >> $input

elif test $printer = 'a4'
then
    echo "PAPERSIZE=A4"             >> $input
else
    error=1
fi

if test $error -ne 0
then
    echo "$0: ERROR in $printer or $paper"
    exit
fi


echo "MODEL=$modelname"            >> $input
diatype='metdat'

#if test $model = "50s"
#then
#    echo "MODEL=$modelname"        >> $input
#    diatype='metdat'
#elif test $model = "ec"
#then
#    echo "MODEL=$modelname"        >> $input
#    diatype='metdat'
#fi

poseinput="$jobdir/pose.$diatype"

echo "RUN=UNDEF"                    >> $input
echo "DIAGRAMTYPE=Meteogram"        >> $input
echo "FILENAME=$printfile"          >> $input

if test $multiplot -ne 0
then
    echo "POSLOOP [POS] MULTIPLE.PLOTS=$ny,$nx,2,0 [ROW] [COL]" >> $input
else
    echo "POSLOOP [POS] "           >> $input
fi

echo "POSITION=[POS]"               >> $input   
test $multiplot -ne 0 && echo "PLOTCELL=[ROW],[COL]"  >> $input
echo "PLOT"                         >> $input
echo "POSLOOP.END"                  >> $input

# Make HDF from .dat
$cbindir/pose $poseinput $parameter $auto tmp.hdf $modelname

###########################################
# Find X-DISPLAY
###########################################
if test $JOB_XDISPLAY 
then
    xdisplay=$JOB_XDISPLAY
    xpid=0
else
    # Start X virtual frame buffer
    echo "\$JOB_XDISPLAY not defined - trying to start Xvfb.."
    xdis=1
    for xdis in 1 2 3 4 5 6 7 8 9
    do
	$xvfbp :$xdis -screen 0 1280x1024x24 -ac >_xvfb.out 2>&1 &
	xpid=$!
	if test $? -ne 0
	then
	    echo "$0: ERROR could not start Xvfb at this time"
	    exit
	fi
	sleep 1s
	res=`grep -c "already active" _xvfb.out`
	if test $res -eq 0
	then
	    xdisplay=":$xdis."
	    echo "Xvfb running with display=$xdisplay"
	    break
	fi
    done
fi

# fetch diagram-styles
cp $jobdir/style.* .

# Make diagrams
echo "Starting $cbindir/$program with Xdisplay=$xdisplay"
$cbindir/$program -input $input -display $xdisplay

if test $xpid -ne 0
then
    # kill Xvfb
    echo "Killing Xvfb, pid=$xpid"
    kill $xpid
fi

# Printing
ls -l $printfile

retcode=0
if [ -s $printfile ]
then
	mv $printfile '../'$filename'.ps'
else
	retcode=1
fi

# Clean up

rm -f $input

cd '../'
rm -rf $tmpdir

echo "$0 exit $retcode"

exit $retcode

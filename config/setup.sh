#!/bin/bash

source /grid/fermiapp/products/uboone/setup_uboone.sh
echo "Setting up uboonecode"
setup uboonecode v07_13_00 -q e17:prof
#setup uboonecode v07_05_00_02 -q e17:prof
echo "Setting up larsoftobj"
setup larsoftobj v07_08_00 -q e17:prof
source /uboone/app/users/cadams/pystack2/setup.sh

# # clean up previously set env
# me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd)"
# `python $me/python/clean_env.py LD_LIBRARY_PATH`
# `python $me/python/clean_env.py DYLD_LIBRARY_PATH`
# `python $me/python/clean_env.py PYTHONPATH`
# `python $me/python/clean_env.py PATH`
# if [[ -z $FORCE_LARLITE_BASEDIR ]]; then
#     # If LARLITE_BASEDIR not set, try to guess
#     # Find the location of this script:
#     me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
#     # Find the directory one above.
#     export LARLITE_BASEDIR="$( cd "$( dirname "$me" )" && pwd )"
# else
#     export LARLITE_BASEDIR=$FORCE_LARLITE_BASEDIR
# fi
# unset me;

# if [[ -z $ENABLE_LARLITE_ALIAS ]]; then
#     export ENABLE_LARLITE_ALIAS=1
# fi

me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Find the directory one above.
export GALLERY_FMWK_BASEDIR="$( cd "$( dirname "$me" )" && pwd )"

echo "GALLERY_FMWK_BASEDIR = $GALLERY_FMWK_BASEDIR"
if [[ -z $GALLERY_FMWK_BASEDIR ]]; then
    echo \$GALLERY_FMWK_BASEDIR not set! 
    echo You have to set this first.
    return;
fi
# Abort if ROOT not installed. Let's check rootcint for this.
if [ `command -v rootcling` ]; then
    export GALLERY_FMWK_ROOT6=1
else 
    if [[ -z `command -v rootcint` ]]; then
        echo
        echo Looks like you do not have ROOT installed.
        echo You cannot use GALLERY_FMWK w/o ROOT!
        echo Aborting.
        echo
        return;
    fi
fi
# Abort if GALLERY not installed. 
if [[ -z $GALLERY_INC ]]; then
    echo
    echo Looks like you do not have GALLERY installed.
    echo You cannot use GALLERY_FMWK w/o GALLERY!
    echo Aborting.
    echo
    return;
fi

GALLERY_FMWK_OS=`uname -s`

# Set path to sub directories
export GALLERY_FMWK_LIBDIR=$GALLERY_FMWK_BASEDIR/lib
export GALLERY_FMWK_COREDIR=$GALLERY_FMWK_BASEDIR/core
export GALLERY_FMWK_USERDEVDIR=$GALLERY_FMWK_BASEDIR/UserDev

if [[ -z $USER_MODULE ]]; then
    export USER_MODULE=""
fi

# Check compiler availability for clang++ and g++
GALLERY_FMWK_CXX=g++
if [ `command -v $GALLERY_FMWK_CXX` ]; then
    export GALLERY_FMWK_CXX="$GALLERY_FMWK_CXX -std=c++11";
else
    GALLERY_FMWK_CXX=g++
    if [[ -z `command -v $GALLERY_FMWK_CXX` ]]; then
        echo
        echo Looks like you do not have neither clang or g++!
        echo You need one of those to compile GALLERY_FMWK... Abort config...
        echo
        return;
    fi
    export GALLERY_FMWK_CXX;
    if [ $GALLERY_FMWK_OS = 'Darwin' ]; then
        echo $GALLERY_FMWK_OS
        echo
        echo "***************** COMPILER WARNING *******************"
        echo "*                                                    *"
        echo "* You are using g++ on Darwin to compile Gallery Framework.    *"
        echo "* Currently Gallery Framework assumes you do not have C++11    *"
        echo "* in this combination. Contact the author if this is *"
        echo "* not the case. At this rate you have no ability to  *"
        echo "* compile packages using C++11 in Gallery Framework.           *"
        echo "*                                                    *"
        echo "* Help to install clang? See manual/contact author!  *"
        echo "*                                                    *"
        echo "******************************************************"
        echo 
    fi
fi
if [[ -z $ROOTSYS ]]; then
    case `uname -n` in
        (houston.nevis.columbia.edu)
            if [[ -z ${ROOTSYS} ]]; then
                source /usr/nevis/adm/nevis-init.sh
                setup root
                export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
            fi
            ;;
        (*)
            echo
            echo "****************** PyROOT WARNING ********************"
            echo "*                                                    *"
            echo "* Did not find your \$ROOTSYS. To use PyROOT feature, *"
            echo "* Make sure ROOT.py is installed (comes with ROOT).  *"
            echo "* You need to export \$PYTHONPATH to include the dir  *"
            echo "* where ROOT.py exists.                              *"
            echo "*                                                    *"
            echo "* Help to install PyROOT? See manual/contact author! *"
            echo "*                                                    *"
            echo "******************************************************"
            echo
            ;;
        esac
else
    export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH;
fi



# python $LARLITE_BASEDIR/config/python/gen_usermakefile.py
# python $LARLITE_BASEDIR/config/python/gen_topmakefile.py
export LD_LIBRARY_PATH=$GALLERY_FMWK_LIBDIR:$LD_LIBRARY_PATH
# if [ $LARLITE_OS = 'Darwin' ]; then
#     export DYLD_LIBRARY_PATH=$LARLITE_LIBDIR:$DYLD_LIBRARY_PATH
# fi
export PATH=$GALLERY_FMWK_BASEDIR/bin:$PATH
# if [ $ENABLE_LARLITE_ALIAS ]; then
#     alias maketop="make --directory=$LARLITE_BASEDIR"
#     alias cdtop="cd $LARLITE_BASEDIR"
#     alias llgen_package="python $LARLITE_BASEDIR/bin/gen_package"
#     alias llgen_repository="python $LARLITE_BASEDIR/bin/gen_repository"
#     alias llgen_class_empty="python $LARLITE_BASEDIR/bin/gen_class_empty"
#     alias llgen_class_anaunit="python $LARLITE_BASEDIR/bin/gen_class_anaunit"
#     alias llgen_class_erana="python $LARLITE_BASEDIR/bin/gen_class_erana"
#     alias llgen_class_eralgo="python $LARLITE_BASEDIR/bin/gen_class_eralgo"
#     alias llgen_class_erfilter="python $LARLITE_BASEDIR/bin/gen_class_erfilter"
# fi

if [ -z ${GALLERY_FMWK_BASEDIR+x} ]; then 
  echo "Must set up gallery framework to use this!";
  return 
fi

#SOURCE="${BASH_SOURCE[0]}"
#while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
#  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"
#  SOURCE="$(readlink "$SOURCE")"
#  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
#done
#DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

# This section extends the path and python path to run from anywhere
export DIR=$GALLERY_FMWK_USERDEVDIR/EventDisplay/


# This section verifies that python dependences are setup 

PYTHONPATH_backup=$PYTHONPATH
PATH_backup=$PATH

if [[ ! ":$PATH:" == *":$DIR/python:"* ]]; then
  export PATH=$DIR/python:$PATH
fi

if [[ ! ":$PYTHONPATH:" == *":$DIR/python:"* ]]; then
  export PYTHONPATH=$DIR/python/:$PYTHONPATH
fi

# Test argparse
if ! $(python -c "import argparse" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package argparse"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup
  return
fi

# Test numpy
if ! $(python -c "import numpy" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package numpy"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup 
  return
fi

# Test pyqt4
if ! $(python -c "import pyqtgraph.Qt" &> /dev/null); then 
  echo "Warning: can not use evd due to missing package PyQt"
  export PATH=$PATH_backup
  export PYTHONPATH=$PYTHONPATH_backup
  return
fi




export BUILD_GALLERY_FMWK_EVD=true




echo
echo "Finish configuration. To build, type:"
echo "> cd \$GALLERY_FMWK_BASEDIR"
echo "> make"
echo



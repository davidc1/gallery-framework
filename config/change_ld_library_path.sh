#!/bin/bash

export LD_LIBRARY_PATH=$(cat $GALLERY_FMWK_BASEDIR/config/ldpathout.txt)

rm $GALLERY_FMWK_BASEDIR/config/ldpath.txt
rm $GALLERY_FMWK_BASEDIR/config/ldpathout.txt
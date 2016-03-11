#!/bin/zsh

BLENDER_SCRIPT_PATH=/Applications/blender/blender.app/Contents/Resources/2.76/scripts/addons/io_brick

echo $BLENDER_SCRIPT_PATH

mkdir $BLENDER_SCRIPT_PATH
cp ./io_mesh/export_brick.py $BLENDER_SCRIPT_PATH
cp ./io_mesh/import_brick.py $BLENDER_SCRIPT_PATH
cp ./io_mesh/__init__.py $BLENDER_SCRIPT_PATH

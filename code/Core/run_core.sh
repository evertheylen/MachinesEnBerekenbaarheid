
# TODO: dynamically set this variable?
PYTHON_LIB=$PWD/../libs/python3

if [ ! -f $PYTHON_LIB/libpython3.4m.a ]; then
	echo "Could not find python in $PYTHON_LIB, try setting a different path."
	exit 1
fi

# DANGER no Mac support for the better of Humanity
export PYTHONPATH=$PYTHON_LIB/Lib:$PYTHON_LIB/Modules:$PYTHON_LIB/build/lib.linux-x86_64-3.4:$PYTHON_LIB
export PYTHONHOME=$PYTHON_LIB/Lib:$PYTHON_LIB/Modules:$PYTHON_LIB/build/lib.linux-x86_64-3.4:$PYTHON_LIB
export PYTHONSTARTUP=$PYTHON_LIB/Lib:$PYTHON_LIB/Modules:$PYTHON_LIB/build/lib.linux-x86_64-3.4:$PYTHON_LIB

export PATH=$PYTHON_LIB/Lib:$PYTHON_LIB/Modules:$PYTHON_LIB/build/lib.linux-x86_64-3.4:$PYTHON_LIB:$PATH

./core $@

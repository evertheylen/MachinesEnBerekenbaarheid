
# TODO: dynamically set this variable?
PYTHON_LIB=../libs/python3

if [ ! -f $PYTHON_LIB/libpython3.5m.a ]; then
	echo "Could not find python in $PYTHON_LIB, try setting a different path."
	exit 1
fi

# TODO Mac support?
export PYTHONPATH=$PYTHON_LIB/Lib/:$PYTHON_LIB/Modules/:$PYTHON_LIB/build/lib.linux-x86_64-3.5/

./core $@

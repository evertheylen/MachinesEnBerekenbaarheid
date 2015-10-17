MBExec.o: MBExec/MBExec.cpp MBExec/../MBLib/CFG.hpp \
 MBExec/../MBLib/../libs/tinyxml/tinyxml.h \
 MBExec/../libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

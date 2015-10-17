CFG.o: CFG.cpp CFG.hpp ../libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

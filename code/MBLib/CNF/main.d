main.o: CNF/main.cpp CNF/../CFG.hpp CNF/../../libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

CNF.o: CNF/CNF.cpp CNF/../CFG.hpp CNF/../../libs/tinyxml/tinyxml.h \
 CNF/CNF.hpp
	$(CC) $(CXXFLAGS) -c $< -o $@

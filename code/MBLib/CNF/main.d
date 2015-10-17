main.o: CNF/main.cpp CNF/CNF.hpp CNF/../CFG.hpp \
 CNF/../../libs/tinyxml/tinyxml.h CNF/../libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

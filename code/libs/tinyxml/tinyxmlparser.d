tinyxmlparser.o: libs/tinyxml/tinyxmlparser.cpp libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

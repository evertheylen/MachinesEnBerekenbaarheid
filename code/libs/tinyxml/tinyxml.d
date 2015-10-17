tinyxml.o: libs/tinyxml/tinyxml.cpp libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

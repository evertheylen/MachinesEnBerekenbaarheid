tinystr.o: libs/tinyxml/tinystr.cpp libs/tinyxml/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@

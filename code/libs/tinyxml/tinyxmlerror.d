tinyxmlerror.o: libs/tinyxml/tinyxmlerror.cpp libs/tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@

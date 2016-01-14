#!/bin/bash

# Don't redo bootstrap.sh (in boost)

# If it doesn't work, execute bootstrap.sh and modify project-config.jam, change the section 'Python Configuration' to:
#  # Python configuration
#  import python ;
#  using python : 3.4 : /usr ;


cd libs/boost/
mkdir -p b2stuff
./b2 link=static install --prefix=b2stuff


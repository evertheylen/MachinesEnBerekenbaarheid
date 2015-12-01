g++ -std=c++11 -I ../code/libs/python3/Include -I../code/libs/boost/b2stuff/include/ main.cpp  -o main.out -pthread -lutil -ldl -lm -L../code/libs/python3 -lpython3.5m -L../code/libs/boost/b2stuff/lib/ -lboost_python3


g++ -std=c++11 -I /usr/include/python3.5m  -ldl -lutil -lm -lpython3.5m -L/usr/lib -lboost_python3 -Xlinker -export-dynamic main.cpp -o main.out

CC = g++
CFLAGC = -std=c++11  -c
CFLAGO = -std=c++11 -o

main : Structure4LQSR.o mainTester.o
	$(CC) $(CFLAGO)   main Structure4LQSR.o mainTester.o

Structure4LQSR.o :  Structure4LQSR.cpp Structure4LQSR.h
	$(CC) $(CFLAGC) Structure4LQSR.cpp

Literal_Tester.o : Test/Literal_Tester.cpp Test/Literal_Tester.h
	$(CC) $(CFLAGC) Test/Literal_Tester.cpp

Operators_Tester.o : Test/Operators_Tester.cpp Test/Operators_Tester.h
	$(CC) $(CFLAGC) Test/Operators_Tester.cpp

Var_Tester.o : Test/Var_Tester.cpp Test/Var_Tester.h
	$(CC) $(CFLAGC) Test/Var_Tester.cpp

VariablesSet_Tester.o :	Test/VariablesSet_Tester.cpp    Test/VariablesSet_Tester.h
	$(CC) $(CFLAGC) Test/VariablesSet_Tester.cpp	

mainTester.o : Test/mainTester.cpp  Literal_Tester.o Operators_Tester.o Var_Tester.o VariablesSet_Tester.o
	$(CC) $(CFLAGC)  Test/mainTester.cpp

clean:
	rm *.o program
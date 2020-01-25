#include <string>
#include <sstream>
#include "Literal_Tester.h"



int testLiteral_getElements(Literal &literal, Var* &var)
{ 
	vector<Var*> temp = literal.getElements();
	for (Var* v : temp)
	{
		if ((v->getName()).compare(var->getName()) == 0)
			if (v->getIndex() == var->getIndex())
				if (v->getSort() == var->getSort())
					if (v->getVarType() == var->getVarType())
						if (v->toString().compare(var->toString())==0)
						{
							 return 0;
						}
	}

	return 1;
}

int testLiteral_getLiteralOp(Literal &literal, int checkLiteralOp)
{
	if (literal.getLiteralOp() == checkLiteralOp)
		return 0;
	return 1;
}

int testLiteral_setLiteralOp(Literal &literal, int checkLiteralOp)
{
	literal.setLiteralOp(checkLiteralOp);
	return testLiteral_getLiteralOp(literal, checkLiteralOp);
}

int testLiteral_getElementAt(Literal &literal, int position, Var* check)
{
	if ( (literal.getElementAt(position))->equal(check) == 0)
		return 0;
	return 1;
}

int testLiteral_equals(Literal &literal, Literal& literal2, int check)
{
	if (literal.equals(literal2)==check)
		return 0;
	return 1;
}

int testLiteral_addElement(Literal literal, Var* var)
{
	literal.addElement(var);
	if (literal.getElementAt( ((int)literal.getElements().size()) - 1)->equal(var) == 0)
		return 0;
	return 1;

}

int testLiteral_setElementAt(Literal literal, Var* var, int position)
{
	literal.setElementAt(position, var);
	if (literal.getElementAt(position)->equal(var) == 0)
		return 0;
	return 1;
}


void testLiteral(vector<pair<string, int>>& results)
{	
	//Testing literal instantiation and getElement
	Var* var1 = new Var("x_0", 0, 0, 0);
	Var* var2 = new Var("x_1", 0, 0, 0);
	Var* var3 = new Var("X_1", 0, 1, 0);
	Literal literal = Literal(0, vector<Var*>{var1, var2, var3});	
	results.push_back(std::make_pair("testLiteral_getElements input ({"+ var1->toString()+ ", "+var2->toString()+", " + var3->toString()+", 0}"+","+var2->toString()+")", testLiteral_getElements(literal, var2)));
	
	//Test getLiteralOp
	results.push_back(std::make_pair("testLiteral_getLiteralOp input ({" + var1->toString() + ", " + var2->toString() + ", " + var3->toString() + ",0}" + "," + "0)", testLiteral_getLiteralOp(literal, 0)));
	
	//Test setLiteralOp
	delete var1, var2, var3;

	var1 = new Var("y_0", 0, 0, 3);
	var2 = new Var("y_1", 0, 0, 2);
	var3 = new Var("y_1", 0, 1, 1);
	literal = Literal(0, vector<Var*>{var1, var2, var3});

	results.push_back(std::make_pair("testLiteral_setLiteralOp input ({" + var1->toString() + ", " + var2->toString() + ", " + var3->toString() + ",0}" + "," + "1)", testLiteral_setLiteralOp(literal, 1)));

	//Testing getElementAt
	delete var1, var2, var3;
	var1 = new Var("z_0", 0, 0, 3);
	var2 = new Var("z_0", 0, 0, 2);
	var3 = new Var("z_1", 0, 1, 1);

	literal = Literal(0, vector<Var*>{var1, var2, var3});
	results.push_back(std::make_pair("testLiteral_getElementAt input ({" + var1->toString() + ", " + var2->toString() + ", " + var3->toString() + ",0}" + "," + "2,"+var3->toString()+")", testLiteral_getElementAt(literal, 2, var3)));

	Var* var1b = new Var("z_0", 0, 0, 3);
	Var* var2b = new Var("z_0", 0, 0, 2);
	Var* var3b = new Var("z_1", 0, 1, 1);
	Literal literal2 = Literal(0, vector<Var*>{var1b, var2b, var3b});
	//Testing equals
	results.push_back(std::make_pair("testLiteral_equals input ({" + var1->toString() + ", " + var2->toString() + ", " + var3->toString() + ",0}," + 
	                                                            "{"+ var1->toString() + ", " + var2->toString() + ", " + var3->toString() + ",0}"+")", testLiteral_equals(literal, literal2, 0)));
	//Testing addElement
	literal2 = Literal();
	results.push_back(std::make_pair("testLiteral_addElement input " + var3->toString(), testLiteral_addElement(literal2, var3b)));
	
	literal2 = Literal(0, vector<Var*>{var1b, var2b, var3b});
	//Testing setElementAt
	results.push_back(std::make_pair("testLiteral_setElementAt input " + var3->toString()+", 1", testLiteral_setElementAt(literal2, var3b, 1)));
}
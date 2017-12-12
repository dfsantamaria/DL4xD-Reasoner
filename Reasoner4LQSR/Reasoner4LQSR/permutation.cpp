// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h> 
#include <vector> 
#include <string>   
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;


void show( vector<int>&a, int n)
{
	int i;
	for (i = 0; i < n; i++)
		printf("%1d", a.at(i));
	printf("\n");
}

void gen_all_numbers(int r, int n, vector<int>& b)
{
	int i;	
	if (r < 2 || n < 1 )          /* parameter check */
		return;
	r -= 1;                    /* r = max digit value */	
	do
	{		
		i = n - 1;
		while ( b.at(i) < r) 
		{        /* increment last digit */
			b[i]++;
			show(b, n);
		}
		/* find next digit to increment */
		while (i >= 0 && b.at(i) == r)
			i--;
		if (i < 0)
			break;         /* return if done */
		b.at(i)++;
		while (++i < n)          /* zero following digits */
			b.at(i) = 0;
		show(b, n);
	} 
	while (1);
	//free(a);
}

int main()
{
	vector<int>b = { 1,1 };
	gen_all_numbers(3,2,b);
	return 0;
}
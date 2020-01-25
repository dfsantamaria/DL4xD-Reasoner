#pragma once
#include <string>
#include <vector>
#include "..\Structure4LQSR.h"

int test_getLogConnectSize(Operators& operators, int aspected);
int test_getSetOpSize(Operators& operators, int aspected);
int test_getQuantSize(Operators& operators, int aspected);
int test_getLogConnectValue(Operators& operators, string connect, int aspected);
int test_getSetOpValue(Operators& operators, string connect, int aspected);
int test_getQuantValue(Operators& operators, string connect, int aspected);
int test_getLogConnectElement(Operators& operators, int position, string aspected);
int test_getSetOpElement(Operators& operators, int position, string aspected);

void testOperators(vector<pair<string, int>>& results);


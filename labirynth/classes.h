
#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <iostream>
using namespace std;
class CorridorBottom
{
	int X, Y;
public:

	CorridorBottom(int startX, int startY, vector<unsigned char> colour);
};
class CorridorRight
{
	int X, Y;
public:

	CorridorRight(int startX, int startY, vector<unsigned char> colour);
};
class CorridorLeft
{
	int X, Y;
public:
	CorridorLeft(int startX, int startY, vector<unsigned char> colour);
};
class CorridorTop
{
	int X, Y;
public:

	CorridorTop(int startX, int startY, vector<unsigned char> colour);
};
// labirynth.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <math.h>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
// zrobic by kolorowalo

//rdzen kodu z trojkata sierpinskiego
using namespace std;
FILE* fp;
const int iYmax = 962;
/* screen ( integer) coordinate */
const int iXmax = 962;
unsigned char ImageMatrix[iXmax][iYmax][3];
char filename[] = "image.ppm";
char comment[] = "# ";/* comment should start with # */
int MaxColorComponentValue = 255;
int width = 0; //zakladam ze szerokosc wszedzie ta sama
void CorridorTop(int, int, vector<unsigned char>);
void CorridorBottom(int, int, vector<unsigned char>);
void CorridorRight(int, int, vector<unsigned char>);
void CorridorLeft(int,int,vector<unsigned char>);
mutex mut;
vector<unsigned char> changeColours(vector<unsigned char> colour)
{
	vector<unsigned char> RGB = colour;
	for (int i = 0;i < 3;i++)
	{
		if (RGB[i] != 0)
		{
			if (RGB[i] >= 200)
			{
				RGB[i] = 0;
				if (i == 2)
				{
					RGB[0] = rand() % 50 + 50;
				}
				else
				{
					RGB[i + 1] = rand() % 50 + 50;
				}
			}
			else
			{
				RGB[i] = RGB[i] + rand() % 50 + 10;
			}
		}
	}
	return RGB;
}



bool checkIfWhite(int X, int Y)
{
	if (X < 0 || Y < 0)
	{
		return false;
	}
	if (ImageMatrix[X][Y][0] == 255 && ImageMatrix[X][Y][1] == 255 && ImageMatrix[X][Y][2] == 255)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool checkifBlack(int X, int Y)
{
	if (X < 0 || Y < 0)
	{
		return false;
	}
	if (ImageMatrix[X][Y][0] == 0 && ImageMatrix[X][Y][1] == 0 && ImageMatrix[X][Y][2] == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int seekX()//rozpatrywany labirynt ma "wejscie" o Y = 0
{
	int X = 0;
	while (checkIfWhite(X, 0) == false)
	{
		X++;
	}
	int temp = X;
	while (checkIfWhite(temp, 0) == true)
	{
		temp++;
		width++;
	}
	return X;
}
void setColours(vector<unsigned char> colour,int X,int Y)
{
	mut.lock();
	for (int i = 0;i < 3;i++)
	{
		ImageMatrix[X][Y][i] = colour[i];
	}
	mut.unlock();
}
void CorridorBottom(int X, int Y, vector<unsigned char> colour)	//ZAWSZE LECI PO LEWEJ SCIANCE
{
	bool ifRight = rand() % 2;	//w ktora strone kontynuowac
	while (checkifBlack(X, Y) == false)
	{
		if (checkIfWhite(X + width + 1, Y) == true)	//czy jest rozgalezienie w prawo
		{
			if (checkifBlack(X + width + 1, Y + 1) == true&&checkifBlack(X, Y +1) == true) //czy dalej leci do przodu
			{
				if (checkIfWhite(X - 1, Y) == true)	//czy jest rozgalezenie w lewo
				{
					if (ifRight == 1)	//sprawdza czy prawy to priorytet
					{

						CorridorRight(X + width, Y, colour);
					}
					else
					{
						//CorridorRight(X + width, Y, changeColours(colour));
						thread childCorridor(CorridorRight, X + width, Y, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorRight(X + width, Y, colour);

				}
			}
			else if(checkifBlack(X + width + 1, Y + 1) == true)	//leci dalej do przodu
			{
				//CorridorRight(X + width, Y, changeColours(colour));
				thread childCorridor(CorridorRight, X + width, Y, changeColours(colour));
				childCorridor.join();
			}
		}
		if (checkIfWhite(X -1, Y) == true)	//czy jest rozgalezienie
		{
			if (checkifBlack(X - 1, Y + 1) == true && checkifBlack(X, Y + 1) == true)
			{

				if (checkifBlack(X + width + 1, Y) == false)	//czy jest rozgalezienie w prawo tez
				{

					if (ifRight == 0)
					{

						CorridorLeft(X - 1, Y, colour);
					}
					else
					{

						//CorridorLeft(X - 1, Y, changeColours(colour));
						thread childCorridor(CorridorLeft, X - 1, Y, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorLeft(X - 1, Y, colour);

				}
			}
			else if (checkifBlack(X - 1, Y + 1) == true)
			{
				//CorridorLeft(X - 1, Y, changeColours(colour));
				thread childCorridor(CorridorLeft, X - 1, Y, changeColours(colour));
				childCorridor.join();
			}
		}
		for (int i = 0;i < width;i++)
		{
			setColours(colour, X + i, Y);
		}
		Y++;
	}
}
void CorridorRight(int X, int Y, vector<unsigned char> colour)	//ZAWSZE LECI PO DOLNEJ SCIANCE
{
	bool ifBottom = rand() % 2;
	while (checkifBlack(X, Y) == false)
	{
		if (checkIfWhite(X, Y+1) == true)	//czy jest rozgalezienie
		{
			if (checkifBlack(X - 1, Y + 1) == true && checkifBlack(X+width, Y) == true)	//czy to ta sama sciezka
			{
				if (checkIfWhite(X, Y - width - 1) == true)
				{
					if (ifBottom == 1)
					{
						CorridorBottom(X, Y + 1, colour);
					}
					else
					{
						//CorridorBottom(X, Y + 1, changeColours(colour));
						thread childCorridor(CorridorBottom, X, Y+1, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorBottom(X, Y + 1, colour);
				}
			}
			else if (checkifBlack(X - 1, Y + 1) == true)	//tworzenie dziecka o innym kolororze
			{
				//CorridorBottom(X, Y+1, changeColours(colour));
				thread childCorridor(CorridorBottom, X, Y + 1, changeColours(colour));
				childCorridor.join();
			}
		}
		if (checkIfWhite(X, Y - width - 1) == true)	//czy jest rozgalezienie
		{
			if (checkifBlack(X - 1, Y - width - 1) == true && checkifBlack(X + width, Y) == true)
			{
				if (checkifBlack(X, Y + 1) == false)
				{
					if (ifBottom == 0)
					{
						CorridorTop(X, Y - width, colour);	//jesli losowo wybrano rozgalezienie gorne

					}
					else
					{
						//CorridorTop(X, Y - width, changeColours(colour));
						thread childCorridor(CorridorTop, X, Y - width, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorTop(X, Y - width, colour);
				}
			}
			else if (checkifBlack(X - 1, Y - width) == true)
			{
				thread childCorridor(CorridorTop, X, Y - width, changeColours(colour));
				childCorridor.join();
			}
		}
		for (int i = 0;i < width;i++)
		{
			setColours(colour, X, Y-i);

		}
		X++;
	}
}
void CorridorLeft(int X, int Y, vector<unsigned char> colour)
{
	bool ifBottom = rand() % 2;
	while (checkifBlack(X, Y) == false)
	{
		if (checkIfWhite(X, Y + 1) == true)	//czy jest rozgalezienie
		{
			
			if (checkifBlack(X + 1, Y + 1) == true && checkifBlack(X - width, Y) == true)
			{
				if (checkifBlack(X, Y - 1-width) == false)	//czy jest rozgalezienie do gory
				{
					if (ifBottom == 1)
					{
						CorridorBottom(X - width+1, Y + 1, colour);	//jesli losowo wybrano rozgalezienie gorne

					}
					else
					{
						//CorridorBottom(X - width+1, Y + 1, changeColours(colour));
						thread childCorridor(CorridorBottom, X-width+1, Y +1, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{

					CorridorBottom(X - width + 1, Y + 1, colour);
				}
			}
			else if (checkifBlack(X + 1, Y + 1) == true)
			{

				thread childCorridor(CorridorBottom, X - width + 1, Y + 1, changeColours(colour));
				childCorridor.join();	//czy jest prosta 
			}
		}
		if (checkIfWhite(X, Y - width -1) == true)	//czy jest rozgalezienie
		{

			if (checkifBlack(X + 1, Y - width) == true && checkifBlack(X - width, Y) == true)	//czy to ta sama sciezka
			{
				if (checkifBlack(X, Y +1) == false)
				{
					if (ifBottom == 0)
					{
						CorridorTop(X - width+1, Y - width, colour);
					}
					else
					{
						//CorridorTop(X - width+1, Y - width, changeColours(colour));
						thread childCorridor(CorridorTop, X - width + 1, Y - width, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorTop(X - width+1, Y - width, colour);
				}
			}
			else if (checkifBlack(X + 1, Y - width) == true)	//tworzenie dziecka o innym kolororze
			{
				thread childCorridor(CorridorTop, X - width + 1, Y - width, changeColours(colour));
				childCorridor.join();
			}
		}
		for (int i = 0;i < width;i++)
		{
			setColours(colour, X, Y - i);
		}
		X--;
	}
}
void CorridorTop(int X, int Y, vector<unsigned char> colour)
{
	bool ifRight = rand() % 2;	//w ktora strone kontynuowac
	while (checkifBlack(X, Y) == false)
	{

		if (checkIfWhite(X + width + 1, Y) == true)	//czy jest rozgalezienie
		{
			if (checkifBlack(X + width + 1, Y + 1) == true && checkifBlack(X, Y - width -1) == true)
			{
				if (checkifBlack(X - 1, Y) == false)	//czy jest rozgalezienie
				{
					if (ifRight == 1)
					{
						CorridorRight(X + width, Y, colour);

					}
					else
					{
						//CorridorRight(X + width, Y, changeColours(colour));
						thread childCorridor(CorridorRight, X + width, Y , changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorRight(X + width, Y, colour);

				}
			}
			else if (checkifBlack(X + width + 1, Y + 1) == true)
			{
				thread childCorridor(CorridorRight, X + width, Y, changeColours(colour));
				childCorridor.join();
			}
		}
		if (checkIfWhite(X - 1, Y) == true)	//czy jest rozgalezienie
		{
			if (checkifBlack(X - 1, Y + 1) == true && checkifBlack(X, Y - 1 - width) == true)
			{

				if (checkifBlack(X + width + 1, Y) == false)	//czy jest rozgalezienie w prawo tez
				{
					if (ifRight == 0)
					{
						CorridorLeft(X - 1, Y, colour);
					}
					else
					{
						//CorridorLeft(X - 1, Y, changeColours(colour));
						thread childCorridor(CorridorLeft, X -1, Y, changeColours(colour));
						childCorridor.join();
					}
				}
				else
				{
					CorridorLeft(X - 1, Y, colour);

				}
			}
			else if (checkifBlack(X - 1, Y + 1) == true)
			{
				thread childCorridor(CorridorLeft, X - 1, Y, changeColours(colour));
				childCorridor.join();
			}
		}
		for (int i = 0;i < width;i++)
		{
			setColours(colour, X+i,Y);
		}
		Y--;
	}
}
int main()
{
	char buff[100];
	char c;
	/*create new file,give it a name and open it in binary mode  */
	fp = fopen(filename, "rb"); /* b -  binary mode */
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		exit(1);
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}

	//check for comments
	c = getc(fp);
	while (c == '#') {
		while (getc(fp) != '\n');
		c = getc(fp);
	}
	ungetc(c, fp);
	int x, y;
	if (fscanf(fp, "%d %d", &x, &y) != 2) {
		fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
		exit(1);
	}
	if (fscanf(fp, "%d", &MaxColorComponentValue) != 1) {
		fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
		exit(1);
	}
	while (fgetc(fp) != '\n');
	for (int i = 0;i < iYmax;i++)
	{
		for (int j = 0;j < iXmax;j++)
		{
			fread(ImageMatrix[j][i], 1, 3, fp);
		}
	}
	int startX = seekX();
	vector<unsigned char> startColour = { 100,50,0 };
	CorridorBottom(startX, 0, startColour);
	
	fclose(fp);
	fp = fopen("test.ppm", "wb"); /* b -  binary mode */
	/*write ASCII header to the file*/
	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmax, iYmax, MaxColorComponentValue);
	for (int i = 0;i < iYmax;i++)
	{
		for (int j = 0;j < iXmax;j++)
		{
			fwrite(ImageMatrix[j][i], 1, 3, fp);
		}
	}
	fclose(fp);

	return 0;
}

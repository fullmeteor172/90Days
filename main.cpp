#include<iostream>
#include<vector>
#include<fstream>
#include<chrono>
#include<ctime>
using namespace std;
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class scene : public olc::PixelGameEngine
{
public:
	int plotPoints[90][2]; //Contains all places where squares should be made
	int progData[4]; // 0-> streak days, 1-> mm, 2->dd, 3->yyyy
	int xPad = 20, yPad = 20; //Padding for the app. Both sides same val.
	scene()
	{
		sAppName = "90 Days";
	}
	
	bool ParsePlotData()
	{
		string word;
		int wordNumber = 0; //The words number position (0...90)
		fstream plotData;
		plotData.open("data.txt", ios::in);
		while (plotData>>word)
		{
			stringstream intVal(word); //converting the string to a stream
			intVal >> plotPoints[wordNumber / 2][wordNumber % 2]; //sending the val to the plotPoint array
			++wordNumber;
		}
		plotData.close();
		return true;
	}

	bool ParseProgressData()
	{
		//Format: "<streakdays>_<lastdate in mmddyyyy>"
		fstream progressData;
		progressData.open("prog.txt", ios::in);
		string word;
		int wordNo = 0;
		while (progressData >> word)
		{
			stringstream intVal(word);
			intVal >> progData[wordNo];
			++wordNo;
		}
		progressData.close();
		return true;
	}

	bool PrintData(int length)
	{
		for (int i = 0; i <= length; i++)
		{
			cout << "x = " << plotPoints[i][0] << "\ty = " << plotPoints[i][1] << "\n";
		}
		return true;
	}

	bool Draw90(int xPadding, int yPadding, olc::Pixel color)
	{
		int xPos, yPos;
		for (int i = 0; i < 90; i++)
		{
			xPos = plotPoints[i][0];
			yPos = plotPoints[i][1];
			DrawRect(xPos * 10 + xPadding, yPos * 10 + yPadding, 10, 10, color);
		}
		return true;
	}

	bool DrawStreak(int xPadding, int yPadding, olc::Pixel color)
	{
		for (int i = 0; i <= progData[0]; i++)
		{
			int xPos = plotPoints[i][0] * 10;
			int yPos = plotPoints[i][1] * 10;
			FillRect(xPos + xPadding, yPos + yPadding, 10, 10, color);
		}
		return true;
	}

	bool Draw()
	{
		FillRect(0, 0, 200, 200, olc::Pixel(250, 224, 209, 255));
		DrawStreak(xPad, yPad, olc::Pixel(230, 91, 106, 255));
		Draw90(xPad, yPad, olc::Pixel(226, 126, 126, 255));
		return true;
	}

	bool OnUserCreate() override
	{
		ParsePlotData();//Parses the data.txt file
		ParseProgressData(); //Parses prog.txt 242, 242, 240, 255
		Draw();
		return true;

	}

	bool UpdateProgress()
	{
		fstream progressData;
		++progData[0];
		progressData.open("prog.txt", ios::out);
		progressData << progData[0] << " ";

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		cout << fElapsedTime << endl;
		int xPos, yPos;
		if (GetMouse(0).bPressed == true)
		{
			UpdateProgress();
			Draw();
		}
		return true;
	}
};

int main()
{
	scene app;
	if (app.Construct(140 + app.xPad*2, 90+app.yPad*2, 2, 2))
		app.Start();
	return 0;
}
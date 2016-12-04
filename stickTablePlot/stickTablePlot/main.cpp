#include "opencv2/opencv.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "stick.h"

using namespace std;
using namespace cv;
using namespace stick;

int Plot(Point &gridDim = Point(4, 4), int **data = NULL){
	static int **oldData = NULL;

	//Point gridDim;
	//gridDim.x = _x;
	//gridDim.y = _y;

	Point gridProp;
	gridProp.x = 40;
	gridProp.y = 40;

	Point gridTextProp;
	gridTextProp.x = 3;
	gridTextProp.y = 25;


	// Create Sheet
	Point sheetDim;

	sheetDim.x = gridDim.x * gridProp.x + 40;
	sheetDim.y = gridDim.y * gridProp.y + 80;

	Mat sheet(sheetDim.y + 40, sheetDim.x + 40, CV_8UC3, Scalar::all(0));

	// Border
	rectangle(sheet, Rect(20, 20, sheetDim.x, sheetDim.y), Scalar(255, 0, 0), 3);

	// Make grid
	Mat sheetGrid = sheet(Rect(20 + 20, 20 + 20, sheetDim.x, sheetDim.y));

	for (int i = 0; i < gridDim.x + 1; i++){
		line(sheetGrid, Point(i*gridProp.x, 0), Point(i*gridProp.x, gridDim.y*gridProp.y), Scalar(0, 255, 0));
	}
	for (int i = 0; i < gridDim.y + 1; i++){
		line(sheetGrid, Point(0, i*gridProp.y), Point(gridDim.x*gridProp.x, i*gridProp.y), Scalar(0, 255, 0));
	}

	// Dummy Table to plot
	//int **data = new int*[gridDim.y];
	if (data == NULL){
		data = new int*[gridDim.y];
		for (int i = 0; i < gridDim.y; i++){
			data[i] = new int[gridDim.x];

			for (int j = 0; j < gridDim.x; j++)
				data[i][j] = rand() % 10;
		}
	}

	// Plot content in cells
	for (int i = 0; i < gridDim.y; i++){
		for (int j = 0; j < gridDim.x; j++){
			//cout << i << "," << j << " : " << data[i][j] << endl;
			stringstream t;
			t << data[i][j];

			Scalar color = Scalar(0, 255, 0);

			if (oldData != NULL){
				if (data[i][j] != oldData[i][j]){
					color = Scalar(0, 0, 255);
					rectangle(sheetGrid, Rect(j*gridProp.x, i*gridProp.y, gridProp.x, gridProp.y), Scalar::all(255), CV_FILLED);
				}
			}

			putText(sheetGrid, String(t.str()), Point(j*gridProp.x + gridTextProp.x, i*gridProp.y + gridTextProp.y), CV_FONT_HERSHEY_PLAIN, 1, color, 1, 4);
		}
	}

	// Plot Help
	putText(sheetGrid, String("Press R to refresh"), Point(0, gridDim.y*gridProp.y + gridTextProp.y), CV_FONT_HERSHEY_PLAIN, 1, Scalar(50, 100, 170), 1, 4);

	// Show sheet
	namedWindow("STICK", CV_WINDOW_KEEPRATIO);
	imshow("STICK", sheet);
	int result = waitKey(0);

	// Cleanup oldData
	if (oldData != NULL){
		for (int i = 0; i < gridDim.y; i++){
			delete[] oldData[i];
		}
		delete[] oldData;
	}

	// Copy current data to oldData
	oldData = createTable(gridDim);
	for (int i = 0; i < gridDim.y; i++){
		for (int j = 0; j < gridDim.x; j++)
			oldData[i][j] = data[i][j];
	}

	// Cleanup Memory
	for (int i = 0; i < gridDim.y; i++){
		delete[] data[i];
	}
	delete[] data;

	return result;
}

int WinMain()
{
	int result = 1;

	// Dummy Table to plot
	//Point dim(4, 4);
	//int **data = createTable(dim, 5);

	//saveStickFile(dim, data);
	//cleanupTable(dim, data);

	pair<Point, int**> src = getStickFile("auto.stick");
	while (result != 27 && result != -1){
		result = Plot(src.first, src.second);

		if (result == 'R' || result == 'r'){
			src = getStickFile("auto.stick");
		}
	}

	return 0;
}

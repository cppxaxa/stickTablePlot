#ifndef STICK_F
#define STICK_F

#include "opencv2/opencv.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

namespace stick{
	
	using namespace cv;
	using namespace std;

	pair<Point, int**> getStickFile(string fname){
		ifstream fin(fname.data());

		Point dim;
		dim.x = 3;
		dim.y = 3;

		fin >> dim.x;
		fin.get();
		fin >> dim.y;

		int **data = new int*[dim.y];
		for (int i = 0; i < dim.y; i++){
			data[i] = new int[dim.x];

			for (int j = 0; j < dim.x; j++){
				int val;
				fin >> val;
				data[i][j] = val;

				// I think it includes comma and \n
				fin.get();
			}
		}

		fin.close();

		return make_pair(dim, data);
	}

	void saveStickFile(Point dim = Point(4, 4), int **data = NULL, string fname = "auto.stick"){
		// Dummy Table to plot
		if (data == NULL){
			data = new int*[dim.y];
			for (int i = 0; i < dim.y; i++){
				data[i] = new int[dim.x];

				for (int j = 0; j < dim.x; j++)
					data[i][j] = rand() % 10;
			}
		}

		ofstream f(fname.data());
		f << dim.x << ", " << dim.y << "\n";

		for (int i = 0; i < dim.y; i++){
			for (int j = 0; j < dim.x; j++){
				f << data[i][j];

				// Check end of line
				if (j != dim.x - 1)
					f << ", ";
				else
					f << "\n";
			}
		}

		f.close();
	}

	int **createTable(Point dim = Point(4, 4), int default = 0){
		int **data = new int*[dim.y];
		for (int i = 0; i < dim.y; i++){
			data[i] = new int[dim.x];

			for (int j = 0; j < dim.x; j++)
				data[i][j] = default;
		}
		return data;
	}

	void cleanupTable(Point dim, int **data){
		// Cleanup Memory
		for (int i = 0; i < dim.y; i++){
			delete[] data[i];
		}
		delete[] data;
	}

	/*********************************
			HELP
			----
		Point dim(4, 4);
		int **data = createTable(dim, 5);

		saveStickFile(dim, data);

		pair<Point, int**> src = getStickFile("auto.stick");

		cleanupTable(dim, data);
	**********************************/
}

#endif
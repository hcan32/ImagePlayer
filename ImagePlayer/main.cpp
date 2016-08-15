#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include <iostream>
#include <Windows.h>
#include "compNat.hpp"
#include <fstream>


using namespace cv;
using namespace std;

bool play = true, stop = false, beginning = false;


// gets all files withing the folder
// code is from stackoverflow by herohuyongtao
vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	char search_path[200];
	sprintf(search_path, "%s/*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}


// split source string by limiter splitChar
// and return splitted parts as string vector
vector<string> splitString(string source, char splitChar) {
	vector<string> splitted;
	stringstream ss(source);
	string tok;
	while (getline(ss, tok, splitChar)) {
		splitted.push_back(tok);
	}
	return splitted;
}

int sliderMax = 100;
int currentSlider;



// empty function to initialize trackbar
void on_trackbar(int, void*){ }


/*
	All images must be in same folder.
	Change directoryName variable as the directory you want to monitor
*/
int main(int argc, char *argv[])
{
	currentSlider = 0;

	// named window for showing trackbar and images in the same window
	namedWindow("FRAMES", 1);


	// directoryName and searchType can change
	char *directoryName = "";
	char *searchType = ".png";

	// if you convert image sequence to video open up 
	//VideoWriter video("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(720, 630), true);

	vector<string> fileNames = get_all_files_names_within_folder(directoryName);


	// eliminate files out of search type
	for (int i = 0; i < fileNames.size(); i++){
		if (fileNames[i].find(searchType) == string::npos){
			cout << fileNames[i] << endl;
			fileNames.pop_back();
			i--;
		}
	}



	sort(fileNames.begin(), fileNames.end(), compareNat);


	sliderMax = fileNames.size();
	createTrackbar("Trackbar", "FRAMES", &currentSlider, sliderMax, on_trackbar);
	on_trackbar(currentSlider, 0);


	int lineCounter = 0;
	char key = 0;

	Mat currentFrame;

	// unless user enters q on keyboard or images are over , continue
	while (key != 'q' && lineCounter < fileNames.size() - 2){
		lineCounter = currentSlider;
		lineCounter++;

		// get full path of current image
		string currentFilePath = string(directoryName) + fileNames[lineCounter];
		// load this image to current frame
		currentFrame = imread(currentFilePath);

		imshow("FRAMES", currentFrame);
		// if you convert image sequence to video , open this too 
		//video.write(leftCurrentFrame);
		currentSlider = lineCounter;

		// wait for 500 ms for monitoring , could be less or more
		key = waitKey(500);
		on_trackbar(currentSlider, 0);
	}

	return 0;
}




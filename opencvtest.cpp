#include "opencv2/opencv.hpp"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv){

    char *search_str = argv[1];
    
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("tongtest2.mp4"); 
   
    // Check if camera opened successfully
    if(!cap.isOpened()){
	cout << "Error opening video stream or file" << endl;
	return -1;
    }

    double FPS = cap.get(CAP_PROP_FPS);
  
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
	fprintf(stderr, "Could not initialize tesseract.\n");
	exit(1);
    }

    int num_frames = 0;	
    while(1){
      
	Mat frame;
	// Capture frame-by-frame
	cap >> frame;
      
	// If the frame is empty, break immediately
	if (frame.empty())
	    break;
      
	cv::imwrite("test.png", frame);

	// Display the resulting frame
	imshow( "Frame", frame );
      
	// Press  ESC on keyboard to exit
	char c=(char)waitKey(25);
	if(c==27)
	    break;
      
	num_frames++;
    
	// Print OCR result approximately every 1 second
	if ( num_frames % ((int) FPS) == 0) {    
	    int total_seconds = 1.0 / (FPS / num_frames);
	  
	    /* CONSTANTS FOR TIME UNITS */
	    const int SECOND = 1;
	    const int MINUTE = 60*SECOND;
	    const int HOUR = 60*MINUTE;
    
	    auto get = [&total_seconds](int unit) {
		int out = total_seconds/unit;
		total_seconds = total_seconds % unit;
		return out;
	    };
	    int hours = get(HOUR);
	    int minutes = get(MINUTE);
	    int seconds = total_seconds;
    
	    /* FORMAT AS TIMESTAMP */
	    char timestamp_[200];
	    if (hours > 0) {
		sprintf(timestamp_, "%.2i:%.2i:%.2i", hours, minutes, seconds);
	    } else {
		sprintf(timestamp_, "%.2i:%.2i", minutes, seconds);
	    }
    
	    // Open input image with leptonica library
	    Pix *image = pixRead("test.png");
	    api->SetImage(image);

	    // Get OCR result
	    char *outText = api->GetUTF8Text();

	    std::string outStr = string(outText);
	    std::string search = string(search_str);
	    std::size_t found = outStr.find(search);
	    
	    if (found != std::string::npos) {
		std::cout << "match: " <<'\n';
		std::cout << "[" << timestamp_ << "] ||" << outStr.substr(found, search.length()) << "||" << outStr.substr(found + search.length(), 100);
	    }
	    
	    // Destroy used object and release memory
	    pixDestroy(&image);
	    delete [] outText;
	}
    }
  
    // Destroy used object and release memory
    api->End();
    delete api;

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();
	
    return 0;
}

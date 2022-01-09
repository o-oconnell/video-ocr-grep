# video-ocr-grep
search the ocr results of video frames

to compile:
install tesseract and opencv >= 4.5.4
clone and go into repo dir
run "cmake ."
run make
change VideoCapture cap("...") to your video file
run ./tst with a search string as the first arg

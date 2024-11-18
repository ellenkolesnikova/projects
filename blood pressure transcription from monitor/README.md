# transcribedigits

*Digit recognition steps
1) Detect screen using YOLOv9 model
2) Run regression model on YOLO-cropped screen to get screen color
3) Run manual color detection program on uncropped image
4) If colors are different, choose color with higher confidence level, as outputted by the models
5) Create a mask of the image based on detected screen color
6) See if YOLO model detection is similar to mask detection
7) If the two detections are not similar, ask user to retake image b/c algorithms aren’t agreeing with each other
Otherwise proceed to crop YOLO-cropped images to obtain sbp/dbp readings & transcribe those with existing digit transcription neural networks

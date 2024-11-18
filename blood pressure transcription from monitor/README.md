# Blood pressure transcription from monitor images

**Purpose**

`transcribe_digits.py` reads in images of iHealth blood pressure monitors from the test_imgs folder and transcribes them using the process outlined below. It saves the transcription and color of the screen for each image in the file `out.txt`. `cropped_dbp` and `cropped_sbp` are folders that contain processed cropped images of the diastolic and systolic readings from each of the larger monitor images. `yolov9`, `helper_files`, and `my_models` are folders that contain models used in the blood pressure monitor transcription (all models are trained by me, except for the digit transcription nerual network).

**Running the code**

To run this code, simply run the following command:

    python3 -W ignore transcribe_digits.py

**Digit recognition steps in `transcribe_digits.py`**

These steps are repeated for every image in test_imgs.
1) Detect screen using YOLOv9 model
2) Rotate image so that the monitor is seen straight-on
3) Run regression model on YOLO-cropped screen to get screen color
4) Run manual color detection program on uncropped image
5) If colors are different, choose color with higher confidence level, as outputted by the models
6) Create a mask of the image based on detected screen color
7) See if YOLO model detection is similar to mask detection
8) If the two detections are not similar, ask user to retake image because the various algorithms aren’t agreeing with each other

If the image does not need to be retaken...
1) Proceed to adjust the perspective of the image to make up for possible warping of the image due to camera angle
2) Crop YOLO-cropped images to obtain systolic and diastolic blood pressure readings
3) Transcribe the SBP and DBP cropped images with digit transcription neural networks

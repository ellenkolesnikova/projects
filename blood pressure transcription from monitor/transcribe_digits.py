import torch
import os
import cv2
import pickle
from tensorflow import keras
from helper_files.get_extended_image_features import get_extended_features
from my_models.find_color_manual import *
from keras.models import load_model
from helper_files.helper_functions import get_lcd_yolo
from ultralytics import YOLO

# TODO
# PUSH THIS CODE TO IHEALTHBPTRANSCRIPTION
# PUSH DATASET TO IHEALTHBPTRANSCRIPTION
# RETRAIN DIGIT TRANSCRIPTION MODEL
# TEST COLOR DETECTION

# Establish color ranges for later - in BGR
color_ranges = {}

LOWER_GREEN = np.array([0, 80, 0], dtype="uint8")
UPPER_GREEN = np.array([250, 255, 205], dtype="uint8")

LOWER_YELLOW = np.array([5, 60, 110], dtype="uint8")
UPPER_YELLOW = np.array([255, 255, 255], dtype="uint8")

LOWER_RED = np.array([5, 0, 130], dtype="uint8")
UPPER_RED = np.array([255, 170, 255], dtype="uint8")

LOWER_GRAY = np.array([30, 40, 40], dtype="uint8")
UPPER_GRAY = np.array([210, 210, 210], dtype="uint8")

color_ranges['green'] = [LOWER_GREEN, UPPER_GREEN]
color_ranges['yellow'] = [LOWER_YELLOW, UPPER_YELLOW]
color_ranges['red'] = [LOWER_RED, UPPER_RED]
color_ranges['gray'] = [LOWER_GRAY, UPPER_GRAY]

# Load regression model
regression_model = pickle.load(open("my_models/find_color_regression_model.sav", 'rb'))
# Load digit recognition model
digit_model = load_model("my_models/best_model.h5")
# Load yolo model - yolov5
#yolo_model = torch.hub.load('/Users/ellenkolesnikova/Desktop/coding/bp_transcription/new_yolo_model/yolov5', 'custom', path='/Users/ellenkolesnikova/Desktop/coding/bp_transcription/new_yolo_model/yolov5/runs/train/5_21_24/weights/best.pt', source='local') 
# Load yolo model - yolov9-t
yolo_model = torch.hub.load('yolov9', 'custom', path='my_models/yolov9best.pt', source='local') 

# Crop the image based on the most confident YOLO bounding 
# Returns cropped image, coordinates, and whether or not the screen was detected
def crop_image_yolo(yolo_model, image):
    yolo_img = image[..., ::-1]
    # Detect screen
    results = yolo_model(yolo_img)
    # Only consider most confident detection
    MAX_CONF = 0
    MAX_CONF_IND = 0
    yolo_screens = results.pandas().xyxy[0].to_dict()

    for detection in range(0, len(yolo_screens['name'].keys())):
        # Convert from dataframe to python dictionary with xmin, ymin, xmax, ymax, and confidence
        if int(yolo_screens['confidence'][detection]) > MAX_CONF:
            MAX_CONF_IND = detection
            MAX_CONF = int(yolo_screens['confidence'][detection])

    # Coordinates of YOLO-detected bounding rectangle 
    Y1 = int(0)
    Y2 = int(image.shape[0])
    X1 = int(0)
    X2 = int(image.shape[1])

    detected = False
    # If there are no detections set yolo_cropped_img to be original image
    if (len(yolo_screens['name'].keys())==0):
        yolo_cropped_img = image
    # Otherwise set yolo_cropped_img to be cropped image based on yolo detection
    else:
        Y1 = int(yolo_screens['ymin'][MAX_CONF_IND])
        Y2 = int(yolo_screens['ymax'][MAX_CONF_IND])
        X1 = int(yolo_screens['xmin'][MAX_CONF_IND])
        X2 = int(yolo_screens['xmax'][MAX_CONF_IND])
        yolo_cropped_img = image[Y1:Y2, X1:X2]
        detected = True
    return yolo_cropped_img, Y1, Y2, X1, X2, detected

# Rotate counterclockwise by specified angle
def rotate_yolo(image, angle):
    (h, w) = image.shape[:2]

    # Define the center of the image
    center = (w / 2, h / 2)

    # Define the rotation matrix for rotating the image
    scale = 1.0
    M = cv2.getRotationMatrix2D(center, angle, scale)

    # Perform the actual rotation and return the image
    rotated = cv2.warpAffine(image, M, (w, h))

    return rotated

# Helper function for rotate_by_best_degree
# Rotates the image until the width is the smallest - repeats this iter_num times, 
# each time rotating by incr
def helper_find_best_degree(img, incr, iter_num):
    yolo_cropped_img, _, _, _, _, _ = crop_image_yolo(yolo_model, img)
    smallest_size = yolo_cropped_img.shape[1]
    best_degree = 0

    curr_degree = incr
    for i in range(iter_num):
        img = rotate_yolo(img, incr)
        yolo_cropped_img, _, _, _, _, _ = crop_image_yolo(yolo_model, img)
        width = yolo_cropped_img.shape[1]
        if width < smallest_size:
            smallest_size = width
            best_degree = curr_degree
        curr_degree += incr
    
    return best_degree

# Returns a straightened image
def rotate_by_best_degree(img):
    # Split up the work into different degree increments for the code to run faster - image won't be straightened 
    # properly if it's more than 20° tilted but this can be adjusted based on real-life testing
    img = rotate_yolo(img, -20)

    best_degree = helper_find_best_degree(img, incr=20, iter_num=2)
    img = rotate_yolo(img, best_degree-10)

    best_degree = helper_find_best_degree(img, incr=10, iter_num=2)
    img = rotate_yolo(img, best_degree-5)

    best_degree = helper_find_best_degree(img, incr=5, iter_num=2)
    img = rotate_yolo(img, best_degree-2)

    best_degree = helper_find_best_degree(img, incr=2, iter_num=2)
    img = rotate_yolo(img, best_degree-2)

    best_degree = helper_find_best_degree(img, incr=1, iter_num=4)
    
    img = rotate_yolo(img, best_degree)

    return img

# Helper function to transcribe_digits
def transcribe_digits_indv(yolo_cropped_img, digit_model, filename):
    width = yolo_cropped_img.shape[1]
    height = yolo_cropped_img.shape[0]

    height_interval = int(height/7)
    sbp = yolo_cropped_img[max(0,height_interval-5):min(height,3*height_interval+10), int(width/4):width]
    dbp = yolo_cropped_img[max(0,3*height_interval):min(height,5*height_interval+20), int(width/4):width]

    # Reshape the sbp image to match the required size for the neural network
    sbp = cv2.resize(sbp,(180,80))
    sbp = np.array(sbp)
    resized_sbp = np.expand_dims(sbp, axis=-1)
    sbp = resized_sbp.reshape((-1, 80, 180, 1))

    # Reshape the dbp image to match the required size for the neural network
    dbp = cv2.resize(dbp,(180,80))
    dbp = np.array(dbp)
    resized_dbp = np.expand_dims(dbp, axis=-1)
    dbp = resized_dbp.reshape((-1, 80, 180, 1))

    # Predict the sbp and dbp
    sbp_prediction = digit_model.predict(sbp)
    dbp_prediction = digit_model.predict(dbp)

    # Convert the sbp_prediction to a number
    sbp_pred_list_i = [np.argmax(pred) for pred in sbp_prediction]
    sbp_conf = (max(sbp_prediction[0][0])+max(sbp_prediction[1][0])+max(sbp_prediction[2][0]))/3
    sbp_predicted_num = 100* sbp_pred_list_i[0] + 10 * sbp_pred_list_i[1] + 1* sbp_pred_list_i[2]
    if sbp_predicted_num >= 1000:
        sbp_predicted_num = sbp_predicted_num-1000

    # Convert the dbp_prediction to a number
    dbp_pred_list_i = [np.argmax(pred) for pred in dbp_prediction]
    dbp_conf = (max(dbp_prediction[0][0])+max(dbp_prediction[1][0])+max(dbp_prediction[2][0]))/3
    dbp_predicted_num = 100* dbp_pred_list_i[0] + 10 * dbp_pred_list_i[1] + 1* dbp_pred_list_i[2]
    if dbp_predicted_num >= 1000:
        dbp_predicted_num = dbp_predicted_num-1000

    conf = (sbp_conf+dbp_conf)/2

    return sbp_predicted_num, dbp_predicted_num, conf

# Transcribes the digits of a yolo-cropped image
def transcribe_digits(yolo_cropped_img, digit_model, filename):
    yolo_cropped_img = get_lcd_yolo(yolo_cropped_img)
   
    yolo_adjusted_top = adjust_perspective_top(yolo_cropped_img)
    yolo_adjusted_bottom = adjust_perspective_bottom(yolo_cropped_img)

    sbpt, dbpt, conft = transcribe_digits_indv(yolo_adjusted_top, digit_model, filename)
    sbpb, dbpb, confb = transcribe_digits_indv(yolo_adjusted_bottom, digit_model, filename)

    max_conf = max(conft, confb)

    if max_conf == conft:
        new_img = yolo_adjusted_top
    elif max_conf == confb:
        new_img = yolo_adjusted_bottom

    width = new_img.shape[1]
    height = new_img.shape[0]
    height_interval = int(height/7)
    sbp_img = new_img[max(0,height_interval-5):min(height,3*height_interval+10), int(width/4):width]
    dbp_img = new_img[max(0,3*height_interval):min(height,5*height_interval+20), int(width/4):width]
    cv2.imwrite("cropped_sbp/sbp"+filename, sbp_img)
    cv2.imwrite("cropped_dbp/dbp"+filename, dbp_img)
    return sbpt, dbpt

# Adjusts the perspective of an image that's taken from closer to the bottom
def adjust_perspective_top(yolo_cropped_img):
    width = yolo_cropped_img.shape[1]
    height = yolo_cropped_img.shape[0]
    expected_height = int(width * (2.5/2))
    percent_height_diff = 1 - height/expected_height

    width_diff = int(0.8*((percent_height_diff*width)/2))

    org_points = np.float32(
        [[0, height],
        [width, height],
        [width_diff, 0],
        [width-width_diff, 0]])
    new_points = np.float32(
        [[0, expected_height],
        [width, expected_height],
        [0, 0],
        [width, 0]])

    # calculate the perspective transform matrix and warp
    # the perspective to grab the screen
    M = cv2.getPerspectiveTransform(org_points, new_points)
    yolo_cropped_img = cv2.warpPerspective(yolo_cropped_img, M, (width,expected_height))

    return yolo_cropped_img

def adjust_perspective_bottom(yolo_cropped_img):
    width = yolo_cropped_img.shape[1]
    height = yolo_cropped_img.shape[0]
    expected_height = int(width * (2.5/2))
    percent_height_diff = 1 - height/expected_height

    width_diff = int(0.8*((percent_height_diff*width)/2))

    org_points = np.float32(
        [[width_diff, height],
        [width-width_diff, height],
        [0, 0],
        [width, 0]])
    new_points = np.float32(
        [[0, expected_height],
        [width, expected_height],
        [0, 0],
        [width, 0]])

    # calculate the perspective transform matrix and warp
    # the perspective to grab the screen
    M = cv2.getPerspectiveTransform(org_points, new_points)
    yolo_cropped_img = cv2.warpPerspective(yolo_cropped_img, M, (width,expected_height))

    return yolo_cropped_img


with open('out.txt', 'w') as f:
    for root, dirs, files in os.walk("test_imgs"):
        for filename in files:
            path = os.path.join(root, filename)
            if filename.endswith(".jpg"):
                print(filename, file=f)

                img = cv2.imread(path)

                rotated_img = rotate_by_best_degree(img)

                yolo_cropped_img, Y1, Y2, X1, X2, detected = crop_image_yolo(yolo_model, rotated_img)

                # 2. Run regression model on YOLO-cropped screen to get screen color

                # Get dataframe of image features
                img_features = get_extended_features(yolo_cropped_img)
                # Predict screen color based on image features using regression
                prediction = regression_model.predict(img_features)
                unrounded = prediction[0]
                color_num = round(unrounded)
                regression_color = ""
                regression_confidence = int()
                # Set color and confidence
                if color_num <= -1:
                    regression_color = "gray"
                    regression_confidence = 1-(abs(unrounded-(-1)))
                elif color_num == 0:
                    regression_color = "green"
                    regression_confidence = 1-(abs(unrounded-(0)))
                elif color_num == 1:
                    regression_color = "yellow"
                    regression_confidence = 1-(abs(unrounded-(1)))
                else:
                    regression_color = "red"
                    regression_confidence = 1-(abs(unrounded-(2)))


                # 3. Run manual color detection model on uncropped image
                manual_color, manual_confidence = find_color(img)
                manual_confidence = manual_confidence*1.5

                # 4. If different, choose color with higher confidence level
                color = ""

                if manual_confidence < regression_confidence:
                    color = regression_color
                else:
                    color = manual_color

                print(color, file=f)

                # 5. Create a mask of the image based on detected screen color
                mask = cv2.inRange(img, color_ranges[color][0], color_ranges[color][1])

                # Only consider areas in the mask above a certain size
                area = img.shape[0]*img.shape[1]
                min_contour_area = int(area/30) # edit this number to adjust the area threshold
                # Find contours in the mask
                contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                # Filter contours based on area
                filtered_mask = np.zeros_like(mask)
                for contour in contours:
                    if cv2.contourArea(contour) > min_contour_area:
                        cv2.drawContours(filtered_mask, [contour], 0, 255, thickness=cv2.FILLED)
                # Now filtered_mask contains contiguous regions above a certain area threshold
                color_detection_mask = filtered_mask


                # Count the number of white pixels in the color detection mask
                color_detection_num_pixels = cv2.countNonZero(color_detection_mask)

                # 6. Create a mask such that the highlighted section is the YOLO-identified bounding rectangle of the screen

                # Create a black mask
                yolo_mask = np.zeros_like(img)

                # Draw a white rectangle on the mask based on the yolo detection
                cv2.rectangle(yolo_mask, (X1, Y1), (X2, Y2), (255,255,255), -1)

                # Make the mask one channel
                yolo_mask = cv2.cvtColor(yolo_mask, cv2.COLOR_BGR2GRAY)


                # 7. See if YOLO model detection is similar to mask detection

                # Create a combined mask, highlighting pixels if either corresponding pixel in yolo_mask or color_detection_mask is highlighted
                combined_mask = cv2.bitwise_or(color_detection_mask, yolo_mask)

                # Count the number of white pixels in the combined mask
                combined_mask_num_pixels = cv2.countNonZero(combined_mask)


                # Calculate how similar yolo_mask and color_detection_mask are
                diff = (combined_mask_num_pixels-color_detection_num_pixels)/(combined_mask_num_pixels)

                # If the difference is bigger than 0.25, the yolo_mask and color_detection_mask are too different
                if diff>0.3 or detected==False:
                    print('bad image, try again', file=f)
                # Otherwise, we can proceed to transcribing the blood pressure
                else:
                    sbp, dbp = transcribe_digits(yolo_cropped_img, digit_model, filename)

                    # write code to ask for a new image if the sbp and dbp are not what they should be 
                    # based on the color
                    print("sbp: ", sbp, file=f)
                    print("dbp: ", dbp, file=f)
                print(file=f)
            



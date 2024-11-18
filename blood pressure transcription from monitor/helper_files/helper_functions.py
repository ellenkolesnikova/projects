#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Script for helper functions required to transcribe the images
1. adjust_gamma(): Performs gamma correction on input image
2. imgs_to_array(): Converts each image in folder into a numpy array & concatenates them
3. get_lcd(): Takes input image filename & extracts binary thresholded single monitor BP LCD frames from it  
@author: skulk26
"""
import numpy as np
import cv2
from PIL import Image
import pandas as pd
def adjust_gamma(image, gamma=1.0):
        """
        Credit: https://stackoverflow.com/questions/33322488/how-to-change-image-illumination-in-opencv-python
        Parameters:
            image: A grayscale image (NxM int array in [0, 255]
            gamma: A positive float. If gamma<1 the image is darken / if gamma>1 the image is enlighten / if gamma=1 nothing happens.
        Returns: the enlighten/darken version of image
        """
        invGamma = 1.0 / gamma
        table = np.array([((i / 255.0) ** invGamma) * 255 for i in np.arange(0, 256)])
        return cv2.LUT(image.astype(np.uint8), table.astype(np.uint8))

def imgs_to_array(filenames, src_folder):
    """
    Function resizes input single monitor images into chosen image size((180 X 80) in paper, and converts into array 
    by reshaping into a (180 x 80 x 1) size array for every image.
    """
    X = []
    for fname in filenames:
        print(fname)
        ID =  src_folder + "%s" % (fname)
        img = Image.open(ID)            
        img=img.resize((180,80))
        img = np.array(img)
        img = img.reshape((img.shape[0],img.shape[1],1))
        # print(img.shape[0],img.shape[1])
        X.append(img)
    X = np.asarray(X)
    return X

def get_lcd(image):#fname
    #image=cv2.imread(fname)
    #Preprocess image under test
    if image is not None:
        #Convert image to grayscale
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        #Apply bilateral filter for smoothing test image(noise reduction), while preserving edges    
        blurred = cv2.bilateralFilter(gray, 11, 11, 11)
        #Apply gamma correction to adjust image illumination
        gamma = adjust_gamma(blurred, gamma=0.7)
        #Apply adaptive thresholding with a mean neighborhood threshold calculation as image may have varying illuniation
        adt_thresh=cv2.adaptiveThreshold(gamma,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,11,2)
        #Erode thresholded image using a 3x3 rectangular kernel
        kernel=cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
        eroded=cv2.erode(adt_thresh, kernel)
        #Invert image to find contours in image
        inverse=cv2.bitwise_not(eroded)
        contours, _ = cv2.findContours(inverse.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)   
        bp_cnt = []
        d=image.copy()  
        for contour in contours:
            x,y,w,h=cv2.boundingRect(contour)
            cv2.rectangle(d,(x,y),(x+w,y+h),(255,0,0),2)
            aspect_ratio=w/h
            size=w*h
            if 0.95<= aspect_ratio<=1.5 and 20000<= size <80000:# and int(aspect_ratio)!=int(prevb):
                bp_cnt.append(contour)
                coord=int(str(x)[:2])
                prevb=aspect_ratio
        if bp_cnt!=[]:
            #print(fname)
            cnt2=max(bp_cnt, key=cv2.contourArea)
            x,y,w,h=cv2.boundingRect(cnt2)
            print("BP",x,y,w,h,w*h,w/h, cv2.contourArea(cnt2))
            upper_left = (int(w / 8), int(h / 16))
            bottom_right = (int(w * 7 / 8), int(h * 15 / 16))
            frame=inverse[y:y+h, x:x+w]
            mask=cv2.rectangle(frame.copy(), upper_left, bottom_right, (0, 0, 0),-1)
            final_img=frame-mask    
            return final_img,w,h

def test_predict(test_results, model_1, X_test, ts_data):
    y_test =  ts_data[["d1", "d2", "d3"]]
    print(X_test.shape[:2])
    y_pred = model_1.predict(X_test)
    correct_preds = 0
    
    for i in range(X_test.shape[0]):
        pred_list_i = [np.argmax(pred[i]) for pred in y_pred]
        true_list_i  = y_test.values[i].astype('int')
        if np.array_equal(true_list_i, pred_list_i):
            correct_preds = correct_preds + 1
    print('exact accuracy', correct_preds / X_test.shape[0])
        
    df=pd.DataFrame(columns=['filename','true', 'predicted', 'difference','quality'], index=range(X_test.shape[0]))    
    diff = []
    for i in range(X_test.shape[0]):
        pred_list_i = [np.argmax(pred[i]) for pred in y_pred]
        pred_number = 100* pred_list_i[0] + 10 * pred_list_i[1] + 1* pred_list_i[2]
        if pred_number<1000:
            df.predicted[i]=pred_number
        else:
            df.predicted[i]=pred_number-1000    
        true_list_i  = y_test.values[i].astype('int')
        true_number = 100*  true_list_i[0] + 10 *  true_list_i[1] + 1*  true_list_i[2]
        df.filename[i]=ts_data.filename[i]

        df.quality[i]=ts_data.quality[i]
        if true_number<1000:
            df.true[i]=true_number
        else:    
            df.true[i]=true_number-1000
        diff.append(true_number - pred_number)
        df.difference[i]=df.true[i]-df.predicted[i]
        print(true_number, pred_number)        
    df.to_csv(test_results+'test_data_predictions.csv')
    print('difference label vs. prediction', df['difference'])
    if not df.loc[df.filename.str.contains('SP.jpg')].empty:
        df.loc[df.filename.str.contains('SP.jpg')].reset_index(drop=True).to_csv(test_results + "sysBP_predictions.csv")
        df.loc[df.filename.str.contains('DP.jpg')].reset_index(drop=True).to_csv(test_results + "diasBP_predictions.csv")

def get_lcd_yolo(image):
    #Preprocess image under test
    if image is not None:
        image=cv2.resize(image,(int(0.4*480),int(0.4*640)))
        #Convert image to grayscale
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        #Apply bilateral filter for smoothing test image(noise reduction), while preserving edges    
        blurred = cv2.bilateralFilter(gray, 15, 15, 15)
        #Apply gamma correction to adjust image illumination
        gamma = adjust_gamma(blurred, gamma=0.7)
        #Apply adaptive thresholding with a mean neighborhood threshold calculation as image may have varying illuniation
        adt_thresh=cv2.adaptiveThreshold(gamma,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,11,2)
        #Erode thresholded image using a 3x3 rectangular kernel
        kernel=cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
        eroded=cv2.erode(adt_thresh, kernel)
        #Invert image to find contours in image
        inverse=cv2.bitwise_not(eroded)
        h=inverse.shape[0]
        w=inverse.shape[1]
        '''upper_left = (int(w / 8), int(h / 16))
        bottom_right = (int(w * 15 / 16), int(h * 15 / 16))
        mask=cv2.rectangle(inverse.copy(), upper_left, bottom_right, (0, 0, 0),-1)'''
        final_img=inverse#-mask 
    
    return final_img
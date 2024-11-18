import cv2
from math import floor
import numpy as np
import pandas as pd

# number of bins = 30

def get_extended_features(img):
    RANGE_SIZE = int(floor((255/30)))
    csv_dict = dict()
    for i in range(30):
        csv_dict["red_features"+str(i)] = []
        csv_dict["green_features"+str(i)] = []

    img_size = img.shape[0]*img.shape[1]
    b, g, r = cv2.split(img)
    for i in range(30):
        curr = i*RANGE_SIZE
        pixel_range = [curr, curr+RANGE_SIZE]
        upper_red = np.sum(r <= curr+RANGE_SIZE)
        lower_red = np.sum(r < curr)

        upper_green = np.sum(g <= curr+RANGE_SIZE)
        lower_green = np.sum(g < curr)

        red = (upper_red-lower_red)/img_size
        green = (upper_green-lower_green)/img_size

        csv_dict["red_features"+str(i)].append(red)
        csv_dict["green_features"+str(i)].append(green)

    df = pd.DataFrame(csv_dict)
    return df


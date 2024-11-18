import cv2
import numpy as np
#import math
#import skimage.measure    


# my ranges
'''RG_lower = 0
RG_upper = 80
RY_lower = 140
RY_upper = 255
RR_lower = 180
RR_upper = 255

GG_lower = 80
GG_upper = 255
GY_lower = 80
GY_upper = 210
GR_lower = 0
GR_upper = 100'''

# computer generated ranges
RG_lower = 0
RG_upper = 135
RY_lower = 80
RY_upper = 255
RR_lower = 70
RR_upper = 245

GG_lower = 95
GG_upper = 250
GY_lower = 50
GY_upper = 190
GR_lower = 0
GR_upper = 110


red_ranges = [(RG_lower, RG_upper), (RY_lower, RY_upper), (RR_lower, RR_upper)]
green_ranges = [(GG_lower, GG_upper), (GY_lower, GY_upper), (GR_lower, GR_upper)]

lower_green = np.array([0, 80, 0], dtype="uint8")
upper_green = np.array([100, 255, 100], dtype="uint8")

lower_yellow = np.array([0, 100, 100], dtype="uint8")
upper_yellow = np.array([180, 255, 255], dtype="uint8")

lower_red = np.array([0, 0, 90], dtype="uint8")
upper_red = np.array([100, 100, 255], dtype="uint8")

'''
returns data of the image in the form of red_dict = G = % red pixels within RG, Y = % red pixels within RY, R = % red pixels within RR
green_dict = G = % green pixels within GG, Y = % green pixels within GY, R = % green pixels within GR
'''
def get_data(i, red_ranges, green_ranges):
    im_size = i.shape[0]*i.shape[1]
    data_list = list()
    red_dict = dict()
    green_dict = dict()                                                                                                                                                                                                             
    for channel in [0,1]:
        b, g, r = cv2.split(i)
        if channel == 0:
            channel_image = r
        else:
            channel_image = g

        for color_range in red_ranges if channel == 0 else green_ranges:
            lower, upper = color_range
            mask = cv2.inRange(channel_image, lower, upper)
            im_size = cv2.countNonZero(channel_image)
            
            data_list.append(cv2.countNonZero(mask) / im_size)
    red_dict["green"] = data_list[0]
    red_dict["yellow"] = data_list[1]
    red_dict["red"] = data_list[2]
    green_dict["green"] = data_list[3]
    green_dict["yellow"] = data_list[4]
    green_dict["red"] = data_list[5]
    return red_dict, green_dict

'''
zooms in to the screen
'''
def zoom_in(i):
    padding = 30
    height = i.shape[0]
    width = i.shape[1]
    if height<150 or width<150:
        return i
    
    
    lower_white = np.array([100, 100, 100], dtype=np.uint8)
    upper_white = np.array([255, 255, 255], dtype=np.uint8)

    # Create a mask based on the white color
    mask = cv2.inRange(i, lower_white, upper_white)
    

    # Find bounding box of the mask
    coords = cv2.findNonZero(mask)
    x, y, w, h = cv2.boundingRect(coords)

    # Crop the image based on the bounding box
    cropped_image = i[y:min(y+h+padding, height), x:min(x+w+padding, width)]
    
    if cropped_image.shape[0]*cropped_image.shape[1] < 100 or cropped_image.all()==i.all():
        return i[int(0.05*height):int(0.95*height), int(0.05*width):int(0.95*width)]
    else:
        return cropped_image

# returns guessed_color, confidence
def find_color(i):
    
    red_dict, green_dict = get_data(i, red_ranges, green_ranges)
    max_red = max(red_dict.values())
    max_red_color = list(red_dict.keys())[list(red_dict.values()).index(max_red)]
    
    max_green = max(green_dict.values())
    max_green_color = list(green_dict.keys())[list(green_dict.values()).index(max_green)]
    counter = 0
    confidence = 0
    guessed_color = str()
    prev_guess_num = 0
    prev_green_sum = 0
    prev_yellow_sum = 0
    prev_red_sum = 0
    prev_guess = str()
    
    while confidence<0.4 and counter<10: #
        i = zoom_in(i)
        #cv2.imshow("image"+str(img_counter), i)
        #cv2.waitKey(0)
        red_dict, green_dict = get_data(i, red_ranges, green_ranges)

        green_sum = ((red_dict['green'] + green_dict['green'])*2 + prev_green_sum)/3
        yellow_sum = ((red_dict['yellow'] + green_dict['yellow'])*2 + prev_yellow_sum)/3
        red_sum = ((red_dict['red'] + green_dict['red'])*2 + prev_red_sum)/3

        all_sums = {'green':green_sum, 'yellow':yellow_sum, 'red':red_sum}
        guessed_color_num = max(all_sums.values())
        guessed_color = list(all_sums.keys())[list(all_sums.values()).index(guessed_color_num)]
        worst_guess_num = min(all_sums.values())
        worst_guess = list(all_sums.keys())[list(all_sums.values()).index(worst_guess_num)]
        all_sums.pop(guessed_color)
        all_sums.pop(worst_guess)
        second_guess = str(list(all_sums.keys())[0])
        second_guess_num = float(list(all_sums.values())[0])
        #print("guessed_color_num", guessed_color_num)
        #print("second_guess_num", second_guess_num)
        
        
        confidence = (guessed_color_num-second_guess_num)/2
        if (red_dict[guessed_color]<0.05 or green_dict[guessed_color]<0.05):#(confidence < 0.6 and prev_guess_num-guessed_color_num>0.1): # and something w/counter or image size
            #print("\thi")
            guessed_color_num = second_guess_num
            second_guess_num = guessed_color_num
            if guessed_color_num == green_sum:
                guessed_color = 'green'
            elif guessed_color_num == yellow_sum:
                guessed_color = 'yellow'
            else:
                guessed_color = 'red'
            confidence = (guessed_color_num-second_guess_num)/2


        prev_guess_num = guessed_color_num
        prev_guess = guessed_color
        prev_green_sum = green_sum
        prev_red_sum = red_sum
        prev_yellow_sum = yellow_sum
        counter += 1
        #print("\t", guessed_color)

        

    return guessed_color, confidence
    #print("\tgreen_sum:", green_sum)
    #print("\tyellow_sum:", yellow_sum)
    #print("\tred_sum:", red_sum)
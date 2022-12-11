import cv2
import numpy as np
import glob
import time
import copy

def close():
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def Contour(img):
    height, width, channels = img.shape
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    (thresh, binary) = cv2.threshold(img_gray, 127, 255, cv2.THRESH_BINARY)
    img_blur = cv2.GaussianBlur(binary, (11, 11), 0)
    img_edge = cv2.Canny(img_blur, 127, 127)
    contours, hierarchy = cv2.findContours(img_edge, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    img_copy = img.copy()
    cv2.drawContours(img_copy, contours, -1, (255, 255, 40), 5)
    value = [img_copy, contours]
    return value

def Draw_Contour(img1, img2): # OK
    value = Contour(img1)
    cv2.imshow("Draw Contour1", value[0])
    
    value = Contour(img2)
    cv2.imshow("Draw Contour2", value[0])
    close()

def Count_Rings(img1, img2): # OK
    value = []
    size = Contour(img1)
    value.insert(0, len(size[1]))
    size = Contour(img2)
    value.insert(1, len(size[1]))
    return value

def Find_Corners(path): # OK
    imgs = glob.glob(path + "/*.bmp")
    images = []
    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        nx, ny = 11, 8
        ret, corners = cv2.findChessboardCorners(gray, (nx, ny), None)
        if ret == True:
            cv2.drawChessboardCorners(image, (nx, ny), corners, ret)
            image = cv2.resize(image, (512, 512))
            images.append(image)
    for i in range(len(images)):
        cv2.imshow("Find_Corners", images[i])
        cv2.waitKey(500)
    close()

def Find_Intrinsic(path): # OK
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []
    imgs = glob.glob(path + "/*.bmp")
    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)
    print("Intrinsic:")
    print(mtx)

def Find_Extrinsic(path, num): # OK
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []

    imgs = glob.glob(path + "/*.bmp")
    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)
    R = cv2.Rodrigues(rvecs[num-1])
    ext = np.hstack((R[0], tvecs[num-1]))
    print("Extrinsic:")
    print(ext)

def Find_Distortion(path): # OK
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []

    imgs = glob.glob(path + "/*.bmp")

    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)
    print("Distortion:")
    print(dist)

def Show_Result(path): # Maybe ok
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []

    imgs = glob.glob(path + "/*.bmp")

    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)

    for i in range(len(imgs)):
        origin = cv2.imread(path + f"/{i+1}.bmp")
        image = cv2.imread(path + f"/{i+1}.bmp")
        h, w = image.shape[:2]
        newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (w, h), 1, (w, h))
        dst = cv2.undistort(image, mtx, dist, None, newcameramtx)
        x, y, w, h = roi
        dst = dst[y:y+h, x:x+w]
        origin = cv2.resize(origin, (512, 512))
        dst = cv2.resize(dst, (512, 512))
        show = np.hstack((origin, dst))
        cv2.imshow("Result", show)
        cv2.waitKey(500)
    close()

def Show_Words_On_Boards(path, input, text_file): # input for the text
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []

    imgs = glob.glob(path + "/*.bmp")

    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
        ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)
        
        for j in range(len(input)):
            file = cv2.FileStorage(text_file, cv2.FILE_STORAGE_READ)
            
            ch = np.float32(file.getNode(input[j].upper()).mat()).reshape(-1, 3)
            for k in range(len(ch)): # Change x and y
                temp = ch[k][0]
                ch[k][0] = ch[k][1]
                ch[k][1] = temp
            for k in range(len(ch)): # Upside down
                if ch[k][0] == 2:
                    ch[k][0] = 0
                elif ch[k][0] == 0:
                    ch[k][0] = 2
            if j == 0:
                for k in range(len(ch)):
                    ch[k][1] += 8
            elif j == 1:
                for k in range(len(ch)):
                    ch[k][1] += 4
            elif j == 2:
                pass
            elif j == 3:
                for k in range(len(ch)):
                    ch[k][1] += 8
                    ch[k][0] += 4
            elif j == 4:
                for k in range(len(ch)):
                    ch[k][1] += 4
                    ch[k][0] += 4
            elif j == 5:
                for k in range(len(ch)):
                    ch[k][0] += 4
            
            print(ch)
            imgpts, jac = cv2.projectPoints(ch, rvecs[i], tvecs[i], mtx, dist)
            for k in range(0, len(ch), 2):
                image = cv2.line(image, tuple(imgpts[k].ravel()), tuple(imgpts[k+1].ravel()), (0, 0, 255), 5)
        
        cv2.namedWindow("Augmented Reality", cv2.WINDOW_GUI_EXPANDED)
        cv2.imshow("Augmented Reality", image)
        cv2.waitKey(1000)
    
    close()

def Show_Words_Vertically(path, input, text_file): # input for the text
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)
    objp = np.zeros((11 * 8, 3), np.float32)
    objp[:, :2] = np.mgrid[0:8, 0:11].T.reshape(-1, 2)
    obj = []
    img = []

    imgs = glob.glob(path + "/*.bmp")

    for i in range(len(imgs)):
        image = cv2.imread(path + f"/{i+1}.bmp")
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        ret, corners = cv2.findChessboardCorners(gray, (8, 11), None)
        if ret == True:
            obj.append(objp)
            corners2 = cv2.cornerSubPix(gray, corners, (7, 7), (-1, -1), criteria)
            img.append(corners2)
    
        ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj, img, (2048, 2048), None, None)
        
        for j in range(len(input)):
            file = cv2.FileStorage(text_file, cv2.FILE_STORAGE_READ)
            
            ch = np.float32(file.getNode(input[j].upper()).mat()).reshape(-1, 3)
            for k in range(len(ch)): # Change x and y
                temp = ch[k][0]
                ch[k][0] = ch[k][1]
                ch[k][1] = temp
            for k in range(len(ch)): # Upside down
                if ch[k][0] == 2:
                    ch[k][0] = 0
                elif ch[k][0] == 0:
                    ch[k][0] = 2
            if j == 0:
                for k in range(len(ch)):
                    ch[k][1] += 8
            elif j == 1:
                for k in range(len(ch)):
                    ch[k][1] += 4
            elif j == 2:
                pass
            elif j == 3:
                for k in range(len(ch)):
                    ch[k][1] += 8
                    ch[k][0] += 4
            elif j == 4:
                for k in range(len(ch)):
                    ch[k][1] += 4
                    ch[k][0] += 4
            elif j == 5:
                for k in range(len(ch)):
                    ch[k][0] += 4
            
            print(ch)
            imgpts, jac = cv2.projectPoints(ch, rvecs[i], tvecs[i], mtx, dist)
            for k in range(0, len(ch), 2):
                image = cv2.line(image, tuple(imgpts[k].ravel()), tuple(imgpts[k+1].ravel()), (0, 0, 255), 5)
        
        cv2.namedWindow("Augmented Reality", cv2.WINDOW_GUI_EXPANDED)
        cv2.imshow("Augmented Reality", image)
        cv2.waitKey(1000)
    
    close()

def Stereo_Disparity_Map(img1, img2):
    stereo = cv2.StereoBM_create(numDisparities=256, blockSize=25)
    img1_gray = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
    img2_gray = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)
    disparity = stereo.compute(img1_gray, img2_gray).astype(np.float32) / 16.0
    disparity_show = cv2.normalize(disparity, None, 0, 1, cv2.NORM_MINMAX)
    cv2.namedWindow("Result", cv2.WINDOW_GUI_EXPANDED)
    cv2.imshow("Result", disparity_show)

    global img_temp
    img_temp = copy.deepcopy(img2)
    cv2.namedWindow("ImgL", cv2.WINDOW_GUI_EXPANDED)
    cv2.namedWindow("ImgR", cv2.WINDOW_GUI_EXPANDED)
    cv2.imshow("ImgL", img1)
    cv2.imshow("ImgR", img_temp)
    def Click_The_Mouse(event, x, y, flags, param):
        global img_temp
        if event == cv2.EVENT_LBUTTONDOWN:
            img_temp = copy.deepcopy(img2)
            cv2.circle(img_temp, (x - int(disparity[y][x]), y), 20, (0, 255, 0), -1)
            cv2.imshow("ImgR", img_temp)

    while True:
        cv2.setMouseCallback("ImgL", Click_The_Mouse)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break

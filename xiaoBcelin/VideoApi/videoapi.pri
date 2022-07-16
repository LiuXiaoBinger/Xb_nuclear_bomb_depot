HEADERS += \
    $$PWD/common.h \
    $$PWD/screenread.h \
    $$PWD/threadworker.h \
    $$PWD/videdialog.h \
    $$PWD/videoread.h

SOURCES += \
    $$PWD/screenread.cpp \
    $$PWD/threadworker.cpp \
    $$PWD/videdialog.cpp \
    $$PWD/videoread.cpp


INCLUDEPATH+=E:/QT/QT512/opencv-release/opencv-release/include/opencv2\
            E:\QT\QT512\opencv-release\opencv-release\include

LIBS+=  E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_calib3d420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_core420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_features2d420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_flann420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_highgui420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_imgproc420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_ml420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_objdetect420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_video420.dll.a\
        E:\QT\QT512\opencv-release\opencv-release\lib\libopencv_videoio420.dll.a

FORMS += \
    $$PWD/videdialog.ui



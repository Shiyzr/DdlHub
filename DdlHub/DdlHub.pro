QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AchievementWidget.cpp \
    ChangePasswordDialog.cpp \
    CircularProgressBar.cpp \
    CourseSchedule.cpp \
    CustomMenuButton.cpp \
    DeadlineWidget.cpp \
    DurationDialog.cpp \
    Item.cpp \
    ItemWidget.cpp \
    LoginDialog.cpp \
    RegisterDialog.cpp \
    ScheduleButton.cpp \
    SlideMenu.cpp \
    SlideMenuButton.cpp \
    TodayTaskWindow.cpp \
    TopMenu.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AchievementWidget.h \
    ChangePasswordDialog.h \
    CircularProgressBar.h \
    CourseSchedule.h \
    CustomMenuButton.h \
    DeadlineWidget.h \
    DurationDialog.h \
    ItemWidget.h \
    LoginDialog.h \
    RegisterDialog.h \
    ScheduleButton.h \
    SlideMenu.h \
    SlideMenuButton.h \
    TodayTaskWindow.h \
    TopMenu.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc

#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T08:56:56
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProductManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    DialogCarriage.cpp \
    DialogProduct.cpp \
    ModelCarriage.cpp \
    DialogBusiness.cpp \
    DataManager.cpp \
    ModelBusiness.cpp \
    ModelProduct.cpp \
    ModelSale.cpp \
    DialogSale.cpp \
    DialogSelectBusiness.cpp \
    DialogSelectProduct.cpp \
    DialogLogin.cpp \
    DialogUser.cpp \
    DialogModifyPassword.cpp

HEADERS  += mainwindow.h \
    DialogCarriage.h \
    DialogProduct.h \
    ModelCarriage.h \
    CommonDefine.h \
    DialogBusiness.h \
    DataManager.h \
    ModelBusiness.h \
    ModelProduct.h \
    ModelSale.h \
    DialogSale.h \
    DialogSelectBusiness.h \
    DialogSelectProduct.h \
    DialogLogin.h \
    DialogUser.h \
    DialogModifyPassword.h

FORMS    += mainwindow.ui \
    DialogCarriage.ui \
    DialogProduct.ui \
    DialogBusiness.ui \
    DialogSale.ui \
    DialogSelectBusiness.ui \
    DialogSelectProduct.ui \
    DialogLogin.ui \
    DialogUser.ui \
    DialogModifyPassword.ui

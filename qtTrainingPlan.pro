# -------------------------------------------------
# Project created by QtCreator 2009-11-19T07:36:49
# -------------------------------------------------
#QT += webkit \
#    xml

QT       += core gui xml webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = qtTrainingPlan
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    ctraining.cpp \
    common.cpp \
    ctime.cpp \
    cclientdata.cpp \
    cedittraining.cpp \
    ceditday.cpp \
    ceditsession.cpp \
    cedittask.cpp \
    cedittasks.cpp \
    cexportpdf.cpp \
    ceditsport.cpp \
    cedithr.cpp
HEADERS += mainwindow.h \
    ctraining.h \
    common.h \
    ctime.h \
    cclientdata.h \
    cedittraining.h \
    ceditday.h \
    ceditsession.h \
    cedittask.h \
    cedittasks.h \
    cexportpdf.h \
    ceditsport.h \
    cedithr.h
FORMS += mainwindow.ui \
    cedittraining.ui \
    ceditday.ui \
    ceditsession.ui \
    cedittask.ui \
    cedittasks.ui \
    cexportpdf.ui \
    ceditsport.ui \
    cedithr.ui
TRANSLATIONS = qtTrainingPlan_de_AT.ts \
    qtTrainingPlan_de_DE.ts

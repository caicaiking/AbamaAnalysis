#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T15:19:03
#
#-------------------------------------------------

QT       += core gui sql network concurrent printsupport

INCLUDEPATH += Stocks \
                Windows \
                DataBase \
                Strategy \
                CandlePlot

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AbamaStockAnalysis
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


SOURCES += \
        main.cpp \
    Stocks/BlockData.cpp \
    Stocks/qeastmoneyblockthread.cpp \
    Stocks/qeastmoneystockinfothread.cpp \
    Stocks/qexchangedatamanage.cpp \
    DataBase/clsDBOp.cpp \
    Windows/clsMainWindow.cpp \
    DataBase/clsDBCreateTables.cpp \
    Stocks/clsStockHisThread.cpp \
    Strategy/clsStrategy.cpp \
    Strategy/clsMaStrategy.cpp \
    Strategy/clsAttact.cpp \
    Strategy/clsWeekMa.cpp \
    Stocks/clsGetLastWorkDay.cpp \
    CandlePlot/qchartviewer.cpp \
    CandlePlot/clsStockShow.cpp \
    Windows/clsShowStockUi.cpp



RC_FILE  =Icon.rc

DESTDIR =$$PWD/Exe/

HEADERS += \
    Stocks/basedata.h \
    Stocks/BlockData.h \
    Stocks/qeastmoneyblockthread.h \
    Stocks/qeastmoneystockinfothread.h \
    Stocks/qexchangedatamanage.h \
    DataBase/clsDBOp.h \
    Windows/clsMainWindow.h \
    DataBase/clsDBCreateTables.h \
    Stocks/clsSingleStockData.h \
    Stocks/clsStockHisThread.h \
    Strategy/clsStrategy.h \
    Strategy/clsMaStrategy.h \
    Strategy/clsAttact.h \
    Strategy/clsWeekMa.h \
    Stocks/clsGetLastWorkDay.h \
    Strategy/clsStrategyFactory.h \
    CandlePlot/bchartdir.h \
    CandlePlot/chartdir.h \
    CandlePlot/FinanceChart.h \
    CandlePlot/memblock.h \
    CandlePlot/qchartviewer.h \
    CandlePlot/clsStockShow.h \
    Windows/clsShowStockUi.h


FORMS += \
    Windows/clsMainWindow.ui \
    Windows/clsShowStockUi.ui


DEFINES += CHARTDIR_HIDE_OBSOLETE _CRT_SECURE_NO_WARNINGS

unix:!macx: LIBS += -L$$PWD/Libs/ -lchartdir

win32: LIBS += -L$$PWD/Libs/ -lchartdir60

RESOURCES += \
    MyRes.qrc


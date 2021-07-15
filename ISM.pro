QT       += core gui multimedia network

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = ISM
DESTDIR = ../ISM_BIN/

# 定义 Log4Qt 源码根目录
LOG4QT_ROOT_PATH = $$PWD/../Log4Qt-master


greaterThan(QT_MAJOR_VERSION,4){
        TARGET_ARCH=$${QT_ARCH}
        QT += widgets
}else{
        TARGET_ARCH=$${QMAKE_HOST.arch}
}
contains(TARGET_ARCH, x86_64){
        message("64-bit")
        LIBS += -L$$PWD/../ISM_BIN/log4Qt/x64/release -llog4qt
}else{
        message("32-bit")
}

INCLUDEPATH += 	frame \
    wndManger \
    ticket \
    qrCode \
    inquiry \
    info \
    guide \
    data \
    data/line \
    sdk/include \
    $$LOG4QT_ROOT_PATH/src \
    $$LOG4QT_ROOT_PATH/src/log4qt \
    $$LOG4QT_ROOT_PATH/include \
    $$LOG4QT_ROOT_PATH/include/log4qt

SOURCES += \
    data/CardInfo.cpp \
    data/DataCenter.cpp \
    data/HttpTool.cpp \
    data/QRCodeInfo.cpp \
    data/SettingCenter.cpp \
    data/TicketInfo.cpp \
    data/TransactionInfo.cpp \
    data/line/ISMTimeTable.cpp \
    data/line/InterchangeInfo.cpp \
    data/line/InterchangeStation.cpp \
    data/line/LineInfo.cpp \
    data/line/LineInterchangeInfo.cpp \
    data/line/LineStations.cpp \
    data/line/LineTimeTables.cpp \
    data/line/Station.cpp \
    frame/CustomTabWidget.cpp \
    frame/ISMFrame.cpp \
    frame/MainWidget.cpp \
    frame/StationListWidget.cpp \
    frame/StationSelectWidget.cpp \
    frame/StatusBar.cpp \
    frame/TitleBar.cpp \
    guide/GuideMainWidget.cpp \
    info/InfoMainWidget.cpp \
    info/LineWidget.cpp \
    info/MapWidget.cpp \
    info/MetroInterchangeWidget.cpp \
    info/MetroPeripheralWidget.cpp \
    info/TicketPriceWidget.cpp \
    info/TimeTableWidget.cpp \
    inquiry/InquiryMainWidget.cpp \
    inquiry/QNChatMessage.cpp \
    main.cpp \
    qrCode/QrCodeMainWidget.cpp \
    qrCode/QrQueryWidget.cpp \
    qrCode/QrReregisterWidget.cpp \
    ticket/PaymentWidget.cpp \
    ticket/RefundWidget.cpp \
    ticket/TicketMainWidget.cpp \
    ticket/TicketQueryWidget.cpp \
    ticket/TicketReregisterWidget.cpp \
    ticket/TicketTransactionWidget.cpp \
    wndManger/WidgetBase.cpp \
    wndManger/WidgetMng.cpp

HEADERS += \
    CommonHead.h \
    data/CardInfo.h \
    data/DataCenter.h \
    data/HttpTool.h \
    data/QRCodeInfo.h \
    data/SettingCenter.h \
    data/TicketInfo.h \
    data/TransactionInfo.h \
    data/line/ISMTimeTable.h \
    data/line/InterchangeInfo.h \
    data/line/InterchangeStation.h \
    data/line/LineInfo.h \
    data/line/LineInterchangeInfo.h \
    data/line/LineStations.h \
    data/line/LineTimeTables.h \
    data/line/Station.h \
    frame/CustomTabWidget.h \
    frame/ISMFrame.h \
    frame/MainWidget.h \
    frame/Singleton.h \
    frame/StationListWidget.h \
    frame/StationSelectWidget.h \
    frame/StatusBar.h \
    frame/TitleBar.h \
    frame/myhelper.h \
    guide/GuideMainWidget.h \
    info/InfoMainWidget.h \
    info/LineWidget.h \
    info/MapWidget.h \
    info/MetroInterchangeWidget.h \
    info/MetroPeripheralWidget.h \
    info/TicketPriceWidget.h \
    info/TimeTableWidget.h \
    inquiry/InquiryMainWidget.h \
    inquiry/QNChatMessage.h \
    qrCode/QrCodeMainWidget.h \
    qrCode/QrQueryWidget.h \
    qrCode/QrReregisterWidget.h \
    ticket/PaymentWidget.h \
    ticket/RefundWidget.h \
    ticket/TicketMainWidget.h \
    ticket/TicketQueryWidget.h \
    ticket/TicketReregisterWidget.h \
    ticket/TicketTransactionWidget.h \
    wndManger/WidgetBase.h \
    wndManger/WidgetMng.h

FORMS += \
    frame/ISMFrame.ui \
    frame/MainWidget.ui \
    frame/StationListWidget.ui \
    frame/StationSelectWidget.ui \
    frame/StatusBar.ui \
    frame/TitleBar.ui \
    guide/GuideMainWidget.ui \
    info/InfoMainWidget.ui \
    info/LineWidget.ui \
    info/MapWidget.ui \
    info/MetroInterchangeWidget.ui \
    info/MetroPeripheralWidget.ui \
    info/TicketPriceWidget.ui \
    info/TimeTableWidget.ui \
    inquiry/InquiryMainWidget.ui \
    qrCode/QrCodeMainWidget.ui \
    qrCode/QrQueryWidget.ui \
    qrCode/QrReregisterWidget.ui \
    ticket/PaymentWidget.ui \
    ticket/RefundWidget.ui \
    ticket/TicketMainWidget.ui \
    ticket/TicketQueryWidget.ui \
    ticket/TicketReregisterWidget.ui \
    ticket/TicketTransactionWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

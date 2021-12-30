QT       += core gui multimedia network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = ISM
DESTDIR = ../ISM_BIN/
#UI_DIR += $$PWD/forms
#RCC_DIR += $$PWD/tmp
#MOC_DIR += $$PWD/tmp
#OBJECTS_DIR += $$PWD/tmp
#DEPENDPATH += . forms include qrc sources

RC_FILE += res.rc

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
        LIBS += -L$$PWD/sdk/lib/log4qt/x64/release -llog4qt
        LIBS += -L$$PWD/sdk/lib/ticket/x64/release -lNC_ReaderLib -lNCNetwork_Lib -lBIM2020A
        LIBS += -L$$PWD/sdk/lib/curl -llibcurl
        LIBS += -L$$PWD/sdk/lib -lQkeyTools
}else{
        message("32-bit")
        LIBS += -L$$PWD/sdk/lib/log4qt/x86 -llog4qt
        LIBS += -L$$PWD/sdk/lib/ticket/x86/release -lBRCBoard -lBIM2020 -lF53Board -lIOBoard -lNC_ReaderLib
}

include(./qtsingleapplication/qtsingleapplication.pri)

INCLUDEPATH += 	frame \
    wndManger \
    ticket \
    qrCode \
    inquiry \
    info \
    guide \
    qtftp \
    device \
    data \
    data/line \
    data/asr \
    data/ticket \
    data/thread \
    sdk/include \
    sdk/include/ticket \
    sdk/include/curl \
    $$LOG4QT_ROOT_PATH/src \
    $$LOG4QT_ROOT_PATH/src/log4qt \
    $$LOG4QT_ROOT_PATH/include \
    $$LOG4QT_ROOT_PATH/include/log4qt

SOURCES += \
    data/BasicInfo.cpp \
    data/BomParamVersionInfo.cpp \
    data/DataCenter.cpp \
    data/HttpTool.cpp \
    data/LibcurlFtp.cpp \
    data/LoginInfo.cpp \
    data/OperatorInfo.cpp \
    data/ReaderSoftFileInfo.cpp \
    data/SettingCenter.cpp \
    data/TestWidget.cpp \
    data/TicketUpdateTestDlg.cpp \
    data/TradeFileInfo.cpp \
    data/UpdateParamInfo.cpp \
    data/X7000FileInfo.cpp \
    data/asr/ASRHttpTool.cpp \
    data/asr/AsrConfig.cpp \
    data/asr/AsrError.cpp \
    data/asr/AsrResult.cpp \
    data/asr/AsrReturn.cpp \
    data/line/ISMTimeTable.cpp \
    data/line/InterchangeInfo.cpp \
    data/line/InterchangeStation.cpp \
    data/line/LineInfo.cpp \
    data/line/LineInterchangeInfo.cpp \
    data/line/LineStationTimetables.cpp \
    data/line/LineStations.cpp \
    data/line/LineTimeTables.cpp \
    data/line/Station.cpp \
    data/line/StationTime.cpp \
    data/thread/AFCTimerTask.cpp \
    data/thread/FileDeleteTask.cpp \
    data/thread/FtpDownloadTask.cpp \
    data/thread/FtpUploadTask.cpp \
    data/thread/HeartTask.cpp \
    data/thread/ISMHttpTask.cpp \
    data/thread/Task.cpp \
    data/thread/TaskThread.cpp \
    data/thread/TradeFileUploadTask.cpp \
    data/ticket/QRCodeInfo.cpp \
    data/ticket/ReregisterInfo.cpp \
    data/ticket/TicketBasicInfo.cpp \
    data/ticket/TransactionInfo.cpp \
    device/CashboxWorker.cpp \
    device/DeviceManager.cpp \
    device/IsmDeviceManager.cpp \
    device/ReaderWorker.cpp \
    frame/AFCTaskThread.cpp \
    frame/CustomTabWidget.cpp \
    frame/ISMFrame.cpp \
    frame/ISMMessageBox.cpp \
    frame/ISMWaiting.cpp \
    frame/LoginDlg.cpp \
    frame/LogoutDlg.cpp \
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
    info/RbTableHeaderView.cpp \
    info/TDMSummaryTableWgt.cpp \
    info/TicketPriceWidget.cpp \
    info/TimeTableWidget.cpp \
    inquiry/InquiryMainWidget.cpp \
    inquiry/QNChatMessage.cpp \
    main.cpp \
    qrCode/QrCodeMainWidget.cpp \
    qrCode/QrQueryWidget.cpp \
    qrCode/QrReregisterWidget.cpp \
    ticket/CardReadWidget.cpp \
    ticket/CompensationFareWidget.cpp \
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
    data/BasicInfo.h \
    data/BomParamVersionInfo.h \
    data/DataCenter.h \
    data/HttpTool.h \
    data/LibcurlFtp.h \
    data/LoginInfo.h \
    data/OperatorInfo.h \
    data/ReaderSoftFileInfo.h \
    data/SettingCenter.h \
    data/TestWidget.h \
    data/TicketUpdateTestDlg.h \
    data/TradeFileInfo.h \
    data/UpdateParamInfo.h \
    data/X7000FileInfo.h \
    data/asr/ASRHttpTool.h \
    data/asr/AsrConfig.h \
    data/asr/AsrError.h \
    data/asr/AsrResult.h \
    data/asr/AsrReturn.h \
    data/line/ISMTimeTable.h \
    data/line/InterchangeInfo.h \
    data/line/InterchangeStation.h \
    data/line/LineInfo.h \
    data/line/LineInterchangeInfo.h \
    data/line/LineStationTimetables.h \
    data/line/LineStations.h \
    data/line/LineTimeTables.h \
    data/line/Station.h \
    data/line/StationTime.h \
    data/thread/AFCTimerTask.h \
    data/thread/FileDeleteTask.h \
    data/thread/FtpDownloadTask.h \
    data/thread/FtpUploadTask.h \
    data/thread/HeartTask.h \
    data/thread/ISMHttpTask.h \
    data/thread/Task.h \
    data/thread/TaskThread.h \
    data/thread/TradeFileUploadTask.h \
    data/ticket/QRCodeInfo.h \
    data/ticket/ReregisterInfo.h \
    data/ticket/TicketBasicInfo.h \
    data/ticket/TransactionInfo.h \
    device/CashboxWorker.h \
    device/DeviceManager.h \
    device/IsmDeviceManager.h \
    device/ReaderWorker.h \
    frame/AFCTaskThread.h \
    frame/CustomTabWidget.h \
    frame/ISMFrame.h \
    frame/ISMMessageBox.h \
    frame/ISMWaiting.h \
    frame/LoginDlg.h \
    frame/LogoutDlg.h \
    frame/MainWidget.h \
    frame/MyHelper.h \
    frame/Singleton.h \
    frame/StationListWidget.h \
    frame/StationSelectWidget.h \
    frame/StatusBar.h \
    frame/TitleBar.h \
    guide/GuideMainWidget.h \
    info/InfoMainWidget.h \
    info/LineWidget.h \
    info/MapWidget.h \
    info/MetroInterchangeWidget.h \
    info/MetroPeripheralWidget.h \
    info/RbTableHeaderView.h \
    info/TDMSummaryTableWgt.h \
    info/TicketPriceWidget.h \
    info/TimeTableWidget.h \
    inquiry/InquiryMainWidget.h \
    inquiry/QNChatMessage.h \
    qrCode/QrCodeMainWidget.h \
    qrCode/QrQueryWidget.h \
    qrCode/QrReregisterWidget.h \
    ticket/CardReadWidget.h \
    ticket/CompensationFareWidget.h \
    ticket/PaymentWidget.h \
    ticket/RefundWidget.h \
    ticket/TicketMainWidget.h \
    ticket/TicketQueryWidget.h \
    ticket/TicketReregisterWidget.h \
    ticket/TicketTransactionWidget.h \
    wndManger/WidgetBase.h \
    wndManger/WidgetMng.h

FORMS += \
    data/TestWidget.ui \
    data/TicketUpdateTestDlg.ui \
    frame/ISMFrame.ui \
    frame/ISMMessageBox.ui \
    frame/ISMWaiting.ui \
    frame/LoginDlg.ui \
    frame/LogoutDlg.ui \
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
    ticket/CardReadWidget.ui \
    ticket/CompensationFareWidget.ui \
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





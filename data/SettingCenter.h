#ifndef SETTINGCENTER_H
#define SETTINGCENTER_H

#include <QObject>
#include <QJsonArray>

class LineInfo;
class LineTimeTables;
class LineStations;
class LineInterchangeInfo;
class InterchangeInfo;
class BasicInfo;
class BomParamVersionInfo;
class TradeFileInfo;
class ReaderSoftFileInfo;
class UpdateParamInfo;
class LineStationTimetables;
class X7000FileInfo;
class OperatorInfo;
class SettingCenter : public QObject
{
    Q_OBJECT
public:
    explicit SettingCenter(QObject *parent = nullptr);
    static SettingCenter* getThis();
    static SettingCenter* m_pInstance;

public:
    // 交易序列号
    void saveTradeFileInfo(TradeFileInfo* info);
    TradeFileInfo* getTradeFileInfo();

    // 线路基础信息
    void saveLineBasicInfo(QList<LineInfo*> lines);
    QList<LineInfo*> getLineBasicInfo();
    QList<LineInfo*> parseLineBasicInfo(QJsonObject obj);

    // 线路站点信息
    void saveLineStations(QList<LineStations*> lines);
    QList<LineStations*> getLineStations();
    QList<LineStations*> parseLineStation(QJsonObject obj);

    // 线路时刻表
    void saveLineTimeTables(QList<LineTimeTables*> lines);
    QList<LineTimeTables*> getLineTimeTables();
    QList<LineTimeTables*> parseLineTimeTables(QJsonObject obj);

    // 线路换乘信息
    void saveLineInterchanes(QList<LineInterchangeInfo*> lines);
    QList<LineInterchangeInfo*> getLineInterchanes();
    QList<LineInterchangeInfo*> parseLineInterchanes(QJsonObject obj);

    // 程序基本配置信息
    BasicInfo* getBasicInfo();

    // 参数文件版本信息
    void saveParamVersionInfo(QList<BomParamVersionInfo*> list, QString fileName);
    QList<BomParamVersionInfo*> getParamVersionInfo(QString filePath);

    // 参数更新文件版本信息
    void saveUpdateParamInfo(QList<UpdateParamInfo*> list, QString fileName);
    QList<UpdateParamInfo*> getUpdateParamInfo(QString filePath);

    // 下载失败的参数文件信息
    void saveDownloadFailedFiles(QList<QString> fileNames);
    QList<QString> getDownloadFailedFiles();

    // 读写器程序文件信息
    void saveReaderSoftInfo(ReaderSoftFileInfo* info);
    ReaderSoftFileInfo* getReaderSoftInfo();

    // 线路站点信息 - 配置文件版本
    QMap<int, LineStationTimetables*> getLineStationTimetables();
    void saveLineStationTimetables(QList<LineStationTimetables*> info);

    // 操作员信息
    QList<OperatorInfo*> getOperatorConfig();

    // 测试运营时间-设置运营结束时间文件
    long getTestServiceOffTime();

    // 交易文件信息维护
    void addTradeFileInfo(QString dateStr, QString md5Str, QString tradeFileName);
    void deleteTradeFileInfo(QString dateStr, QString md5Str);
    QMap<QString,QVariant> getTradeFileToUploadByDate(QString dateStr);
    QJsonObject map2json(QMap<QString, QVariant> map);
    QMap<QString, QVariant> json2map(QJsonDocument document);

private:
    QList<InterchangeInfo*> matchInterchangeStations(QList<InterchangeInfo*> stations, QString lineCode);
    void clearJsonObject(QJsonObject &object);
    void saveJsonFile(QJsonObject rootObject, const QString &fileName);
    QJsonDocument readJsonFile(const QString &fileName);

};

#endif // SETTINGCENTER_H

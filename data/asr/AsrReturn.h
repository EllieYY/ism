#ifndef ASRRETURN_H
#define ASRRETURN_H

#include <QObject>

class AsrResult;
class AsrReturn : public QObject
{
    Q_OBJECT
public:
    explicit AsrReturn(QObject *parent = nullptr);

    QString traceToken() const;
    void setTraceToken(const QString &traceToken);

    AsrResult *result() const;
    void setResult(AsrResult *result);

    QList<QString> warning() const;
    void setWarning(const QList<QString> &warning);

private:
    QString m_traceToken;       // 服务内部的token字段，用于日志追溯
    AsrResult* m_result;        // asr识别结果
    QList<QString> m_warning;  // 可能有的警告信息

signals:

};

#endif // ASRRETURN_H

#ifndef ASRRESULT_H
#define ASRRESULT_H

#include <QObject>


/*
 * asr 返回结果
 */
class AsrResult : public QObject
{
    Q_OBJECT
public:
    explicit AsrResult(QObject *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    float confidence() const;
    void setConfidence(float confidence);

private:
    QString m_text;         // 识别出的文本内容
    float m_confidence;     // 识别的置信度（预留，不准确）

signals:

};

#endif // ASRRESULT_H

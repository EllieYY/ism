#ifndef ASRCONFIG_H
#define ASRCONFIG_H

#include <QObject>

/*
 * asr 参数配置
 */
class AsrConfig : public QObject
{
    Q_OBJECT
public:
    explicit AsrConfig(QObject *parent = nullptr);

    QString audioFormat() const;
    void setAudioFormat(const QString &audioFormat);


    bool isAddPunc() const;
    void setIsAddPunc(bool isAddPunc);

private:
    QString m_audioFormat;   // 音频数据格式 pcm_s16le_8k
    bool m_isAddPunc;        // 是否添加标点

signals:

};

#endif // ASRCONFIG_H

#include "AsrConfig.h"

AsrConfig::AsrConfig(QObject *parent) : QObject(parent)
{

}

QString AsrConfig::audioFormat() const
{
    return m_audioFormat;
}

void AsrConfig::setAudioFormat(const QString &audioFormat)
{
    m_audioFormat = audioFormat;
}

bool AsrConfig::isAddPunc() const
{
    return m_isAddPunc;
}

void AsrConfig::setIsAddPunc(bool isAddPunc)
{
    m_isAddPunc = isAddPunc;
}


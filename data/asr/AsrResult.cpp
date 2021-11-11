#include "AsrResult.h"

AsrResult::AsrResult(QObject *parent) : QObject(parent)
{

}

QString AsrResult::text() const
{
    return m_text;
}

void AsrResult::setText(const QString &text)
{
    m_text = text;
}

float AsrResult::confidence() const
{
    return m_confidence;
}

void AsrResult::setConfidence(float confidence)
{
    m_confidence = confidence;
}

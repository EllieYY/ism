#include "AsrReturn.h"

AsrReturn::AsrReturn(QObject *parent) : QObject(parent)
{

}

QString AsrReturn::traceToken() const
{
    return m_traceToken;
}

void AsrReturn::setTraceToken(const QString &traceToken)
{
    m_traceToken = traceToken;
}

AsrResult *AsrReturn::result() const
{
    return m_result;
}

void AsrReturn::setResult(AsrResult *result)
{
    m_result = result;
}

QList<QString> AsrReturn::warning() const
{
    return m_warning;
}

void AsrReturn::setWarning(const QList<QString> &warning)
{
    m_warning = warning;
}

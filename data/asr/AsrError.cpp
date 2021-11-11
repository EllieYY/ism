#include "AsrError.h"

AsrError::AsrError(QObject *parent) : QObject(parent)
{

}

int AsrError::code() const
{
    return m_code;
}

void AsrError::setCode(int code)
{
    m_code = code;
}

QString AsrError::message() const
{
    return m_message;
}

void AsrError::setMessage(const QString &message)
{
    m_message = message;
}

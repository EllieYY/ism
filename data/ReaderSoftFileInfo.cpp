#include "ReaderSoftFileInfo.h"
#include <QDate>

ReaderSoftFileInfo::ReaderSoftFileInfo(QObject *parent) : QObject(parent)
{
    hasUpdated = false;
    fileReady = false;

    QDate date = QDate::currentDate();
    dateString = date.toString("yyyy-MM-dd");
}

QString ReaderSoftFileInfo::getFileName() const
{
    return fileName;
}

void ReaderSoftFileInfo::setFileName(const QString &value)
{
    fileName = value;
}

QString ReaderSoftFileInfo::getDateString()
{
    QDate date = QDate::currentDate();
    dateString = date.toString("yyyy-MM-dd");
    return dateString;
}

bool ReaderSoftFileInfo::getHasUpdated() const
{
    return hasUpdated;
}

void ReaderSoftFileInfo::setHasUpdated(bool value)
{
    hasUpdated = value;
}

bool ReaderSoftFileInfo::getFileReady() const
{
    return fileReady;
}

void ReaderSoftFileInfo::setFileReady(bool value)
{
    fileReady = value;
}

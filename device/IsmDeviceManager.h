#ifndef ISMDEVICEMANAGER_H
#define ISMDEVICEMANAGER_H

#include <QObject>

class IsmDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit IsmDeviceManager(QObject *parent = nullptr);

signals:

};

#endif // ISMDEVICEMANAGER_H

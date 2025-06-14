//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_CLIENT_TABLE_MODEL_H
#define OMNISERVER_CLIENT_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QDateTime>

#include "net/client_info.h"

/**
 * @brief ClientTableModel - Displays list of logged in clients (their app UUIDS + host info)
 */
class ClientTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ClientTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void onClientConnected(const ClientInfo& clientInfo);
    void onClientDisconnected(const QString& clientUUID);

private:
    QVector<ClientInfo> clientRows;

    const int NUM_COLUMNS = 2;

    // Find index by client uuid, or -1 if not found
    int indexOf(const QString &clientUUID) const;
};


#endif //OMNISERVER_CLIENT_TABLE_MODEL_H

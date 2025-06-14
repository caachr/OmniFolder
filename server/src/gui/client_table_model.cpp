//
// Created by Christian Caamano on 6/6/25.
//

#include "client_table_model.h"

ClientTableModel::ClientTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int ClientTableModel::rowCount(const QModelIndex &parent) const
{
    // ???
    if (parent.isValid()) {
        return 0;
    }
    return clientRows.size();
}

int ClientTableModel::columnCount(const QModelIndex &parent) const
{
    // ???
    if (parent.isValid()) {
        return 0;
    }
    return NUM_COLUMNS;
}

QVariant ClientTableModel::data(const QModelIndex &index, int role) const
{
    // ???
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const auto& clientRow = clientRows.at(index.row());
    switch (index.column()) {
        case 0: return clientRow.clientUUID;
        case 1: return clientRow.clientHost;
        default: return {};      // ???
    }
}

QVariant ClientTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // ???
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return {};
    }

    switch (section) {
        case 0: return "Client Application UUID";
        case 1: return "Host Machine Public IP";
        default: return {};
    }
}

void ClientTableModel::onClientConnected(const ClientInfo& clientInfo)
{
    beginInsertRows({}, clientRows.size(), clientRows.size());
    clientRows.append(clientInfo);
    endInsertRows();
}

void ClientTableModel::onClientDisconnected(const QString& clientUUID)
{
    int row = indexOf(clientUUID);
    if (row < 0) {
        return;
    }
    beginRemoveRows({}, row, row);
    clientRows.removeAt(row);
    endRemoveRows();
}

int ClientTableModel::indexOf(const QString &clientUUID) const
{
    for (int i = 0; i < clientRows.size(); ++i) {
        if (clientRows[i].clientUUID == clientUUID) {
            return i;
        }
    }
    return -1;
}

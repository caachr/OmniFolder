//
// Created by Christian Caamano on 6/6/25.
//

#ifndef OMNISERVER_MAIN_WINDOW_H
#define OMNISERVER_MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QTreeView>
#include <QStandardItemModel>

#include "gui/client_table_model.h"
#include "db/omni_network.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ClientTableModel* getClientTableModel() const;

    // Called by app core once the network is loaded in
    void setNetwork(const OmniNetwork* network);

private slots:
    void onNavIndexChanged(int row);

private:
    QListWidget   *navList {nullptr};   // left-side navigation
    QStackedWidget *pages {nullptr};

    ClientTableModel *clientTableModel {nullptr};

    QTreeView* foldersView {nullptr};
    QStandardItemModel* foldersModel {nullptr};

    QWidget* createDashboardPage();
    QWidget* createClientsPage();
    QWidget* createFoldersPage();
    QWidget* createSettingsPage();
};


#endif //OMNISERVER_MAIN_WINDOW_H

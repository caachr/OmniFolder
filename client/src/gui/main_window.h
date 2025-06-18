#ifndef OMNICLIENT_MAIN_WINDOW_H
#define OMNICLIENT_MAIN_WINDOW_H

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

#include "db/omni_network.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    // Called by app core once the network is loaded in
    void setNetwork(const OmniNetwork* network);

private slots:
    void onNavIndexChanged(int row);

private:
    QListWidget   *navList {nullptr};   // left-side navigation
    QStackedWidget *pages {nullptr};

    QTreeView* foldersView {nullptr};
    QStandardItemModel* foldersModel {nullptr};

    QWidget* createDashboardPage();
    QWidget* createFoldersPage();
    QWidget* createSettingsPage();
};


#endif //OMNICLIENT_MAIN_WINDOW_H

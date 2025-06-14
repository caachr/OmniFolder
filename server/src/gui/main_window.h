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

#include "gui/client_table_model.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ClientTableModel* getClientTableModel() const;

private slots:
    void onNavIndexChanged(int row);

private:
    QListWidget   *navList {nullptr};   // left-side navigation
    QStackedWidget *pages {nullptr};
    ClientTableModel *clientTableModel {nullptr};

    QWidget* createDashboardPage();
    QWidget* createClientsPage();
    QWidget* createFoldersPage();
    QWidget* createSettingsPage();
};


#endif //OMNISERVER_MAIN_WINDOW_H

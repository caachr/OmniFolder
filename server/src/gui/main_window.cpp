//
// Created by Christian Caamano on 6/6/25.
//

#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("OmniFolder Server");
    resize(800, 600);

    // Central splitter: nav list | pages
    auto splitter = new QSplitter(Qt::Horizontal, this);

    // Nav list
    navList = new QListWidget(splitter);
    navList->addItems({"Dashboard", "Clients", "Folders", "Settings"});
    navList->setFixedWidth(140);
    navList->setCurrentRow(0);
    connect(navList, &QListWidget::currentRowChanged, this, &MainWindow::onNavIndexChanged);

    // Page stack
    pages = new QStackedWidget(splitter);
    pages->addWidget(createDashboardPage());  // index 0
    pages->addWidget(createClientsPage());    // index 1
    pages->addWidget(createFoldersPage());    // index 2
    pages->addWidget(createSettingsPage());   // index 3

    setCentralWidget(splitter);
}

ClientTableModel* MainWindow::getClientTableModel() const
{
    return clientTableModel;
}

void MainWindow::onNavIndexChanged(int row)
{
    pages->setCurrentIndex(row);
}


// --------------- Page Builders ---------------

QWidget* MainWindow::createDashboardPage()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("Dashboard coming soon…", widget));
    layout->addStretch();
    return widget;
}

QWidget* MainWindow::createClientsPage()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);

    clientTableModel = new ClientTableModel(widget);
    auto *view = new QTableView(widget);
    view->setModel(clientTableModel);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setSelectionMode(QTableView::SingleSelection);
    view->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(view);
    return widget;
}

QWidget* MainWindow::createFoldersPage()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("Folders view", widget));
    layout->addStretch();
    return widget;
}

QWidget* MainWindow::createSettingsPage()
{
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);
    layout->addWidget(new QLabel("Settings", widget));
    layout->addStretch();
    return widget;
}

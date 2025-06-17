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

void MainWindow::setNetwork(const OmniNetwork* network)
{
    // Clear existing model data
    foldersModel->removeRows(0, foldersModel->rowCount());

    // Create top-level network item
    auto* networkItem = new QStandardItem(network->getName());
    networkItem->setData("Network", Qt::UserRole+1);
    auto* networkTypeItem = new QStandardItem("Network");
    foldersModel->appendRow({networkItem, networkTypeItem});

    // Add folders to the network
    for (const auto& folder : network->getFolders()) {
        auto* folderItem = new QStandardItem(folder->getName());
        folderItem->setData(folder->getId(), Qt::UserRole);
        auto* folderTypeItem = new QStandardItem("Folder");
        networkItem->appendRow({folderItem, folderTypeItem});

        // Add drives for each folder
        for (const auto& drive : folder->getDrives()) {
            auto* driveItem = new QStandardItem(drive->getName());
            driveItem->setData(drive->getId(), Qt::UserRole);

            QString driveTypeText = (drive->getType() == "F") ? "Full" : "Partial";
            auto* driveTypeItem = new QStandardItem(driveTypeText);

            folderItem->appendRow({driveItem, driveTypeItem});
        }
    }

    // Expand all items to show the full hierarchy
    foldersView->expandAll();
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

    foldersView = new QTreeView(widget);
    foldersModel = new QStandardItemModel;
    foldersModel->setHorizontalHeaderLabels({"Name", "Type"});
    foldersView->setModel(foldersModel);
    foldersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    foldersView->header()->setStretchLastSection(true);

    layout->addWidget(foldersView);
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

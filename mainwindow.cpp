#include <QGuiApplication>
#include <QScreen>
#include <QSplitter>
#include <QMenuBar>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QCloseEvent>
#include "mainwindow.h"
#include "dataarea.h"

class PrivateMainWindow
{
public:
    DataArea*       dataArea;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_data(new PrivateMainWindow)
{
    m_data->dataArea = new DataArea(this);
    setCentralWidget(m_data->dataArea);

    QMenu* fileMenu = new QMenu(this);
    fileMenu->setTitle("文件");
    fileMenu->addAction("保存");
    fileMenu->addAction("读取并制作库存数据");
    connect(fileMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
    menuBar()->addMenu(fileMenu);

    QMenu* dataMenu = new QMenu(this);
    dataMenu->setTitle("数据");
    dataMenu->addAction("录入");
    dataMenu->addAction("筛选");
    dataMenu->addAction("显示所有(清除筛选)");
    dataMenu->addAction("显示今日价格已修改货品");
    dataMenu->addAction("显示今日包邮已修改货品");
    dataMenu->addAction("显示无条码货品");
    dataMenu->addAction("设置对比库存");
    connect(dataMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
    menuBar()->addMenu(dataMenu);

    QMenu* luluMenu = new QMenu(this);
    luluMenu->setTitle("璐璐");
    luluMenu->addAction("一键筛选璐璐家上架货品");
//    luluMenu->addAction("一键导出璐璐家链接");
    luluMenu->addAction("核对璐璐家价格");
    luluMenu->addAction("筛选和璐璐家价差10元以内(含10元)");
    luluMenu->addAction("筛选和璐璐家价差10元以上(不含10元)");
    connect(luluMenu, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
    menuBar()->addMenu(luluMenu);

    adjustSize();

//    QDesktopServices::openUrl(QUrl("https://www.google.com"));
}

MainWindow::~MainWindow()
{
    delete m_data;
}

void MainWindow::menuTriggered(QAction* action)
{
    if ("保存" == action->text()) {
        m_data->dataArea->save();
    }
    else if ("读取并制作库存数据" == action->text()) {
        m_data->dataArea->readStockData();
    }
    else if ("录入" == action->text()) {
        m_data->dataArea->addNew();
    }
    else if ("筛选" == action->text()) {
        m_data->dataArea->openFilter();
    }
    else if ("显示所有(清除筛选)" == action->text()) {
        m_data->dataArea->clearFilter();
    }
    else if ("显示今日价格已修改货品" == action->text()) {
        m_data->dataArea->showPriceChangedData();
    }
    else if ("显示今日包邮已修改货品" == action->text()) {
        m_data->dataArea->showShippingChangedData();
    }
    else if ("显示无条码货品" == action->text()) {
        m_data->dataArea->showWithoutBarcodeData();
    }
    else if ("一键筛选璐璐家上架货品" == action->text()) {
        m_data->dataArea->openLuluFilter();
    }
    else if ("核对璐璐家价格" == action->text()) {
        m_data->dataArea->startCheckPrice();
    }
    else if ("一键导出璐璐家链接" == action->text()) {
        m_data->dataArea->exportLuluURL();
    }
    else if ("筛选和璐璐家价差10元以内(含10元)" == action->text()) {
        m_data->dataArea->filterLuluPriceSpread(false);
    }
    else if ("筛选和璐璐家价差10元以上(不含10元)" == action->text()) {
        m_data->dataArea->filterLuluPriceSpread(true);
    }
    else if ("设置对比库存" == action->text()) {
        m_data->dataArea->openStockComparingSetting();
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox mq;
    mq.setWindowTitle("提示");
    mq.setText("是否保存？");

    QAbstractButton* b1 = mq.addButton("保存", QMessageBox::YesRole);
    QAbstractButton* b2 = mq.addButton("不保存", QMessageBox::NoRole);
    QAbstractButton* b3 = mq.addButton("返回", QMessageBox::RejectRole);

    mq.exec();

    if (mq.clickedButton() == b1) {
        m_data->dataArea->save();
    }
    else if (mq.clickedButton() == b2) {
        QMessageBox mb;
        mb.setWindowTitle("警告！");
        mb.setText("所有未保存的数据都将丢失，确定不保存吗?");

        mb.addButton("非常确定", QMessageBox::YesRole);
        QAbstractButton* b12 = mb.addButton("再想想", QMessageBox::NoRole);

        mb.exec();

        if (mb.clickedButton() == b12) {
            e->ignore();
        }
    }
    else if (mq.clickedButton() == b3) {
        e->ignore();
    }
}

void MainWindow::adjustSize()
{
    QRect r = QGuiApplication::screens().at(0)->geometry();
    QRect r1;

    if (r.width() >= 1600) {
        r1.setWidth(1600);
    }
    else {
        r1.setWidth(r.width() - 200);
    }

    if (r.height() >= 1000) {
        r1.setHeight(900);
    }
    else {
        r1.setHeight(r.height() - 100);
    }

    r1 = QRect((r.width()-r1.width())/2, (r.height()-r1.height())/2, r1.width(), r1.height());
    setGeometry(r1);
}

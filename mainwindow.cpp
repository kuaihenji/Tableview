#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    /** 构造数据*/
    for(int index = 0;index < 2000;index ++ ){
        ModelData mdata;
        mdata.setValue("key1",std::to_string(index));
        mdata.setValue("key2",QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz").toStdString());

        if( datas.size() < TABLE_PAGE_SIZE ){
            datas.push_back(mdata);
        }else {
            dataList.push_back(datas);
            datas.clear();
            datas.push_back(mdata);
        }

        if(index + 1 == 2000 ){
            dataList.push_back(datas);
        }

    }

    QStringList columnNames = {"序号","时间"};
    QStringList columnFields = {"key1","key2"};


    pDataModel = new CommonDataModel(this);
    pDataModel->setColNameList(columnNames);
    pDataModel->setColFieldList(columnFields);

    pTableWidget = new TableOrdinaryPage(pDataModel,this);
    pTableWidget->setTableSortEnable(true);


    int pageSize = dataList.size() > 0 ? dataList.size() : 1;
    pDataModel->setModelData(&dataList[0]);
    pTableWidget->setReferencePage(1,pageSize);

    connect(pTableWidget,&TableOrdinaryPage::previousPage,this,&MainWindow::onTablePageChange);
    connect(pTableWidget,&TableOrdinaryPage::nextPage,this,&MainWindow::onTablePageChange);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    pTableWidget->resize(width(),height());
}

void MainWindow::onTablePageChange(int index)
{
    int listIndex = index - 1;

    if(listIndex >=0 && listIndex < dataList.size())
    {
        pDataModel->setModelData(&dataList[listIndex]);
    }
}

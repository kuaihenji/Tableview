#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <CommonDataModel.h>
#include <TableOrdinaryPage.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     void resizeEvent(QResizeEvent *e);

protected slots:
    void onTablePageChange(int index);

private:
    Ui::MainWindow *ui;
    ModelDatas datas;
    ModelDataList dataList;
    CommonDataModel *pDataModel;
    TableOrdinaryPage *pTableWidget;
};

#endif // MAINWINDOW_H

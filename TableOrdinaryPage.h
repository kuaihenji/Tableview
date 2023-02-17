#ifndef TABLEORDINARYPAGE_H
#define TABLEORDINARYPAGE_H

#include <QLabel>
#include <QComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QDateEdit>
#include <QLineEdit>
#include <QIntValidator>
#include <QGraphicsDropShadowEffect>

#include "CommonDataModel.h"
#include "BaseWidget.h"

/** 每页数据大小*/
#define TABLE_PAGE_SIZE 1000

class TableOrdinaryPage : public BaseWidget
{
    Q_OBJECT

public:
    explicit TableOrdinaryPage(QAbstractTableModel *model,QWidget *parent = nullptr);
    ~TableOrdinaryPage();

    /** 设置代理控件*/
    void setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate);

    void setColumnWidth(int index,int width);

    void setColumnsWidth(QStringList widths);

    void setReferencePage(int startIndex,int endIndex);

    void setTableSortEnable(const bool &flag);

    QTableView *getTableView() const;

    QAbstractTableModel *getTableModel() const;

signals :

    void previousPage(int index);

    void nextPage(int index);


protected slots:

    void on_previous_page(bool clicked);

    void on_next_page(bool clicked);

    void onRefPagePress();

protected:

    void initView();

    void resizeEvent(QResizeEvent *e);

private:

    QWidget *mBackWidget;

    bool isCustomWidth;
    QTableView *mTableView;
    QAbstractTableModel *mTableModel;
    QFont mTableFont;

    int mStartPageIndex;
    int mEndPageIndex;

    QLabel *lbRefPage;
    QLineEdit *leRefPage;
    QIntValidator *pageValidator;

    QPushButton *btPrevPage;
    QPushButton *btNextPage;
};

#endif // TABLEORDINARYPAGE_H

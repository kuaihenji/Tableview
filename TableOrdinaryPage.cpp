#include "TableOrdinaryPage.h"

TableOrdinaryPage::TableOrdinaryPage(QAbstractTableModel *model, QWidget *parent)
    :BaseWidget(parent),isCustomWidth(false)
{
    this->mTableModel = model;
    this->mStartPageIndex = 1;
    this->mEndPageIndex = 1;
    initView();

}

TableOrdinaryPage::~TableOrdinaryPage()
{

}

void TableOrdinaryPage::setItemDelegateForColumn(int column, QAbstractItemDelegate *delegate)
{
    mTableView->setItemDelegateForColumn(column,delegate);
}

void TableOrdinaryPage::setColumnWidth(int index, int width)
{
    isCustomWidth = true;
    mTableView->setColumnWidth(index,width);
}

void TableOrdinaryPage::setColumnsWidth(QStringList widths)
{
    if(widths.size()>1)
    {
        isCustomWidth = true;
    }
    int columns = mTableModel->columnCount();
    bool isOk =false;
    for(int i = 0; i< widths.size();i++)
    {
        int width = widths[i].toInt(&isOk);

        if(isOk && i <columns)
        {
            mTableView->setColumnWidth(i,width);
        }
    }
}

void TableOrdinaryPage::setReferencePage(int startIndex, int endIndex)
{
    QString sRefPageText;
    leRefPage->setText(QString::number(startIndex));

    sRefPageText.sprintf("  /  %d",endIndex);

    this->mStartPageIndex = startIndex;
    this->mEndPageIndex = endIndex;

    lbRefPage->setText(sRefPageText);
}

void TableOrdinaryPage::setTableSortEnable(const bool &flag)
{
    if(this->mTableView == nullptr){
    }else {
        this->mTableView->setSortingEnabled(flag);
    }
}

QTableView *TableOrdinaryPage::getTableView() const
{
    return this->mTableView;

}

QAbstractTableModel *TableOrdinaryPage::getTableModel() const
{
    return this->mTableModel;
}

void TableOrdinaryPage::on_previous_page(bool clicked)
{
    int pageIndex = mStartPageIndex - 1;
    if(pageIndex <= 0){
        pageIndex = 1;
    }

    mStartPageIndex = pageIndex;

    setReferencePage(mStartPageIndex,mEndPageIndex);

    emit previousPage(pageIndex);

}

void TableOrdinaryPage::on_next_page(bool clicked)
{

    int pageIndex = mStartPageIndex + 1;
    if(pageIndex > mEndPageIndex){
        pageIndex = mEndPageIndex;
    }

    mStartPageIndex = pageIndex;

    setReferencePage(mStartPageIndex,mEndPageIndex);

    emit nextPage(pageIndex);

}

void TableOrdinaryPage::onRefPagePress()
{
    QString sPageIndex = leRefPage->text();
    int pageIndex = sPageIndex.toInt();

    if( pageIndex > 0 && pageIndex <= mEndPageIndex){
        emit nextPage(pageIndex);
    }

}

void TableOrdinaryPage::initView()
{

    int xPos = 0;
    int yPos = 20;

    mBackWidget = new QWidget(this);
    mBackWidget->setStyleSheet("QWidget {background-color:#F4F4FB;}");
    mBackWidget->resize(width(),height());
    mBackWidget->move(0,0);

    mTableView = new QTableView(mBackWidget);
 //    mTableView->setShowGrid(false);
 //    mTableView->setFrameShape(QFrame::NoFrame);
 //    mTableView = new FreezeTableWidget(mTableModel,this);
     //mTableView->setStyleSheet("QTableView::item:selected{ color:white; background:rgb(0, 0, 160);}");
     mTableView->setStyleSheet("QTableView::item:selected{ color:white; background:#209FDF;}"
                                                   "QTableView {background-color:#F4F4FB;}");
     mTableView->move(0,0);
     mTableView->resize(width(),height()-100);
     mTableView->setMouseTracking(true);
//     mTableView->stackUnder(lb_tooltip);
 //    mTableView->setSortingEnabled(true);
     //mTableView->setSortingEnabled(true);
     mTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
     mTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
     mTableFont.setPixelSize(18);
     mTableView->setFont(mTableFont);
     if(nullptr != mTableModel)
     {
         mTableView->setModel(mTableModel);
         QHeaderView* CurveInfoTableHorizontalHeaderView = mTableView->horizontalHeader();
         CurveInfoTableHorizontalHeaderView->setMinimumHeight(55);
         //CurveInfoTableHorizontalHeaderView->setObjectName("CurveInfoTableHorizontalHeaderView");
         CurveInfoTableHorizontalHeaderView->setStyleSheet(QString("QHeaderView::section {background-color:#F4F4FB;color:#4e99dc;font-weight: bold;border:0px solid #caced9;"
                                                                                                            "border-bottom:1px solid #caced9;border-right:1px solid #caced9;}"));

         CurveInfoTableHorizontalHeaderView->setDefaultAlignment (Qt::AlignHCenter | Qt::AlignVCenter); //居中

         QHeaderView* CurveInfoTableVerticalHeaderView= mTableView->verticalHeader();
         //CurveInfoTableVerticalHeaderView->setObjectName("CurveInfoTableVerticalHeaderView");
         CurveInfoTableVerticalHeaderView->setStyleSheet(QString("QHeaderView::section {background-color:#F4F4FB;color:#4e99dc;font-weight: bold;border:0px solid #caced9;"
                                                                                                            "border-bottom:1px solid #caced9;border-right:1px solid #caced9;}"));
         CurveInfoTableVerticalHeaderView->setVisible(false);
         mTableView->verticalHeader()->setDefaultSectionSize(40);
     }

     pageValidator = new QIntValidator(1,999,this);

     leRefPage = new QLineEdit(mBackWidget);
     leRefPage->resize(30,30);
     leRefPage->setValidator(pageValidator);
     leRefPage->setText("1");

     connect(leRefPage,&QLineEdit::returnPressed,this,&TableOrdinaryPage::onRefPagePress);

     lbRefPage = new QLabel("  /  1",mBackWidget);
     lbRefPage->resize(70,30);

     xPos = 100;
     yPos = yPos + mTableView->y()+mTableView->height();
     btPrevPage = new QPushButton("前一页",mBackWidget);
     //btPrevPage->setObjectName("CommonWidgetBtn");
     btPrevPage->setStyleSheet(QString("QPushButton { background-color: #e5e7ef;border-radius: 6px;color: #8b8f9e; font-weight: bold;}"
                                                                "QPushButton:hover { background-color: #4e99dc;color: #ffffff;}"));
     btPrevPage->resize(100,30);
     btPrevPage->move(xPos,yPos);
     connect(btPrevPage,&QPushButton::clicked,this,&TableOrdinaryPage::on_previous_page);


     xPos = btPrevPage->x() + btPrevPage->width() + xPos;
     btNextPage = new QPushButton("下一页",mBackWidget);
     //btNextPage->setObjectName("CommonWidgetBtn");
     btNextPage->setStyleSheet(QString("QPushButton { background-color: #e5e7ef;border-radius: 6px;color: #8b8f9e; font-weight: bold;}"
                                                                "QPushButton:hover { background-color: #4e99dc;color: #ffffff;}"));


     btNextPage->resize(100,30);
     btNextPage->move(xPos,yPos);
     connect(btNextPage,&QPushButton::clicked,this,&TableOrdinaryPage::on_next_page);

}

void TableOrdinaryPage::resizeEvent(QResizeEvent *e)
{

    mBackWidget->resize(width(),height());

    mTableView->resize(width(),height()-60);
    if(mTableModel->columnCount() > 0 && ! isCustomWidth)
    {
        int colWidth = width()/mTableModel->columnCount();
        for(int i= 0; i<mTableModel->columnCount();i++)
        {
            mTableView->setColumnWidth(i,colWidth);
        }
    }

    int xPos = 0;
    int yPos = 20;
    int btSpace = 50;
    xPos = width() - 200;



    yPos = yPos + mTableView->y()+mTableView->height();
    btNextPage->move(xPos,yPos);

    int xOffest = btSpace - (lbRefPage->width() - btSpace);
    xPos = btNextPage->x() - xOffest - lbRefPage->width();
    lbRefPage->move(xPos,yPos);

    xPos = lbRefPage->x() - leRefPage->width();
    leRefPage->move(xPos,yPos);


    xOffest = btSpace - (leRefPage->width() - btSpace);
    xPos = leRefPage->x() - xOffest - btPrevPage->width();
    btPrevPage->move(xPos,yPos);


}

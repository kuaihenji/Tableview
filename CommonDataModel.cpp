#include "CommonDataModel.h"
#include <QDebug>
#include <iostream>

CommonDataModel::CommonDataModel(QObject *parent) :
    QAbstractTableModel(parent),partSortFlag(false),
    mDatas(nullptr)
{

}

int CommonDataModel::rowCount(const QModelIndex &parent) const
{
    if(nullptr !=mDatas)
    {
        return mDatas->size();
    }
    else{
        return 0;
    }
}

int CommonDataModel::columnCount(const QModelIndex &parent) const
{
    return mColNameList.size();
}

QVariant CommonDataModel::data(const QModelIndex &index, int role) const
{

    if(mDatas == nullptr || mDatas->size() <= 0 || index.row()>mDatas->size() || index.column() > mColNameList.size())
    {
        return QVariant();
    }


    ModelDatas::iterator it = mDatas->begin();

    if (role == Qt::DisplayRole)
    {

        if(index.column()<mColFieldList.size())
        {

            QString fieldValue = QString::fromStdString( (it+index.row())->getValue(mColFieldList.at(index.column()).toStdString()));


            return fieldValue;

//            bool tranFlag = false;
//            fieldValue.toDouble(&tranFlag);
//            if(tranFlag && munTranColum != index.column() ){
//                return QString::number(fieldValue.toDouble(),'f',4);
//            }else {
//                return fieldValue;
//            }
        }
    }

    if(!mToolTipColumVector.isEmpty()){
        for(int columnIndex = 0;columnIndex < mToolTipColumVector.size();columnIndex++)
        {
            if(role == Qt::ToolTipRole && index.column() == mToolTipColumVector.at(columnIndex) )
            {
                QFont toopFont;
                toopFont.setPixelSize(18);
                QString dataString = index.data(Qt::DisplayRole).toString();
                QString toopString = this->toopTipString(dataString,toopFont,400);



                QToolTip::showText(QCursor::pos(),toopString);
            }
        }
    }

    if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    if(role == Qt::BackgroundRole)
    {
        QString color = QString::fromStdString( (it+index.row())->getValue("background_color_value"));
        if(!color.isEmpty())
        {
            return QColor(color);
        }
    }

    if(role == Qt::TextColorRole){
        QString color = QString::fromStdString( (it+index.row())->getValue("text_color_value"));
        if(!color.isEmpty())
        {
            return QColor(color);
        }
    }

    return QVariant();
}

QVariant CommonDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < 0 || section >= mColNameList.size())
                return "";
            else
                return mColNameList[section];
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

typedef bool (*LessThan)(const QPair<QVariant,int> &left,const QPair<QVariant,int> &right);
bool CommonDataModel::itemLessThan(const QPair<QVariant, int> &left, const QPair<QVariant, int> &right)
{
    bool compareResult = false;
    bool tranLeftFlag = false;
    bool tranRightFlag = false;

    double dLeft = left.first.toDouble(&tranLeftFlag);
    double dRight = right.first.toDouble(&tranRightFlag);
    if(tranLeftFlag && tranRightFlag)
    {
        compareResult = dLeft < dRight;
    }else {
        compareResult = left.first<right.first;
    }

     return compareResult;
}

bool CommonDataModel::itemGreaterThan(const QPair<QVariant, int> &left, const QPair<QVariant, int> &right)
{

    bool compareResult = false;
    bool tranLeftFlag = false;
    bool tranRightFlag = false;

    double dLeft = left.first.toDouble(&tranLeftFlag);
    double dRight = right.first.toDouble(&tranRightFlag);
    if(tranLeftFlag && tranRightFlag)
    {
        compareResult = dLeft > dRight;
    }else {
        compareResult = left.first>right.first;
    }

     return compareResult;
}

void CommonDataModel::sort(int column, Qt::SortOrder order)
{
    if(mDatas == nullptr){
        return;
    }


    QVector<QPair<QVariant, int> > sortable;
    QVector<int> unsortable;


    if(!partSortFlag)
    {
        sortable.reserve(rowCount());
        unsortable.reserve(rowCount());

        for (int row = 0; row < rowCount(); ++row)
        {
            QVariant itm = this->data(this->index(row,column));

            if (!itm.isNull())
            {
                sortable.append(QPair<QVariant,int>(itm, row));
            }
            else
            {
                unsortable.append(row);
            }

        }

        LessThan lessThanFunction = (order == Qt::AscendingOrder ? &itemLessThan : &itemGreaterThan);
        std::stable_sort(sortable.begin(), sortable.end(), lessThanFunction);
        ModelDatas sort_data;

        emit layoutAboutToBeChanged();
        int nSwapCount = rowCount();
        for(int i=0;i<nSwapCount;i++)
        {
            int r = (i < sortable.count()
                     ? sortable.at(i).second
                     : unsortable.at(i - sortable.count()));
            sort_data.push_back(mDatas->at(r));
        }

        mDatas->clear();
        foreach (ModelData dataBean, sort_data) {
           mDatas->push_back(dataBean);

        }
    }
    else
    {

        int leftRowCount = rowCount() - needSortMaxRowCount;

        sortable.reserve(needSortMaxRowCount);
        unsortable.reserve(needSortMaxRowCount);

        for (int row = 0; row < needSortMaxRowCount; ++row)
        {
            QVariant itm = this->data(this->index(row,column));

            if (!itm.isNull())
            {
                sortable.append(QPair<QVariant,int>(itm, row));
            }
            else
            {
                unsortable.append(row);
            }

        }

        LessThan lessThanFunction = (order == Qt::AscendingOrder ? &itemLessThan : &itemGreaterThan);
        std::stable_sort(sortable.begin(), sortable.end(), lessThanFunction);
        ModelDatas sort_data;
        ModelDatas left_data;

        emit layoutAboutToBeChanged();
        int nSwapCount = needSortMaxRowCount;
        for(int i=0;i<nSwapCount;i++)
        {
            int r = (i < sortable.count()
                     ? sortable.at(i).second
                     : unsortable.at(i - sortable.count()));
            sort_data.push_back(mDatas->at(r));
        }

        for(int i = needSortMaxRowCount ;i < rowCount();i++)
        {
            left_data.push_back(mDatas->at(i));
        }




        mDatas->clear();
        foreach (ModelData dataBean, sort_data) {
           mDatas->push_back(dataBean);
        }

        foreach (ModelData dataBean, left_data) {
           mDatas->push_back(dataBean);
        }
    }

     emit layoutChanged();
}

void CommonDataModel::setModelData(ModelDatas *data)
{

    beginResetModel();
    mDatas = data;
    endResetModel();

}

ModelDatas *CommonDataModel::getModelData()
{
    return mDatas;
}

QString CommonDataModel::toopTipString(QString toopString, QFont stringFont, int width) const
{
    int iCurrent = 0;
    QString strToolTip = "";
    QString strTemp = "";
    QFontMetrics fontMetrics(stringFont);
    while(iCurrent < toopString.size())
    {
        strTemp += toopString.at(iCurrent);
        if(fontMetrics.width(strTemp)>=width)
        {
            strToolTip +=strTemp + "\n";
            strTemp.clear();
        }
         ++iCurrent;
    }
    strToolTip +=strTemp;
    return strToolTip;
}

QStringList CommonDataModel::getColNameList() const
{
    return mColNameList;
}

void CommonDataModel::setColNameList(const QStringList &colNameList)
{
    foreach (QString name, colNameList) {
        int index= name.indexOf("\\n");
        while(index > 0){
            name.replace(index,2,'\n');
            index= name.indexOf("\\n");
        }
        mColNameList<<name;
    }
}

QStringList CommonDataModel::getColFieldList() const
{
    return mColFieldList;
}

void CommonDataModel::setColFieldList(const QStringList &colKeyList)
{
    mColFieldList = colKeyList;
}

unsigned int CommonDataModel::getToolTipColum() const
{
    return mToolTipColum;
}

void CommonDataModel::setToolTipColum(unsigned int tipcolum)
{
    mToolTipColum = tipcolum;

    mToolTipColumVector.push_back(tipcolum);

}


void CommonDataModel::setPartSortRow(int row, bool flag)
{
    needSortMaxRowCount = row;
    partSortFlag = flag;
}

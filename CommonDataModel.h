#ifndef PEAKMONTHCHECKMODEL_H
#define PEAKMONTHCHECKMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <QToolTip>
#include "ModelData.h"


/**
@note
此类时常用表格模型
支持列排序功能；
支持表格数据字体染色；
支持表数据以提示框展示
**/

class CommonDataModel : public QAbstractTableModel
{
public:
    explicit CommonDataModel(QObject *parent = nullptr);

    // 表格的行数
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    // 表格的列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // 表格数据，返回每个单元格的各种role的值
    virtual QVariant data(const QModelIndex &index, int role  = Qt::DisplayRole ) const;

    // 编辑时实现，非必须，有修改值的需求时实现即可
    //virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    // 标志，比如是否可选或可编辑，如果需要编辑必须返回可编辑flag
    //virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    // 可以返回行列名称
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /** 列内数据大小比较*/
    static bool itemLessThan(const QPair<QVariant,int> &left,const QPair<QVariant,int> &right);
    static bool itemGreaterThan(const QPair<QVariant,int> &left,const QPair<QVariant,int> &right);

    /** 排序数据*/
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);


    void setModelData(ModelDatas *data) ;

    ModelDatas* getModelData();

    /** 提示框的适应大小换行*/
    QString toopTipString(QString toopString,QFont stringFont,int width) const;

    QStringList getColNameList() const;
    void setColNameList(const QStringList &colNameList);

    QStringList getColFieldList() const;
    void setColFieldList(const QStringList &colKeyList);

    /** 设置某列提示框显示*/
    unsigned int getToolTipColum() const;
    void setToolTipColum(unsigned int tipcolum);

    void setPartSortRow(int row,bool flag = true);


private:

    QStringList mColNameList;
    QStringList mColFieldList;
    ModelDatas *mDatas;

    unsigned int mToolTipColum;
    QVector<unsigned int> mToolTipColumVector;

    bool partSortFlag;
    int needSortMaxRowCount;
};

#endif // PEAKMONTHCHECKMODEL_H

#include "ModelCarriage.h"

ModelCarriage::ModelCarriage(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ModelCarriage::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        QString title;
        switch(section){
        case ColumnHeaderDate:
            title = u8"日期";
            break;
        case ColumnHeaderPurchaseCost:
            title = u8"进货费(元)";
            break;
        case ColumnHeaderCarriageCost:
            title = u8"运费(元)";
            break;
        case ColumnHeaderRemark:
            title = u8"备注";
            break;
        }
        return title;
    }
    return QString();
}

int ModelCarriage::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)carriages_.size();
}

int ModelCarriage::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnHeaderCount;
}

QVariant ModelCarriage::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }

    if(role != Qt::DisplayRole){
        return QVariant();
    }
    QVariant ret;
    auto& carriage = carriages_[index.row()];
    switch (index.column()) {
    case ColumnHeaderDate:
        ret = carriage->date;
        break;
    case ColumnHeaderPurchaseCost:
        ret = carriage->purchase_cost;
        break;
    case ColumnHeaderCarriageCost:
        ret = carriage->carriage_cost;
        break;
    case ColumnHeaderRemark:
        ret = carriage->remark;
        break;
    default:
        break;
    }
    return ret;
}

void ModelCarriage::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    switch (column) {
    case ColumnHeaderDate:
        std::sort(carriages_.begin(),carriages_.end(),[order](const CarriagePointer& item1,const CarriagePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->date < item2->date) : (item1->date > item2->date);
        });
        break;
    case ColumnHeaderPurchaseCost:
        std::sort(carriages_.begin(),carriages_.end(),[order](const CarriagePointer& item1,const CarriagePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->purchase_cost < item2->purchase_cost) : (item1->purchase_cost > item2->purchase_cost);
        });
        break;
    case ColumnHeaderCarriageCost:
        std::sort(carriages_.begin(),carriages_.end(),[order](const CarriagePointer& item1,const CarriagePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->carriage_cost < item2->carriage_cost) : (item1->carriage_cost > item2->carriage_cost);
        });
        break;
    case ColumnHeaderRemark:
        std::sort(carriages_.begin(),carriages_.end(),[order](const CarriagePointer& item1,const CarriagePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->remark < item2->remark) : (item1->remark > item2->remark);
        });
        break;
    default:
        break;
    }

    endResetModel();
}

std::pair<double,double> ModelCarriage::GetTotalCost()
{
    std::pair<double,double> result;
    //进货费用
    result.first = std::accumulate(carriages_.begin(),carriages_.end(),0,[](double sum,CarriagePointer& carriage)->double{
        return sum + carriage->purchase_cost;
    });
    //运费
    result.second = std::accumulate(carriages_.begin(),carriages_.end(),0,[](double sum,CarriagePointer& carriage)->double{
        return sum + carriage->carriage_cost;
    });
    return result;
}
CarriagePointer ModelCarriage::GetCarriage(const QModelIndex &index)
{
    return carriages_[index.row()];
}
void ModelCarriage::SetCarriages(std::vector<CarriagePointer>& carriages)
{
    beginResetModel();
    carriages_ = carriages;
    endResetModel();
}
void ModelCarriage::RefreshItem(const QModelIndex &index,CarriagePointer& item)
{
    carriages_[index.row()] = item;
}
void ModelCarriage::DeleteItem(const QModelIndex &index)
{
    beginRemoveRows(index.parent(),index.row(),index.row());
    carriages_.erase(carriages_.begin()+index.row());
    endRemoveRows();
}

#include "ModelBusiness.h"

ModelBusiness::ModelBusiness(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ModelBusiness::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        QString title;
        switch(section){
        case ColumnHeaderName:
            title = u8"名字";
            break;
        case ColumnHeaderPhone:
            title = u8"电话";
            break;
        case ColumnHeaderAddress:
            title = u8"地址";
            break;
        case ColumnHeaderRemark:
            title = u8"备注";
            break;
        }
        return title;
    }
    return QString();
}

int ModelBusiness::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)businesses_.size();
}

int ModelBusiness::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnHeaderCount;
}

QVariant ModelBusiness::data(const QModelIndex &index, int role) const
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
    auto& business = businesses_[index.row()];
    switch (index.column()) {
    case ColumnHeaderName:
        ret = business->name;
        break;
    case ColumnHeaderPhone:
        ret = business->phone;
        break;
    case ColumnHeaderAddress:
        ret = business->address;
        break;
    case ColumnHeaderRemark:
        ret = business->remark;
        break;
    default:
        break;
    }
    return ret;
}

void ModelBusiness::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    switch (column) {
    case ColumnHeaderName:
        std::sort(businesses_.begin(),businesses_.end(),[order](const BusinessPointer& item1,const BusinessPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->name < item2->name) : (item1->name > item2->name);
        });
        break;
    case ColumnHeaderPhone:
        std::sort(businesses_.begin(),businesses_.end(),[order](const BusinessPointer& item1,const BusinessPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->phone < item2->phone) : (item1->phone > item2->phone);
        });
        break;
    case ColumnHeaderAddress:
        std::sort(businesses_.begin(),businesses_.end(),[order](const BusinessPointer& item1,const BusinessPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->address < item2->address) : (item1->address > item2->address);
        });
        break;
    case ColumnHeaderRemark:
        std::sort(businesses_.begin(),businesses_.end(),[order](const BusinessPointer& item1,const BusinessPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->remark < item2->remark) : (item1->remark > item2->remark);
        });
        break;
    default:
        break;
    }

    endResetModel();
}

BusinessPointer ModelBusiness::GetBusiness(const QModelIndex &index)
{
    return businesses_[index.row()];
}

void ModelBusiness::SetBusinesses(std::vector<BusinessPointer>& businesses)
{
    beginResetModel();
    businesses_ = businesses;
    endResetModel();
}

void ModelBusiness::RefreshItem(const QModelIndex &index,BusinessPointer& item)
{
    businesses_[index.row()] = item;
}

void ModelBusiness::DeleteItem(const QModelIndex &index)
{
    beginRemoveRows(index.parent(),index.row(),index.row());
    businesses_.erase(businesses_.begin()+index.row());
    endRemoveRows();
}

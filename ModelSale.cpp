#include "ModelSale.h"

ModelSale::ModelSale(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ModelSale::headerData(int section, Qt::Orientation orientation, int role) const
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
        case ColumnHeaderProductName:
            title = u8"商品名";
            break;
        case ColumnHeaderTotalPrice:
            title = u8"总价格";
            break;
        case ColumnHeaderSaleCount:
            title = u8"销售数量";
            break;
        case ColumnHeaderRemark:
            title = u8"备注";
            break;
        }
        return title;
    }
    return QString();
}

int ModelSale::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)sales_.size();
}

int ModelSale::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnHeaderCount;
}

QVariant ModelSale::data(const QModelIndex &index, int role) const
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
    auto& sale = sales_[index.row()];
    switch (index.column()) {
    case ColumnHeaderDate:
        ret = sale->date;
        break;
    case ColumnHeaderProductName:
        if(sale->product){
            ret = sale->product->name;
        }else{
            ret = sale->product_name;
        }
        break;
    case ColumnHeaderTotalPrice:
        ret = sale->total_price;
        break;
    case ColumnHeaderSaleCount:
        ret = sale->count;
        break;
    case ColumnHeaderRemark:
        ret = sale->remark;
        break;
    default:
        break;
    }
    return ret;
}

void ModelSale::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    switch (column) {
    case ColumnHeaderDate:
        std::sort(sales_.begin(),sales_.end(),[order](const SalePointer& item1,const SalePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->date < item2->date) : (item1->date > item2->date);
        });
        break;
    case ColumnHeaderProductName:
        std::sort(sales_.begin(),sales_.end(),[order](const SalePointer& item1,const SalePointer& item2)->bool{
            if(item1->product && item2->product){
                return order == Qt::AscendingOrder ? (item1->product->name < item2->product->name) : (item1->product->name > item2->product->name);
            }else{
                return order == Qt::AscendingOrder ? (item1->product_name < item2->product_name) : (item1->product_name > item2->product_name);
            }
        });
        break;
    case ColumnHeaderTotalPrice:
        std::sort(sales_.begin(),sales_.end(),[order](const SalePointer& item1,const SalePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->total_price < item2->total_price) : (item1->total_price > item2->total_price);
        });
        break;
    case ColumnHeaderSaleCount:
        std::sort(sales_.begin(),sales_.end(),[order](const SalePointer& item1,const SalePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->count < item2->count) : (item1->count > item2->count);
        });
        break;
    case ColumnHeaderRemark:
        std::sort(sales_.begin(),sales_.end(),[order](const SalePointer& item1,const SalePointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->remark < item2->remark) : (item1->remark > item2->remark);
        });
        break;
    default:
        break;
    }

    endResetModel();
}

std::pair<double,double> ModelSale::GetTotalSales()
{
    std::pair<double,double> result;
    //总成本
    result.first = std::accumulate(sales_.begin(),sales_.end(),0,[](double sum,SalePointer& sale)->double{
        return sum+sale->product->cost_price*sale->count;
    });
    //总费用
    result.second = std::accumulate(sales_.begin(),sales_.end(),0,[](double sum,SalePointer& sale)->double{
        return sum+sale->total_price;
    });
    return result;
}

SalePointer ModelSale::GetSale(const QModelIndex &index)
{
    return sales_[index.row()];
}

void ModelSale::SetSales(std::vector<SalePointer>& sales)
{
    beginResetModel();
    sales_ = sales;
    endResetModel();
}

void ModelSale::RefreshItem(const QModelIndex &index,SalePointer& item)
{
    sales_[index.row()] = item;
}

void ModelSale::DeleteItem(const QModelIndex &index)
{
    beginRemoveRows(index.parent(),index.row(),index.row());
    sales_.erase(sales_.begin()+index.row());
    endRemoveRows();
}

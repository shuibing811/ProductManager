#include "ModelProduct.h"

#include <QDebug>

ModelProduct::ModelProduct(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ModelProduct::headerData(int section, Qt::Orientation orientation, int role) const
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
            title = u8"商品名";
            break;
        case ColumnHeaderCategory:
            title = u8"类别";
            break;
        case ColumnHeaderBusiness:
            title = u8"商家";
            break;
        case ColumnHeaderCost:
            title = u8"进货价(元)";
            break;
        case ColumnHeaderSale:
            title = u8"销售价(元)";
            break;
        case ColumnHeaderStockCount:
            title = u8"库存数量";
            break;
        case ColumnHeaderRemark:
            title = u8"备注";
            break;
        }
        return title;
    }
    return QString();
}

int ModelProduct::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)products_.size();
}

int ModelProduct::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ColumnHeaderCount;
}

QVariant ModelProduct::data(const QModelIndex &index, int role) const
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
    auto& product = products_[index.row()];
    switch (index.column()) {
    case ColumnHeaderName:
        ret = product->name;
        break;
    case ColumnHeaderCategory:
        if(product->category){
            ret = product->category->name;
        }
        break;
    case ColumnHeaderBusiness:
        if(product->business){
            ret = product->business->name;
        }
        break;
    case ColumnHeaderCost:
        ret = product->cost_price;
        break;
    case ColumnHeaderStockCount:
        ret = product->stock_count;
        break;
    case ColumnHeaderSale:
        ret = product->sale_price;
        break;
    case ColumnHeaderRemark:
        ret = product->remark;
        break;
    default:
        break;
    }
    return ret;
}

void ModelProduct::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    switch (column) {
    case ColumnHeaderName:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->name < item2->name) : (item1->name > item2->name);
        });
        break;
    case ColumnHeaderCategory:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->category->name < item2->category->name) : (item1->category->name > item2->category->name);
        });
        break;
    case ColumnHeaderBusiness:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->business->name < item2->business->name) : (item1->business->name > item2->business->name);
        });
        break;
    case ColumnHeaderCost:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->cost_price < item2->cost_price) : (item1->cost_price > item2->cost_price);
        });
        break;
    case ColumnHeaderSale:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->sale_price < item2->sale_price) : (item1->sale_price > item2->sale_price);
        });
        break;
    case ColumnHeaderStockCount:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->stock_count < item2->stock_count) : (item1->stock_count > item2->stock_count);
        });
        break;
    case ColumnHeaderRemark:
        std::sort(products_.begin(),products_.end(),[order](const ProductPointer& item1,const ProductPointer& item2)->bool{
            return order == Qt::AscendingOrder ? (item1->remark < item2->remark) : (item1->remark > item2->remark);
        });
        break;
    default:
        break;
    }

    endResetModel();
}

ProductPointer ModelProduct::GetProduct(const QModelIndex &index)
{
    return products_[index.row()];
}

void ModelProduct::SetProducts(std::vector<ProductPointer>& products)
{
    beginResetModel();
    products_ = products;
    endResetModel();
}

void ModelProduct::RefreshItem(const QModelIndex &index,ProductPointer& item)
{
    products_[index.row()] = item;
}

void ModelProduct::DeleteItem(const QModelIndex &index)
{
    beginRemoveRows(index.parent(),index.row(),index.row());
    products_.erase(products_.begin()+index.row());
    endRemoveRows();
}
std::pair<double,double> ModelProduct::GetTotalCost()
{
    std::pair<double,double> result;
    //总成本
    result.first = std::accumulate(products_.begin(),products_.end(),0,[](double sum,ProductPointer& product)->double{
        return sum + product->cost_price * product->stock_count;
    });
    //总费用
    result.second = std::accumulate(products_.begin(),products_.end(),0,[](double sum,ProductPointer& product)->double{
        return sum + product->sale_price * product->stock_count;
    });
    return result;
}

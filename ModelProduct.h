#ifndef MODELPRODUCT_H
#define MODELPRODUCT_H

#include <QAbstractTableModel>

#include "commondefine.h"

class ModelProduct : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelProduct(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    std::pair<double,double> GetTotalCost();

    ProductPointer GetProduct(const QModelIndex &index);
    void SetProducts(std::vector<ProductPointer>& products);
    void RefreshItem(const QModelIndex &index,ProductPointer& item);
    void DeleteItem(const QModelIndex &index);
private:
    enum columnHeader{
        ColumnHeaderName,
        ColumnHeaderCategory,
        ColumnHeaderBusiness,
        ColumnHeaderCost,
        ColumnHeaderSale,
        ColumnHeaderStockCount,
        ColumnHeaderRemark,
        ColumnHeaderCount
    };

private:
    std::vector<ProductPointer> products_;
};

#endif // MODELPRODUCT_H

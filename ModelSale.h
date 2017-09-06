#ifndef MODELSALE_H
#define MODELSALE_H

#include <QAbstractTableModel>

#include "commondefine.h"

class ModelSale : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelSale(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    std::pair<double,double> GetTotalSales();

    SalePointer GetSale(const QModelIndex &index);
    void SetSales(std::vector<SalePointer>& sales);
    void RefreshItem(const QModelIndex &index,SalePointer& item);
    void DeleteItem(const QModelIndex &index);
private:
    enum columnHeader{
        ColumnHeaderDate,
        ColumnHeaderProductName,
        ColumnHeaderTotalPrice,
        ColumnHeaderSaleCount,
        ColumnHeaderRemark,
        ColumnHeaderCount,
    };

private:
    std::vector<SalePointer> sales_;
};

#endif // MODELSALE_H

#ifndef MODELCARRIAGE_H
#define MODELCARRIAGE_H

#include <QAbstractTableModel>
#include <vector>

#include "commondefine.h"


class ModelCarriage : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelCarriage(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    std::pair<double,double> GetTotalCost();

    CarriagePointer GetCarriage(const QModelIndex &index);
    void SetCarriages(std::vector<CarriagePointer>& carriages);
    void RefreshItem(const QModelIndex &index,CarriagePointer& item);
    void DeleteItem(const QModelIndex &index);
private:
    enum columnHeader{
        ColumnHeaderDate,
        ColumnHeaderPurchaseCost,
        ColumnHeaderCarriageCost,
        ColumnHeaderRemark,
        ColumnHeaderCount,
    };

private:
    std::vector<CarriagePointer> carriages_;
};

#endif // MODELCARRIAGE_H

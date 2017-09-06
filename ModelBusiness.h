#ifndef MODELBUSINESS_H
#define MODELBUSINESS_H

#include <QAbstractTableModel>
#include "commondefine.h"

class ModelBusiness : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelBusiness(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    BusinessPointer GetBusiness(const QModelIndex &index);
    void SetBusinesses(std::vector<BusinessPointer>& businesses);
    void RefreshItem(const QModelIndex &index,BusinessPointer& item);
    void DeleteItem(const QModelIndex &index);
private:
    enum columnHeader{
        ColumnHeaderName,
        ColumnHeaderPhone,
        ColumnHeaderAddress,
        ColumnHeaderRemark,
        ColumnHeaderCount,
    };

private:
    std::vector<BusinessPointer> businesses_;
};

#endif // MODELBUSINESS_H

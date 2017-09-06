#ifndef DIALOGSELECTPRODUCT_H
#define DIALOGSELECTPRODUCT_H

#include <QDialog>
#include <QStandardItemModel>

#include "commondefine.h"

namespace Ui {
class DialogSelectProduct;
}

class DialogSelectProduct : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectProduct(QWidget *parent = 0);
    ~DialogSelectProduct();

    ProductPointer GetProduct();
private slots:
    void on_btnOk_clicked();
    void on_edtName_textChanged(const QString &arg1);
    void on_lvProduct_doubleClicked(const QModelIndex &index);

    ProductPointer GetCurrentSelectedProduct();
private:
    Ui::DialogSelectProduct *ui;
    QStandardItemModel* standard_item_model;
    ProductPointer product_;
};

#endif // DIALOGSELECTPRODUCT_H

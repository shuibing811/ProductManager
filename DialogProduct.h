#ifndef DIALOGPRODUCT_H
#define DIALOGPRODUCT_H

#include <QDialog>

#include "commondefine.h"

namespace Ui {
class DialogProduct;
}

class DialogProduct : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProduct(QWidget *parent = 0);
    ~DialogProduct();

    void SetProduct(const ProductPointer& product);
    ProductPointer GetProduct();
private slots:
    void on_btnOk_clicked();

    void on_btnSelectBusiness_clicked();

private:
    Ui::DialogProduct *ui;

    int     product_id_;
    CategoryPointer category_;
    BusinessPointer business_;
};

#endif // DIALOGPRODUCT_H

#ifndef DIALOGSALE_H
#define DIALOGSALE_H

#include <QDialog>

#include "commondefine.h"

namespace Ui {
class DialogSale;
}

class DialogSale : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSale(QWidget *parent = 0);
    ~DialogSale();

    void SetSale(const SalePointer& sale);
    SalePointer GetSale();
private slots:
    void on_btnOk_clicked();

    void on_btnSelectProduct_clicked();

    void on_sbSaleCount_valueChanged(int arg1);

private:
    Ui::DialogSale *ui;
    int sale_id_ = 0;
    ProductPointer product_;
    int old_sale_count_ = 0;
};

#endif // DIALOGSALE_H

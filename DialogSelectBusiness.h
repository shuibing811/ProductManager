#ifndef DIALOGSELECTBUSINESS_H
#define DIALOGSELECTBUSINESS_H

#include <QDialog>
#include <QStandardItemModel>

#include "commondefine.h"

namespace Ui {
class DialogSelectBusiness;
}

class DialogSelectBusiness : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectBusiness(QWidget *parent = 0);
    ~DialogSelectBusiness();

    BusinessPointer GetBusiness();
private slots:
    void on_edtName_textChanged(const QString &arg1);
    void on_btnOk_clicked();
    void on_lvBusiness_doubleClicked(const QModelIndex &index);

    BusinessPointer GetCurrentSelectedBusiness();
private:
    Ui::DialogSelectBusiness *ui;
    QStandardItemModel* standard_item_model;
    BusinessPointer business_;
};

#endif // DIALOGSELECTBUSINESS_H

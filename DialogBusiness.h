#ifndef DIALOGBUSINESS_H
#define DIALOGBUSINESS_H

#include <QDialog>
#include <vector>
#include "commondefine.h"

namespace Ui {
class DialogBusiness;
}

class DialogBusiness : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBusiness(QWidget *parent = 0);
    ~DialogBusiness();
    void SetBusiness(const BusinessPointer& business);
    BusinessPointer GetBusiness();
private slots:
    void on_btnOk_clicked();
private:
    Ui::DialogBusiness *ui;
    int     business_id_;
};

#endif // DIALOGBUSSINESS_H

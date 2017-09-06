#ifndef DIALOGCARRIAGE_H
#define DIALOGCARRIAGE_H

#include <QDialog>
#include "commondefine.h"

namespace Ui {
class DialogCarriage;
}

class DialogCarriage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCarriage(QWidget *parent = 0);
    ~DialogCarriage();

    void SetCarriage(const CarriagePointer& carriage);
    CarriagePointer GetCarriage();
private slots:
    void on_btnOk_clicked();

private:
    Ui::DialogCarriage *ui;
    int carriage_id_;
};

#endif // DIALOGCARRIAGE_H

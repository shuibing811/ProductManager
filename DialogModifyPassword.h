#ifndef DIALOGMODIFYPASSWORD_H
#define DIALOGMODIFYPASSWORD_H

#include <QDialog>

namespace Ui {
class DialogModifyPassword;
}

class DialogModifyPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModifyPassword(QWidget *parent = 0);
    ~DialogModifyPassword();

private slots:
    void on_btnOk_clicked();

private:
    Ui::DialogModifyPassword *ui;
};

#endif // DIALOGMODIFYPASSWORD_H

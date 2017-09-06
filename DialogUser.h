#ifndef DIALOGUSER_H
#define DIALOGUSER_H

#include <QDialog>

namespace Ui {
class DialogUser;
}

class DialogUser : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUser(QWidget *parent = 0);
    ~DialogUser();

private slots:
    void on_btnOk_clicked();

private:
    Ui::DialogUser *ui;
};

#endif // DIALOGUSER_H

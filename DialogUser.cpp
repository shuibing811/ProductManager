#include "DialogUser.h"
#include "ui_DialogUser.h"

DialogUser::DialogUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUser)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DialogUser::~DialogUser()
{
    delete ui;
}

void DialogUser::on_btnOk_clicked()
{
    accept();
}

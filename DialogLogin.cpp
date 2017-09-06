#include "DialogLogin.h"
#include "ui_DialogLogin.h"

#include <QMessageBox>

#include "DataManager.h"
#include "DialogModifyPassword.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    connect(ui->btnExit,SIGNAL(clicked()),this,SLOT(reject()));
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::on_btnLogin_clicked()
{
    QString username = ui->edtUserName->text();
    QString password = ui->edtPassword->text();
    bool is_name_or_password_error = true;
    if(!g_data_manager.CheckUserLogin(username,password,is_name_or_password_error)){
        if(is_name_or_password_error){
            QMessageBox::warning(this,u8"错误",u8"登录失败,用户名或密码错误");
        }else{
            QMessageBox::warning(this,u8"错误",u8"登录失败,数据库错误");
        }
       return;
    }
    accept();
}

void DialogLogin::on_btnModifyPassword_clicked()
{
    DialogModifyPassword dialog_modify_password(this);
    dialog_modify_password.exec();
}

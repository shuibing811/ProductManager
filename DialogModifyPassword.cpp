#include "DialogModifyPassword.h"
#include "ui_DialogModifyPassword.h"

#include <QMessageBox>

#include "DataManager.h"

DialogModifyPassword::DialogModifyPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModifyPassword)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DialogModifyPassword::~DialogModifyPassword()
{
    delete ui;
}

void DialogModifyPassword::on_btnOk_clicked()
{
    if(ui->edtNewPassword1->text() != ui->edtNewPassword2->text()){
        QMessageBox::warning(this,u8"错误",u8"两次输入的新密码不一致");
        return;
    }
    if(ui->edtNewPassword1->text().isEmpty()){
        QMessageBox::warning(this,u8"错误",u8"密码不能为空");
        return;
    }
    if(!g_data_manager.HasUser(ui->edtUserName->text())){
        QMessageBox::warning(this,u8"错误",u8"无此用户");
        return;
    }
    bool is_name_or_password_error;
    if(!g_data_manager.CheckUserLogin(ui->edtUserName->text(),ui->edtOldPassword->text(),is_name_or_password_error)){
        QMessageBox::warning(this,u8"错误",u8"用户原密码错误");
        return;
    }
    if(!g_data_manager.ModifyUserPassword(ui->edtUserName->text(),ui->edtNewPassword1->text())){
        QMessageBox::warning(this,u8"错误",u8"修改密码错误");
        return;
    }
    QMessageBox::information(this,u8"信息",u8"修改密码成功");
    accept();
}

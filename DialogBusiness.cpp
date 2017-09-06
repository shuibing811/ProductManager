#include "DialogBusiness.h"
#include "ui_DialogBusiness.h"
#include <QMessageBox>

DialogBusiness::DialogBusiness(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBusiness)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    business_id_ = 0;
}

DialogBusiness::~DialogBusiness()
{
    delete ui;
}

void DialogBusiness::SetBusiness(const BusinessPointer& business)
{
    business_id_ = business->id;
    ui->edtName->setText(business->name);
    ui->edtPhone->setText(business->phone);
    ui->edtAddress->setText(business->address);
    ui->teRemark->setPlainText(business->remark);
}
BusinessPointer DialogBusiness::GetBusiness()
{
    BusinessPointer business = std::make_shared<Business>();
    business->id = business_id_;
    business->name = ui->edtName->text();
    business->phone = ui->edtPhone->text();
    business->address = ui->edtAddress->text();
    business->remark = ui->teRemark->toPlainText();
    return business;
}

void DialogBusiness::on_btnOk_clicked()
{
    if(ui->edtName->text().isEmpty()){
        QMessageBox::information(this,u8"提示",u8"名字不能为空");
        return;
    }
    this->accept();
}

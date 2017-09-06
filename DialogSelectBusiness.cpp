#include "DialogSelectBusiness.h"
#include "ui_DialogSelectBusiness.h"

#include "DataManager.h"
#include <QMessageBox>

DialogSelectBusiness::DialogSelectBusiness(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectBusiness)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    standard_item_model = new QStandardItemModel(this);
    ui->lvBusiness->setModel(standard_item_model);

    on_edtName_textChanged("");
}

DialogSelectBusiness::~DialogSelectBusiness()
{
    delete ui;
}

BusinessPointer DialogSelectBusiness::GetBusiness()
{
    return business_;
}

BusinessPointer DialogSelectBusiness::GetCurrentSelectedBusiness()
{
    BusinessPointer business;
    auto& index = ui->lvBusiness->currentIndex();
    if(index.isValid()){
        QStandardItem* item = standard_item_model->item(index.row());
        auto iter = g_data_manager.GetBusinesses().find(item->data(Qt::UserRole).toInt());
        business = iter->second;
    }
    return business;
}

void DialogSelectBusiness::on_edtName_textChanged(const QString &arg1)
{
    standard_item_model->clear();

    const auto& bussinesses = g_data_manager.GetBusinesses();
    QList<QStandardItem*> items;
    for(const auto& business_pair : bussinesses){
        if(business_pair.second->name.contains(arg1)){
            QStandardItem *item = new QStandardItem(business_pair.second->name);
            item->setData(business_pair.second->id,Qt::UserRole);
            standard_item_model->appendRow(item);
        }
    }
}

void DialogSelectBusiness::on_btnOk_clicked()
{
    business_ = GetCurrentSelectedBusiness();
    if(!business_){
        QMessageBox::warning(this,u8"错误",u8"请选择商家");
        return;
    }
    accept();
}

void DialogSelectBusiness::on_lvBusiness_doubleClicked(const QModelIndex &/*index*/)
{
    business_ = GetCurrentSelectedBusiness();
    accept();
}

#include "DialogSelectProduct.h"
#include "ui_DialogSelectProduct.h"

#include <QMessageBox>

#include "DataManager.h"

DialogSelectProduct::DialogSelectProduct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectProduct)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    standard_item_model = new QStandardItemModel(this);
    ui->lvProduct->setModel(standard_item_model);
    on_edtName_textChanged("");
}

DialogSelectProduct::~DialogSelectProduct()
{
    delete ui;
}

ProductPointer DialogSelectProduct::GetProduct()
{
    return product_;
}

ProductPointer DialogSelectProduct::GetCurrentSelectedProduct()
{
    ProductPointer product;
    auto& index = ui->lvProduct->currentIndex();
    if(index.isValid()){
        QStandardItem* item = standard_item_model->item(index.row());
        auto iter = g_data_manager.GetProducts().find(item->data(Qt::UserRole).toInt());
        product = iter->second;
    }
    return product;
}

void DialogSelectProduct::on_edtName_textChanged(const QString &arg1)
{
    standard_item_model->clear();

    const auto& products = g_data_manager.GetProducts();
    QList<QStandardItem*> items;
    for(const auto& product_pair : products){
        if(product_pair.second->name.contains(arg1)){
            QStandardItem *item = new QStandardItem(product_pair.second->name);
            item->setData(product_pair.second->id,Qt::UserRole);
            standard_item_model->appendRow(item);
        }
    }
}

void DialogSelectProduct::on_btnOk_clicked()
{
    product_ = GetCurrentSelectedProduct();
    if(!product_){
        QMessageBox::warning(this,u8"错误",u8"请选择商品");
        return;
    }
    accept();
}

void DialogSelectProduct::on_lvProduct_doubleClicked(const QModelIndex &/*index*/)
{
    product_ = GetCurrentSelectedProduct();
    accept();
}

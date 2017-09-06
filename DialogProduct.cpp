#include "DialogProduct.h"
#include "ui_DialogProduct.h"

#include <QMessageBox>

#include "DataManager.h"

#include "DialogSelectBusiness.h"

DialogProduct::DialogProduct(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProduct)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    //加载类别
    ui->cmbCategory->clear();
    const auto& categories = g_data_manager.GetCategories();
    for(const auto& category_pair : categories){
        ui->cmbCategory->addItem(category_pair.second->name,category_pair.second->id);
    }

    product_id_ = 0;
}

DialogProduct::~DialogProduct()
{
    delete ui;
}

void DialogProduct::SetProduct(const ProductPointer& product)
{
    product_id_ = product->id;
    category_ = product->category;
    business_ = product->business;

    ui->edtName->setText(product->name);
    if(product->category){
        ui->cmbCategory->setCurrentText(product->category->name);
    }
    if(product->business){
        ui->edtBusiness->setText(product->business->name);
    }
    ui->dsbCostPrice->setValue(product->cost_price);
    ui->dsbSalePrice->setValue(product->sale_price);
    ui->sbStockCount->setValue(product->stock_count);
    ui->teRemark->setPlainText(product->remark);
}

ProductPointer DialogProduct::GetProduct()
{
    ProductPointer product = std::make_shared<Product>();
    product->id = product_id_;
    product->name = ui->edtName->text();
    product->cost_price = ui->dsbCostPrice->value();
    product->sale_price = ui->dsbSalePrice->value();
    product->stock_count = ui->sbStockCount->value();
    product->remark = ui->teRemark->toPlainText();    
    if(category_){
        product->category = category_;
        product->category_id = category_->id;
    }
    if(business_){
        product->business = business_;
        product->business_id = business_->id;
    }
    return product;
}

void DialogProduct::on_btnOk_clicked()
{
    if(ui->edtName->text().isEmpty()){
        QMessageBox::information(this,u8"提示",u8"商品名不能为空");
        return;
    }
    if(ui->cmbCategory->currentText().isEmpty()){
        QMessageBox::information(this,u8"提示",u8"类别不能为空");
        return;
    }

    if(ui->edtBusiness->text().isEmpty()){
        QMessageBox::information(this,u8"提示",u8"商家不能为空");
        return;
    }

    //获得类别
    auto& categories = g_data_manager.GetCategories();
    auto iter = categories.find(ui->cmbCategory->currentData(Qt::UserRole).toInt());
    category_ = iter->second;

    accept();
}

void DialogProduct::on_btnSelectBusiness_clicked()
{
    DialogSelectBusiness dialog_select_business;
    if(dialog_select_business.exec() == QDialog::Accepted){
        auto business = dialog_select_business.GetBusiness();
        if(business){
            business_ = business;
            ui->edtBusiness->setText(business->name);
        }
    }
}

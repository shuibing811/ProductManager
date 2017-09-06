#include "DialogSale.h"
#include "ui_DialogSale.h"

#include <QMessageBox>

#include "DialogSelectProduct.h"

DialogSale::DialogSale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSale)
{
    ui->setupUi(this);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    ui->dteDate->setDate(QDate::currentDate());
}

DialogSale::~DialogSale()
{
    delete ui;
}

void DialogSale::SetSale(const SalePointer& sale)
{
    sale_id_ = sale->id;
    product_ = sale->product;

    if(sale->product){
        ui->edtProduct->setText(sale->product->name);
    }
    ui->dteDate->setDate(sale->date);
    ui->dsbSalePrice->setValue(sale->total_price);
    ui->sbSaleCount->setValue(sale->count);
    ui->teRemark->setPlainText(sale->remark);

    ui->dsbSalePrice->setEnabled(true);
    ui->sbSaleCount->setEnabled(true);

    old_sale_count_ = sale->count;
}

SalePointer DialogSale::GetSale()
{
    SalePointer sale = std::make_shared<Sale>();
    sale->id = sale_id_;
    sale->date = ui->dteDate->date();
    sale->total_price = ui->dsbSalePrice->value();
    sale->count = ui->sbSaleCount->value();
    sale->remark = ui->teRemark->toPlainText();
    if(product_){
        sale->product = product_;
        sale->product_id = product_->id;
        sale->product_name = product_->name;
    }
    return sale;
}

void DialogSale::on_btnOk_clicked()
{
    if(ui->edtProduct->text().isEmpty()){
        QMessageBox::information(this,u8"提示",u8"商品不能为空");
        return;
    }

    if(product_ && ui->sbSaleCount->value() > old_sale_count_){
        int sub_count = ui->sbSaleCount->value() - old_sale_count_;
        if(sub_count > product_->stock_count){
            QMessageBox::warning(this,u8"错误",u8"此商品库存不足，不能销售指定数量的商品");
            return;
        }
    }

    accept();
}

void DialogSale::on_btnSelectProduct_clicked()
{
    DialogSelectProduct dialog_select_product;
    if(dialog_select_product.exec() == QDialog::Accepted){
        auto product = dialog_select_product.GetProduct();
        if(product){
            product_ = product;
            ui->edtProduct->setText(product->name);
            if(!ui->sbSaleCount->isEnabled()){
                ui->dsbSalePrice->setEnabled(true);
                ui->sbSaleCount->setEnabled(true);
                ui->sbSaleCount->setValue(1);
                on_sbSaleCount_valueChanged(1);
            }
        }
    }
}

void DialogSale::on_sbSaleCount_valueChanged(int arg1)
{
    if(product_){
        ui->dsbSalePrice->setValue(product_->sale_price*arg1);
    }
}

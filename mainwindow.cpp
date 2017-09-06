#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <memory>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QDir>

#include <DialogCarriage.h>
#include <DialogBusiness.h>
#include <DialogProduct.h>
#include <DialogSale.h>
#include <DialogUser.h>
#include <DialogSelectBusiness.h>
#include <DialogSelectProduct.h>

#include "DataManager.h"



template <class Mutex>
class log_sink: public spdlog::sinks::base_sink<Mutex>
{
    using MyType = log_sink<Mutex>;
public:
    log_sink()
    {}
    static std::shared_ptr<MyType> instance()
    {
        static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
        return instance;
    }

    void _sink_it(const spdlog::details::log_msg& msg) override
    {
        qDebug()<< QString::fromStdString(std::string(msg.formatted.data(),msg.formatted.size()));
        //flush();
    }

    void flush() override
    {
        //fflush(stdout);
    }
};

typedef log_sink<spdlog::details::null_mutex> log_sink_st;
typedef log_sink<std::mutex> log_sink_mt;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //logger_ = spdlog::details::registry::instance().create("logger", log_sink<spdlog::details::null_mutex>::instance());

    QDir dir;
    dir.mkdir(QApplication::applicationDirPath()+"/log");
    dir.mkdir(QApplication::applicationDirPath()+"/backup");

    logger_ = spdlog::daily_logger_st("logger",(QApplication::applicationDirPath()+"/log/data_log").toStdString(),12);

    ui->dteSaleBeginDate->setDate(QDate::currentDate());
    ui->dteSaleEndDate->setDate(QDate::currentDate());
    ui->dteCarriageBeginDate->setDate(QDate::currentDate());
    ui->dteCarriageEndDate->setDate(QDate::currentDate());

    ui->tabMain->setCurrentIndex(0);

    ui->tblCarriageView->setModel(&carriage_model_);
    ui->tblBusinessView->setModel(&business_model_);
    ui->tblProductView->setModel(&product_model_);
    ui->tblSaleView->setModel(&sale_model_);

    ui->tblCarriageView->setSortingEnabled(true);
    ui->tblBusinessView->setSortingEnabled(true);
    ui->tblProductView->setSortingEnabled(true);
    ui->tblSaleView->setSortingEnabled(true);

    ui->tabMain->removeTab(5);
}

MainWindow::~MainWindow()
{
    DataManager::BackupDatabase();
    delete ui;
}

void MainWindow::InitData()
{
    ShowAllCategory();
    UpdateCategory();
}

void MainWindow::UpdateCategory()
{
    ui->cmbProductCategory->clear();
    const auto& categories = g_data_manager.GetCategories();
    for(const auto& category_pair : categories){
        ui->cmbProductCategory->addItem(category_pair.second->name,category_pair.second->id);
    }
}

//-----------------------------------------------------------------------
//销售管理
//-----------------------------------------------------------------------
//添加销售项
void MainWindow::on_btnSaleAdd_clicked()
{
    DialogSale sale_dialog;
    if(sale_dialog.exec() != QDialog::Accepted){
        return;
    }
    SalePointer sale = sale_dialog.GetSale();
    if(!g_data_manager.AddSale(sale)){
        QMessageBox::warning(this,u8"错误",u8"添加销售项失败");
        return;
    }
    if(!g_data_manager.ModifyProductStockCount(sale->product->id,sale->product->stock_count - sale->count)){
        QMessageBox::warning(this,u8"错误",u8"修改商品库存数量失败"); //出现了中间状态
    }
    QMessageBox::information(this,u8"提示",u8"添加销售项成功");
}
//查询销售项
void MainWindow::on_btnSaleQuery_clicked()
{
    const auto& all_sales = g_data_manager.GetSales();
    std::vector<SalePointer> sales;
    for(auto& sale_pair : all_sales){
        auto& sale = sale_pair.second;
        //商品
        if(ui->gpbSaleProduct->isChecked() && sale_product_){
            if(sale->product_id != sale_product_->id){
                continue;
            }
        }
        //商家
        if(ui->gpbSaleBusiness->isChecked()){
            if(sale->product && sale->product->business && sale_business_){
                if(sale->product->business->id != sale_business_->id){
                    continue;
                }
            }
        }
        //数量
        if(ui->gpbSaleCount->isChecked()){
            if(sale->count != ui->sbSaleCount->value()){
                continue;
            }
        }
        //日期
        if(ui->gpbSaleDate->isChecked()){
            if(sale->date < ui->dteSaleBeginDate->date()){
                continue;
            }
            if(sale->date > ui->dteSaleEndDate->date()){
                continue;
            }
        }
        //价格
        if(ui->gpbSalePrice->isChecked()){
            if(sale->total_price < ui->dsbSaleMinPrice->value()){
                continue;
            }
            if(sale->total_price > ui->dsbSaleMaxPrice->value()){
                continue;
            }
        }
        sales.push_back(sale);
    }
    sale_model_.SetSales(sales);
}
//修改销售项
void MainWindow::on_tblSaleView_doubleClicked(const QModelIndex &index)
{
    DialogSale sale_dialog;
    auto old_sale = sale_model_.GetSale(index);
    int old_sale_count = old_sale->count;
    sale_dialog.SetSale(old_sale);
    if(sale_dialog.exec() != QDialog::Accepted){
        return;
    }
    auto new_sale = sale_dialog.GetSale();
    int new_sale_count = new_sale->count;
    if(!g_data_manager.ModifySale(new_sale)){
        QMessageBox::warning(this,u8"错误",u8"修改销售项失败");
        return;
    }
    if(new_sale_count > old_sale_count){
        if(!g_data_manager.ModifyProductStockCount(old_sale->product->id,old_sale->product->stock_count - (new_sale_count - old_sale_count))){
            QMessageBox::warning(this,u8"错误",u8"修改商品库存数量失败");//中间状态
        }
    }else if(new_sale_count < old_sale_count){
        if(!g_data_manager.ModifyProductStockCount(old_sale->product->id,old_sale->product->stock_count + (old_sale_count - new_sale_count))){
            QMessageBox::warning(this,u8"错误",u8"修改商品库存数量失败");//中间状态
        }
    }
    sale_model_.RefreshItem(index,new_sale);
    ui->tblSaleView->update();
    QMessageBox::information(this,u8"提示",u8"修改销售项成功");
}
//删除销售项
void MainWindow::on_btnSaleDelete_clicked()
{
    auto index = ui->tblSaleView->currentIndex();
    if(index.isValid()){
        if(QMessageBox::question(this,u8"删除",u8"是否删除当前销售项？") == QMessageBox::Yes){
            auto sale = sale_model_.GetSale(index);
            int product_id = sale->product->id;
            int new_stock_count = sale->count + sale->product->stock_count;
            if(!g_data_manager.DeleteSale(sale->id)){
                QMessageBox::warning(this,u8"错误",u8"删除销售项失败");
            }else{
                if(!g_data_manager.ModifyProductStockCount(product_id,new_stock_count)){
                    QMessageBox::warning(this,u8"错误",u8"修改商品库存数量失败");//中间状态
                }
                sale_model_.DeleteItem(index);
                QMessageBox::information(this,u8"提示",u8"删除销售项成功");
            }
        }
    }
}
//计算销售额
void MainWindow::on_btnSaleCalcAllPrice_clicked()
{
    std::pair<double,double> result = sale_model_.GetTotalSales();
    QString text = QString(u8"成本为 : %1 元,  销售额为 : %2 元, 利润为 : %3 元").arg(result.first).arg(result.second).arg(result.second-result.first);
    QMessageBox::information(this,u8"销售统计",text);
}
void MainWindow::on_btnSaleSelectBusiness_clicked()
{
    DialogSelectBusiness dialog_select_business;
    if(dialog_select_business.exec() == QDialog::Accepted){
        auto business = dialog_select_business.GetBusiness();
        if(business){
            sale_business_ = business;
            ui->edtSaleBusiness->setText(business->name);
        }
    }
}
void MainWindow::on_btnSaleSelectProduct_clicked()
{
    DialogSelectProduct dialog_select_product;
    if(dialog_select_product.exec() == QDialog::Accepted){
        auto product = dialog_select_product.GetProduct();
        if(product){
            sale_product_ = product;
            ui->edtSaleProduct->setText(product->name);
        }
    }
}
//-----------------------------------------------------------------------
//商品管理
//-----------------------------------------------------------------------
//添加商品
void MainWindow::on_btnProductAdd_clicked()
{
    DialogProduct product_dialog;
    if(product_dialog.exec() != QDialog::Accepted){
        return;
    }
    if(!g_data_manager.AddProduct(product_dialog.GetProduct())){
        QMessageBox::warning(this,u8"错误",u8"添加商品失败");
        return;
    }
    QMessageBox::information(this,u8"提示",u8"添加商品成功");
}
//查询商品
void MainWindow::on_btnProductQuery_clicked()
{
    const auto& all_products = g_data_manager.GetProducts();
    std::vector<ProductPointer> products;
    for(auto& product_pair : all_products){
        auto& product = product_pair.second;
        //商品名
        if(!ui->edtProductName->text().isEmpty()){
            if(!product->name.contains(ui->edtProductName->text())){
                continue;
            }
        }
        //类别
        if(ui->gpbProductCategory->isChecked() && ui->cmbProductCategory->count() != 0){
            int id = product->category_id;
            if(product->category){
                id = product->category->id;
            }
            if(id != ui->cmbProductCategory->currentData(Qt::UserRole).toInt()){
                continue;
            }
        }
        //库存数量
        if(ui->gpbProductStockCount->isChecked()){
            if(product->stock_count < ui->sbProductStockMinCount->value()){
                continue;
            }
            if(product->stock_count > ui->sbProductStockMaxCount->value()){
                continue;
            }
        }
        //商家
        if(ui->gpbProductBusiness->isChecked() && product_business_){
            int id = product->business_id;
            if(product->business){
                id = product->business->id;
            }
            if(product->business_id != product_business_->id){
                continue;
            }
        }
        //进货价
        if(ui->gpbProductCostPrice->isChecked()){
            if(product->cost_price < ui->dsbProductCostMinPrice->value()){
                continue;
            }
            if(product->cost_price > ui->dsbProductCostMaxPrice->value()){
                continue;
            }
        }
        //销售价
        if(ui->gpbProductSalePrice->isChecked()){
            if(product->sale_price < ui->dsbProductSaleMinPrice->value()){
                continue;
            }
            if(product->sale_price > ui->dsbProductSaleMaxPrice->value()){
                continue;
            }
        }
        products.push_back(product);
    }
    product_model_.SetProducts(products);
}
//修改商品
void MainWindow::on_tblProductView_doubleClicked(const QModelIndex &index)
{
    DialogProduct product_dialog;
    product_dialog.SetProduct(product_model_.GetProduct(index));
    if(product_dialog.exec() != QDialog::Accepted){
        return;
    }
    auto new_product = product_dialog.GetProduct();
    if(!g_data_manager.ModifyProduct(new_product)){
        QMessageBox::warning(this,u8"错误",u8"修改商品信息失败");
        return;
    }
    product_model_.RefreshItem(index,new_product);
    ui->tblProductView->update();
    QMessageBox::information(this,u8"提示",u8"修改商品信息成功");
}
//删除商品
void MainWindow::on_btnProductDelete_clicked()
{
    auto index = ui->tblProductView->currentIndex();
    if(index.isValid()){
        if(QMessageBox::question(this,u8"删除",u8"是否删除当前商品？") == QMessageBox::Yes){
            auto product = product_model_.GetProduct(index);
            bool is_data_related = false;
            if(!g_data_manager.DeleteProduct(product->id,is_data_related)){
                if(is_data_related){
                    QMessageBox::warning(this,u8"错误",u8"不能删除商品,已被使用");
                }else{
                    QMessageBox::warning(this,u8"错误",u8"删除商品失败");
                }
            }else{
                product_model_.DeleteItem(index);
                QMessageBox::warning(this,u8"提示",u8"删除商品成功");
            }
        }
    }
}
void MainWindow::on_btnProductSelectBusiness_clicked()
{
    DialogSelectBusiness dialog_select_business;
    if(dialog_select_business.exec() == QDialog::Accepted){
        auto business = dialog_select_business.GetBusiness();
        if(business){
            product_business_ = business;
            ui->edtProductBusiness->setText(business->name);
        }
    }
}
void MainWindow::on_btnProductCalcCost_clicked()
{
    std::pair<double,double> result = product_model_.GetTotalCost();
    QString text = QString(u8"成本为 : %1 元,  预计销售额为 : %2 元, 可盈利为 : %3 元").arg(result.first).arg(result.second).arg(result.second-result.first);
    QMessageBox::information(this,u8"商品盈利统计",text);
}
//-----------------------------------------------------------------------
//商家管理
//-----------------------------------------------------------------------
//添加商家
void MainWindow::on_btnBusinessAdd_clicked()
{
    DialogBusiness bussiness_dialog;
    if(bussiness_dialog.exec() != QDialog::Accepted){
        return;
    }
    if(!g_data_manager.AddBusiness(bussiness_dialog.GetBusiness())){
        QMessageBox::warning(this,u8"错误",u8"添加商家信息失败");
        return;
    }
    QMessageBox::information(this,u8"提示",u8"添加商家信息成功");
}
//查询商家
void MainWindow::on_btnBusinessQuery_clicked()
{
    const auto& all_businesses = g_data_manager.GetBusinesses();
    std::vector<BusinessPointer> businesses;
    for(auto& business_pair : all_businesses){
        auto& business = business_pair.second;
        if(!ui->edtBusinessName->text().isEmpty()){
            if(!business->name.contains(ui->edtBusinessName->text())){
                continue;
            }
        }
        if(!ui->edtBusinessPhone->text().isEmpty()){
            if(!business->phone.contains(ui->edtBusinessPhone->text())){
                continue;
            }
        }
        if(!ui->edtBussinessAddress->text().isEmpty()){
            if(!business->name.contains(ui->edtBussinessAddress->text())){
                continue;
            }
        }
        businesses.push_back(business);
    }
    business_model_.SetBusinesses(businesses);
}
//修改商家
void MainWindow::on_tblBusinessView_doubleClicked(const QModelIndex &index)
{
    DialogBusiness bussiness_dialog;
    bussiness_dialog.SetBusiness(business_model_.GetBusiness(index));
    if(bussiness_dialog.exec() != QDialog::Accepted){
        return;
    }
    auto new_business = bussiness_dialog.GetBusiness();
    if(!g_data_manager.ModifyBusiness(new_business)){
        QMessageBox::warning(this,u8"错误",u8"修改商家信息失败");
        return;
    }
    business_model_.RefreshItem(index,new_business);
    ui->tblBusinessView->update();
    QMessageBox::information(this,u8"提示",u8"修改商家信息成功");
}
//删除商家
void MainWindow::on_btnBusinessDelete_clicked()
{
    auto index = ui->tblBusinessView->currentIndex();
    if(index.isValid()){
        if(QMessageBox::question(this,u8"删除",u8"是否删除当前商家信息？") == QMessageBox::Yes){
            auto business = business_model_.GetBusiness(index);
            bool is_data_related = false;
            if(!g_data_manager.DeleteBusiness(business->id,is_data_related)){
                if(is_data_related){
                    QMessageBox::warning(this,u8"错误",u8"不能删除商家信息,已被使用");
                }else{
                    QMessageBox::warning(this,u8"错误",u8"删除商家信息失败");
                }
            }else{
                business_model_.DeleteItem(index);
                QMessageBox::warning(this,u8"提示",u8"删除商家信息成功");
            }
        }
    }
}
//-----------------------------------------------------------------------
//运费管理
//-----------------------------------------------------------------------
//添加运费
void MainWindow::on_btnCarriageAdd_clicked()
{
    DialogCarriage carriage_dialog;
    if(carriage_dialog.exec() != QDialog::Accepted){
        return;
    }
    if(!g_data_manager.AddCarriage(carriage_dialog.GetCarriage())){
        QMessageBox::warning(this,u8"错误",u8"添加运费失败");
        return;
    }
    QMessageBox::information(this,u8"提示",u8"添加运费成功");
}
//查询运费
void MainWindow::on_btnCarriageQuery_clicked()
{
    const auto& all_carriages = g_data_manager.GetCarriages();
    std::vector<CarriagePointer> carriages;
    for(auto& carriage_pair : all_carriages){
        auto& carriage = carriage_pair.second;
        if(ui->gpbCarriageDate->isChecked()){
            if(carriage->date < ui->dteCarriageBeginDate->date()){
                continue;
            }
            if(carriage->date > ui->dteCarriageEndDate->date()){
                continue;
            }
        }
        if(ui->gpbCarriagePrice->isChecked()){
            if(carriage->purchase_cost + carriage->carriage_cost < ui->dsbCarriageMinPrice->value()){
                continue;
            }
            if(carriage->purchase_cost + carriage->carriage_cost > ui->dsbCarriageMaxPrice->value()){
                continue;
            }
        }
        carriages.push_back(carriage);
    }
    carriage_model_.SetCarriages(carriages);
}
//修改运费
void MainWindow::on_tblCarriageView_doubleClicked(const QModelIndex &index)
{
    DialogCarriage carriage_dialog;
    carriage_dialog.SetCarriage(carriage_model_.GetCarriage(index));
    if(carriage_dialog.exec() != QDialog::Accepted){
        return;
    }
    auto new_carriage = carriage_dialog.GetCarriage();
    if(!g_data_manager.ModifyCarriage(new_carriage)){
        QMessageBox::warning(this,u8"错误",u8"修改运费失败");
        return;
    }
    carriage_model_.RefreshItem(index,new_carriage);
    ui->tblCarriageView->update();
    QMessageBox::information(this,u8"提示",u8"修改运费成功");
}
//删除运费
void MainWindow::on_btnCarriageDelete_clicked()
{
    auto index = ui->tblCarriageView->currentIndex();
    if(index.isValid()){
        if(QMessageBox::question(this,u8"删除",u8"是否删除当前运费？") == QMessageBox::Yes){
            auto carriage = carriage_model_.GetCarriage(index);
            if(!g_data_manager.DeleteCarriage(carriage->id)){
                QMessageBox::warning(this,u8"错误",u8"删除运费失败");
            }else{
                carriage_model_.DeleteItem(index);
                QMessageBox::information(this,u8"提示",u8"删除运费成功");
            }
        }
    }
}
//计算总运费
void MainWindow::on_btnCarriageCalcSales_clicked()
{
    std::pair<double,double> result = carriage_model_.GetTotalCost();
    QString text = QString(u8"总进货费用为 : %1 元,  总运费为 : %2 元, 总费用为 : %3 元").arg(result.first).arg(result.second).arg(result.second+result.first);
    QMessageBox::information(this,u8"费用统计",text);
}
//-----------------------------------------------------------------------
//类别
//-----------------------------------------------------------------------
//添加类别
void MainWindow::on_btnCategoryAdd_clicked()
{
    bool ok = false;
    QString category_name = QInputDialog::getText(this,u8"输入类别",u8"类别名：",QLineEdit::Normal,QString(),&ok);
    if(!ok){
        return;
    }
    if(category_name.isEmpty()){
        QMessageBox::warning(this,u8"错误",u8"类别为空");
        return;
    }
    CategoryPointer category = std::make_shared<Category>();
    category->name = category_name;
    if(!g_data_manager.AddCategory(category)){
        QMessageBox::warning(this,u8"错误",u8"添加类别失败");
        return;
    }
    ShowAllCategory();
    UpdateCategory();
}
//修改类别
void MainWindow::on_lwCategory_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem* item = ui->lwCategory->item(index.row());
    auto& categories = g_data_manager.GetCategories();
    auto iter = categories.find(item->data(Qt::UserRole).toInt());
    const CategoryPointer& category = iter->second;

    bool ok = false;
    QString category_name = QInputDialog::getText(this,u8"输入类别",u8"类别名：",QLineEdit::Normal,category->name,&ok);
    if(!ok){
        return;
    }
    if(category_name.isEmpty()){
        QMessageBox::warning(this,u8"错误",u8"类别为空");
        return;
    }
    CategoryPointer new_category = std::make_shared<Category>();
    new_category->id = category->id;
    new_category->name = category_name;
    if(!g_data_manager.ModifyCategory(new_category)){
        QMessageBox::warning(this,u8"错误",u8"添加类别失败");
        return;
    }
    ShowAllCategory();
    UpdateCategory();
}
//删除类别
void MainWindow::on_btnCategoryDelete_clicked()
{
    auto item = ui->lwCategory->currentItem();
    if(item){
        if(QMessageBox::question(this,u8"删除",u8"是否删除当前类别？") == QMessageBox::Yes){
            bool is_data_related = false;
            if(!g_data_manager.DeleteCategory(item->data(Qt::UserRole).toInt(),is_data_related)){
                if(is_data_related){
                    QMessageBox::warning(this,u8"错误",u8"不能删除此类别,已被使用");
                }else{
                    QMessageBox::warning(this,u8"错误",u8"删除类别失败");
                }
            }else{
                ShowAllCategory();
                UpdateCategory();
                QMessageBox::information(this,u8"提示",u8"删除类别成功");
            }
        }
    }
}
void MainWindow::ShowAllCategory()
{
    ui->lwCategory->clear();
    const auto& categories = g_data_manager.GetCategories();
    for(const auto& category_pair : categories){
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(category_pair.second->name);
        item->setData(Qt::UserRole,category_pair.second->id);
        ui->lwCategory->addItem(item);
    }
}

//-----------------------------------------------------------------------
//删除
//-----------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *keyevent)
{
    int uKey = keyevent->key();
    Qt::Key key = static_cast<Qt::Key>(uKey);
    if(key == Qt::Key_Delete)
    {
        //运费
        if(ui->tblCarriageView->hasFocus()){
            on_btnCarriageDelete_clicked();
        }
        //商家
        if(ui->tblBusinessView->hasFocus()){
            on_btnBusinessDelete_clicked();
        }
        //商品
        if(ui->tblProductView->hasFocus()){
            on_btnProductDelete_clicked();
        }
        //销售
        if(ui->tblSaleView->hasFocus()){
            on_btnSaleDelete_clicked();
        }
        //类别
        if(ui->lwCategory->hasFocus()){
            on_btnCategoryDelete_clicked();
        }
    }

//    if(key == Qt::Key_Control || key == Qt::Key_Shift || key == Qt::Key_Alt )
//    {
//        return ;
//    }
//    Qt::KeyboardModifiers modifiers = keyevent->modifiers();
//    if(modifiers & Qt::ShiftModifier)
//        uKey += Qt::SHIFT;
//    if(modifiers & Qt::ControlModifier)
//        uKey += Qt::CTRL;
//    if(modifiers & Qt::AltModifier)
//        uKey += Qt::ALT;
//    QKeySequence key_sequence(uKey);
}

void MainWindow::on_actionBackupDatabase_triggered()
{
    if(DataManager::BackupDatabase()){
        QMessageBox::information(this,u8"信息",u8"备份数据库成功");
    }else{
        QMessageBox::warning(this,u8"错误",u8"备份数据库失败");
    }
}

void MainWindow::on_btnUserAdd_clicked()
{
    DialogUser user_dialog;
    if(user_dialog.exec() != QDialog::Accepted){
        return;
    }
//    if(!g_data_manager.AddCarriage(carriage_dialog.GetCarriage())){
//        QMessageBox::warning(this,u8"错误",u8"添加运费失败");
//        return;
//    }
    QMessageBox::information(this,u8"提示",u8"添加运费成功");
}

void MainWindow::on_btnUserDelete_clicked()
{

}

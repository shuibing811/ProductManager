#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <vector>

#include <QMainWindow>
#include <QKeyEvent>
#include "spdlog/spdlog.h"

#include "ModelCarriage.h"
#include "ModelBusiness.h"
#include "ModelProduct.h"
#include "ModelSale.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitData();

    std::shared_ptr<spdlog::logger> GetLogger(){
        return logger_;
    }
protected:
    void keyPressEvent(QKeyEvent *keyevent);
private slots:
    void on_btnSaleAdd_clicked();
    void on_btnSaleQuery_clicked();

    void on_btnProductAdd_clicked();
    void on_btnProductQuery_clicked();

    void on_btnBusinessAdd_clicked();
    void on_btnBusinessQuery_clicked();

    void on_btnCarriageAdd_clicked();
    void on_btnCarriageQuery_clicked();
    void on_btnCarriageCalcSales_clicked();

    void on_tblCarriageView_doubleClicked(const QModelIndex &index);

    void on_tblBusinessView_doubleClicked(const QModelIndex &index);

    void on_tblProductView_doubleClicked(const QModelIndex &index);

    void on_tblSaleView_doubleClicked(const QModelIndex &index);

    void on_btnSaleSelectBusiness_clicked();

    void on_btnSaleSelectProduct_clicked();

    void on_lwCategory_doubleClicked(const QModelIndex &index);

    void on_btnCategoryAdd_clicked();
    void on_btnProductSelectBusiness_clicked();

    void on_btnSaleCalcAllPrice_clicked();

    void on_btnSaleDelete_clicked();

    void on_btnProductDelete_clicked();

    void on_btnBusinessDelete_clicked();

    void on_btnCarriageDelete_clicked();

    void on_btnCategoryDelete_clicked();

    void on_actionBackupDatabase_triggered();

    void on_btnUserAdd_clicked();

    void on_btnUserDelete_clicked();

    void on_btnProductCalcCost_clicked();

private:
    void ShowAllCategory();
    void UpdateCategory();
private:
    Ui::MainWindow *ui;
    std::shared_ptr<spdlog::logger> logger_;


    ModelCarriage carriage_model_;
    ModelBusiness business_model_;
    ModelProduct  product_model_;
    ModelSale     sale_model_;

    //销售
    ProductPointer sale_product_;
    BusinessPointer sale_business_;
    //商品
    BusinessPointer product_business_;

};

#endif // MAINWINDOW_H

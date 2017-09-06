#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "commondefine.h"

#include <unordered_map>
#include <QSqlDatabase>

#include "spdlog/spdlog.h"

class DataManager
{
public:
    using CategoryMap = std::unordered_map<int,CategoryPointer>;
    using BusinessMap = std::unordered_map<int,BusinessPointer>;
    using ProductMap = std::unordered_map<int,ProductPointer>;
    using SaleMap = std::unordered_map<int,SalePointer>;
    using CarriageMap = std::unordered_map<int,CarriagePointer>;
public:
    DataManager();

    bool Init();
    void Uninit();

    static bool BackupDatabase();

    void SetLogger(std::shared_ptr<spdlog::logger> logger){
        logger_ = logger;
    }

    bool  AddCategory(CategoryPointer& category);
    bool  ModifyCategory(const CategoryPointer& category);
    bool  DeleteCategory(int id,bool& is_data_related);

    bool  AddCarriage(CarriagePointer& carriage);
    bool  ModifyCarriage(CarriagePointer& carriage);
    bool  DeleteCarriage(int id);

    bool  AddBusiness(BusinessPointer& business);
    bool  ModifyBusiness(BusinessPointer& business);
    bool  DeleteBusiness(int id,bool& is_data_related);

    bool  AddProduct(ProductPointer& product);
    bool  ModifyProduct(ProductPointer& product);
    bool  ModifyProductStockCount(int id,int count);
    bool  DeleteProduct(int id,bool& is_data_related);

    bool  AddSale(SalePointer& sale);
    bool  ModifySale(SalePointer& sale);
    bool  DeleteSale(int id);


    bool  HasUser(QString name);
    bool  ModifyUserPassword(QString name,QString password);
    bool  AddUser(User& user);
    bool  ModifyUser(User& user);
    bool  DeleteUser(QString name);
    bool CheckUserLogin(QString name,QString password,bool& is_name_or_password_error);

    //get

    const CategoryMap& GetCategories(){
        return categroy_map_;
    }

    const BusinessMap& GetBusinesses(){
        return business_map_;
    }

    const ProductMap&  GetProducts(){
        return product_map_;
    }

    const SaleMap&     GetSales(){
        return sale_map_;
    }

    const CarriageMap& GetCarriages(){
        return carriage_map_;
    }
private:
    bool CheckDB(QSqlDatabase& db);
    bool LoadData(QSqlDatabase& db);
private:
    enum Permission{
        PermissionSale = 0x01,
        PermissionProduct = 0x02,
        PermissionBusiness = 0x04,
        PermissionCarriage = 0x08,
        PermissionCategory = 0x10,
        PermissionAll = PermissionSale|PermissionProduct|PermissionBusiness|PermissionCarriage|PermissionCategory,
    };

private:
    CategoryMap     categroy_map_;
    BusinessMap     business_map_;
    ProductMap      product_map_;
    SaleMap         sale_map_;
    CarriageMap     carriage_map_;

    std::shared_ptr<spdlog::logger> logger_;
    QSqlDatabase    db_;
};
extern DataManager g_data_manager;

#endif // DATAMANAGER_H

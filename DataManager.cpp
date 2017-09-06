#include "DataManager.h"

#include <QSqlQuery>
#include <QVariant>
#include <QFile>
#include <QDebug>
#include <QApplication>

DataManager g_data_manager;

DataManager::DataManager()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
bool DataManager::CheckDB(QSqlDatabase& db)
{
    QSqlQuery query(db);
    bool bsuccess = false;
    //创建运费表
    bsuccess = query.exec("create table if not exists carriage ("
                                   "   id integer primary key autoincrement not null,"
                                   "   date text,"
                                   "   carriage_cost real,"
                                   "   purchase_cost real,"
                                   "   remark text"
                                   ");");
    if(!bsuccess){
        logger_->error("create carriage table is error");
        return false;
    }
    //创建类别表
    bsuccess = query.exec("create table if not exists category ("
                                   "   id integer primary key autoincrement not null,"
                                   "   name text"
                                   ");");
    if(!bsuccess){
        logger_->error("create category table is error");
        return false;
    }
    //创建商家表
    bsuccess = query.exec("create table if not exists business ("
                               "   id integer primary key autoincrement not null,"
                               "   name text,"
                               "   phone text,"
                               "   address text,"
                               "   remark text"
                               ");");
    if(!bsuccess){
        logger_->error("create business table is error");
        return false;
    }
    //创建产品表
    bsuccess = query.exec("create table if not exists product ("
                                   "   id integer primary key autoincrement not null,"
                                   "   name text,"
                                   "   cost_price real,"
                                   "   sale_price real,"
                                   "   stock_count integer,"
                                   "   remark text,"
                                   "   category_id integer,"
                                   "   business_id integer"
                                   ");");
    if(!bsuccess){
        logger_->error("create product table is error");
        return false;
    }
    //创建销售表
    bsuccess = query.exec("create table if not exists sale ("
                                   "   id integer primary key autoincrement not null,"
                                   "   total_price real,"
                                   "   count integer,"
                                   "   date text,"
                                   "   remark text,"
                                   "   product_name text,"
                                   "   product_id integer"
                                   ");");
    if(!bsuccess){
        logger_->error("create sale table is error");
        return false;
    }

    //创建用户表
    bsuccess = query.exec("create table if not exists user ("
                                   "   name text primary key not null,"
                                   "   password text,"
                                   "   create_datetime text,"
                                   "   permission integer,"
                                   "   remark text"
                                   ");");
    if(!bsuccess){
        logger_->error("create user table is error");
        return false;
    }

    bsuccess = query.exec("SELECT * FROM user WHERE name='admin'");
    if(!bsuccess){
        logger_->error("select admin user is error");
        return false;
    }
    int query_count = 0;
    while (query.next()) {
        query_count++;
    }
    if(query_count==0){
        query.prepare("INSERT INTO user (name,password,create_datetime,permission,remark) "
                      "VALUES ('admin','admin',:create_datetime,:permission,'init admin user')");
        query.bindValue(":create_datetime", QDateTime::currentDateTime().toString());
        query.bindValue(":permission", PermissionAll);
        bsuccess = query.exec();

        if(!bsuccess){
            logger_->error("insert admin user is error");
            return false;
        }
    }
    return true;
}
bool DataManager::LoadData(QSqlDatabase &db)
{
    QSqlQuery query(db);
    bool bsuccess = false;
    //运费表
    bsuccess = query.exec("SELECT * FROM carriage");
    if(!bsuccess){
        logger_->error("SELECT carriage table is error");
        return false;
    }
    while (query.next()) {
        auto carriage = std::make_shared<Carriage>();
        int index = 0;
        carriage->id = query.value(index++).toInt();
        carriage->date = query.value(index++).toDate();
        carriage->carriage_cost = query.value(index++).toDouble();
        carriage->purchase_cost = query.value(index++).toDouble();
        carriage->remark = query.value(index++).toString();
        carriage_map_[carriage->id] = carriage;
    }
    //类别表
    bsuccess = query.exec("SELECT * FROM category");
    if(!bsuccess){
        logger_->error("SELECT category table is error");
        return false;
    }
    while (query.next()) {
        auto category = std::make_shared<Category>();
        int index = 0;
        category->id = query.value(index++).toInt();
        category->name = query.value(index++).toString();
        categroy_map_[category->id] = category;
    }
    //商家表
    bsuccess = query.exec("SELECT * FROM business");
    if(!bsuccess){
        logger_->error("SELECT business table is error");
        return false;
    }
    while (query.next()) {
        auto business = std::make_shared<Business>();
        int index = 0;
        business->id = query.value(index++).toInt();
        business->name = query.value(index++).toString();
        business->phone = query.value(index++).toString();
        business->address = query.value(index++).toString();
        business->remark = query.value(index++).toString();
        business_map_[business->id] = business;
    }
    //产品表
    bsuccess = query.exec("SELECT * FROM product");
    if(!bsuccess){
        logger_->error("SELECT product table is error");
        return false;
    }
    while (query.next()) {
        auto product = std::make_shared<Product>();
        int index = 0;
        product->id = query.value(index++).toInt();
        product->name = query.value(index++).toString();
        product->cost_price = query.value(index++).toDouble();
        product->sale_price = query.value(index++).toDouble();
        product->stock_count = query.value(index++).toInt();
        product->remark = query.value(index++).toString();
        product->category_id = query.value(index++).toInt();
        product->business_id = query.value(index++).toInt();
        product->category = categroy_map_[product->category_id];
        product->business = business_map_[product->business_id];
        product_map_[product->id] = product;
    }
    //销售表
    bsuccess = query.exec("SELECT * FROM sale");
    if(!bsuccess){
        logger_->error("SELECT sale table is error");
        return false;
    }
    while (query.next()) {
        auto sale = std::make_shared<Sale>();
        int index = 0;
        sale->id = query.value(index++).toInt();
        sale->total_price = query.value(index++).toDouble();
        sale->count = query.value(index++).toInt();
        sale->date = query.value(index++).toDate();
        sale->remark = query.value(index++).toString();
        sale->product_name = query.value(index++).toString();
        sale->product_id = query.value(index++).toInt();
        sale->product = product_map_[sale->product_id];
        sale_map_[sale->id] = sale;
    }
    return true;
}
bool DataManager::Init()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(QCoreApplication::applicationDirPath()+"/product.db");     //创建数据库
    if(!db_.open()){
        logger_->error("open database is error");
        return false;
    }

    if(!CheckDB(db_)){
        return false;
    }
    if(!LoadData(db_)){
        return false;

    }

    return true;
}

void DataManager::Uninit()
{
    if(db_.isOpen()){
        db_.close();
    }
}

bool DataManager::BackupDatabase()
{
    QString date_time_string = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
    QFile file(QCoreApplication::applicationDirPath()+"/product.db");
    return file.copy(QCoreApplication::applicationDirPath()+"/backup/"+date_time_string+".db");
}
//-----------------------------------------------------------------------
//类别
//-----------------------------------------------------------------------
bool  DataManager::AddCategory(CategoryPointer& category)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO category (name) "
                            "VALUES (:name)");
    query.bindValue(":name", category->name);
    if(!query.exec()){
        logger_->error("INSERT category is error");
        return false;
    }
    category->id = query.lastInsertId().toInt();
    categroy_map_[category->id] = category;

    logger_->info("add category : generate id = {} , name = {}",category->id,category->name.toStdString());

    return true;
}

bool  DataManager::ModifyCategory(const CategoryPointer& category)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE category SET name = ? WHERE id = ?");
    query.bindValue(0, category->name);
    query.bindValue(1, category->id);
    if(!query.exec()){
        logger_->error("UPDATE category is error");
        return false;
    }
    categroy_map_[category->id]->name = category->name;

    logger_->info("modify category : id = {} , name = {}",category->id,category->name.toStdString());

    return true;
}

bool  DataManager::DeleteCategory(int id,bool& is_data_related)
{
    //检查是否可以删除
    auto iter = std::find_if(product_map_.begin(),product_map_.end(),[id](const auto& product_pair)->bool{
        if(product_pair.second->category){
            return product_pair.second->category->id == id;
        }else{
            return product_pair.second->category_id == id;
        }
    });
    if(iter != product_map_.end()){
        is_data_related = true;
        return false;
    }
    is_data_related = false;

    QSqlQuery query(db_);
    query.prepare("DELETE FROM category WHERE id = ?");
    query.bindValue(0, id);
    if(!query.exec()){
        logger_->error("DELETE category is error");
        return false;
    }

    logger_->info("delete category : id = {} , name = {}",id,categroy_map_[id]->name.toStdString());
    categroy_map_.erase(id);
    return true;
}
//-----------------------------------------------------------------------
//用户
//-----------------------------------------------------------------------
bool  DataManager::AddUser(User& user)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO user (name,password,create_datetime,permission,remark) "
                  "VALUES (:name,:password,:create_datetime,:permission,:remark)");
    query.bindValue(":name", user.name);
    query.bindValue(":password", user.password);
    query.bindValue(":create_datetime", user.create_time.toString());
    query.bindValue(":permission", user.permission);
    query.bindValue(":remark", user.remark);
    if(!query.exec()){
        logger_->error("INSERT user is error");
        return false;
    }
    logger_->info("add user : name = {} , create_datetime = {} , permission = {} , remark = {}",
                  user.name.toStdString(),user.create_time.toString().toStdString(),user.permission,user.remark.toStdString());

    return true;
}

bool  DataManager::ModifyUser(User& user)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE user SET password = ?,permission=?,remark=? WHERE name = ?");
    int index = 0;
    query.bindValue(index++, user.password);
    query.bindValue(index++, user.permission);
    query.bindValue(index++, user.remark);
    query.bindValue(index++, user.name);
    if(!query.exec()){
        logger_->error("UPDATE user is error");
        return false;
    }
    logger_->info("update user : name = {} , permission = {} , remark = {}",
                  user.name.toStdString(),user.permission,user.remark.toStdString());

    return true;
}

bool  DataManager::DeleteUser(QString name)
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM user WHERE name = ?");
    query.bindValue(0, name);
    if(!query.exec()){
        logger_->error("DELETE user is error");
        return false;
    }
    logger_->info("delete user : name = {}",name.toStdString());
    return true;
}

bool  DataManager::HasUser(QString name)
{
    QSqlQuery query(db_);
    query.prepare("SELECT * FROM user WHERE name=?");
    query.bindValue(0, name);
    if(!query.exec()){
        return false;
    }
    int query_count = 0;
    while (query.next()) {
        query_count++;
    }
    if(query_count == 0){
        return false;
    }
    return true;
}

bool  DataManager::ModifyUserPassword(QString name,QString password)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE user SET password = ? WHERE name = ?");
    query.bindValue(0, password);
    query.bindValue(1, name);
    if(!query.exec()){
        logger_->error("UPDATE user password is error");
        return false;
    }
    logger_->info("update user password : name = {}",name.toStdString());

    return true;
}

bool DataManager::CheckUserLogin(QString name,QString password,bool& is_name_or_password_error)
{
    QSqlQuery query(db_);
    query.prepare("SELECT * FROM user WHERE name=? and password=?");
    query.bindValue(0, name);
    query.bindValue(1, password);
    if(!query.exec()){
        return false;
    }
    int query_count = 0;
    while (query.next()) {
        query_count++;
    }
    if(query_count != 1){
        is_name_or_password_error = true;
        return false;
    }
    is_name_or_password_error = false;
    return true;
}
//-----------------------------------------------------------------------
//运费
//-----------------------------------------------------------------------
bool  DataManager::AddCarriage(CarriagePointer& carriage)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO carriage (date,carriage_cost,purchase_cost,remark) "
                            "VALUES (:date,:carriage_cost,:purchase_cost,:remark)");
    query.bindValue(":date", carriage->date);
    query.bindValue(":carriage_cost", carriage->carriage_cost);
    query.bindValue(":purchase_cost", carriage->purchase_cost);
    query.bindValue(":remark", carriage->remark);
    if(!query.exec()){
        logger_->error("INSERT carriage is error");
        return false;
    }
    carriage->id = query.lastInsertId().toInt();
    carriage_map_[carriage->id] = carriage;

    logger_->info("add carriage : id = {} , date = {}, carriage_cost = {}, purchase_cost = {}, remark={}",
                  carriage->id, carriage->date.toString().toStdString(),carriage->carriage_cost,
                  carriage->purchase_cost,carriage->remark.toStdString());
    return true;
}

bool  DataManager::ModifyCarriage(CarriagePointer& carriage)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE carriage SET date = ?,carriage_cost = ?,purchase_cost = ?,remark = ? WHERE id = ?");
    int index = 0;
    query.bindValue(index++, carriage->date);
    query.bindValue(index++, carriage->carriage_cost);
    query.bindValue(index++, carriage->purchase_cost);
    query.bindValue(index++, carriage->remark);
    query.bindValue(index++, carriage->id);
    if(!query.exec()){
        logger_->error("UPDATE carriage is error");
        return false;
    }
    auto& old_carriage = carriage_map_[carriage->id];
    old_carriage->date = carriage->date;
    old_carriage->carriage_cost = carriage->carriage_cost;
    old_carriage->purchase_cost = carriage->purchase_cost;
    old_carriage->remark = carriage->remark;

    logger_->info("modify carriage : id = {} , date = {}, carriage_cost = {}, purchase_cost = {}, remark={}",
                  carriage->id,carriage->date.toString().toStdString(),carriage->carriage_cost,
                  carriage->purchase_cost,carriage->remark.toStdString());

    return true;
}

bool  DataManager::DeleteCarriage(int id)
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM carriage WHERE id = ?");
    query.bindValue(0, id);
    if(!query.exec()){
        logger_->error("DELETE carriage is error");
        return false;
    }

    const auto& carriage = carriage_map_[id];
    logger_->info("delete carriage : id = {} , date = {},carriage_cost = ?,purchase_cost = ?, remark={}",
                  carriage->id,carriage->date.toString().toStdString(),carriage->carriage_cost,
                  carriage->purchase_cost,carriage->remark.toStdString());
    carriage_map_.erase(id);
    return true;
}
//-----------------------------------------------------------------------
//商户
//-----------------------------------------------------------------------
bool  DataManager::AddBusiness(BusinessPointer& business)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO business (name,phone,address,remark) "
                            "VALUES (:name,:phone,:address,:remark)");
    query.bindValue(":name", business->name);
    query.bindValue(":phone", business->phone);
    query.bindValue(":address", business->address);
    query.bindValue(":remark", business->remark);
    if(!query.exec()){
        logger_->error("INSERT business is error");
        return false;
    }
    business->id = query.lastInsertId().toInt();
    business_map_[business->id] = business;

    logger_->info("add business : id = {} , name = {}, phone = {}, address = {}, remark = {}",
                  business->id, business->name.toStdString(),business->phone.toStdString(),
                  business->address.toStdString(),business->remark.toStdString());


    return true;
}

bool  DataManager::ModifyBusiness(BusinessPointer& business)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE business SET name = ?,phone = ?,address = ?,remark=? WHERE id = ?");
    int index = 0;
    query.bindValue(index++, business->name);
    query.bindValue(index++, business->phone);
    query.bindValue(index++, business->address);
    query.bindValue(index++, business->remark);
    query.bindValue(index++, business->id);
    if(!query.exec()){
        logger_->error("UPDATE business is error");
        return false;
    }

    auto& old_business = business_map_[business->id];
    old_business->name = business->name;
    old_business->phone = business->phone;
    old_business->address = business->address;
    old_business->remark = business->remark;

    logger_->info("modify business : id = {} , name = {}, phone = {}, address = {}, remark = {}",
                  business->id, business->name.toStdString(),business->phone.toStdString(),
                  business->address.toStdString(),business->remark.toStdString());

    return true;
}

bool  DataManager::DeleteBusiness(int id,bool& is_data_related)
{
    //检查是否可以删除
    auto iter = std::find_if(product_map_.begin(),product_map_.end(),[id](const auto& product_pair)->bool{
        if(product_pair.second->business){
            return product_pair.second->business->id == id;
        }else{
            return product_pair.second->business_id == id;
        }
    });
    if(iter != product_map_.end()){
        is_data_related = true;
        return false;
    }
    is_data_related = false;

    QSqlQuery query(db_);
    query.prepare("DELETE FROM business WHERE id = ?");
    query.bindValue(0, id);
    if(!query.exec()){
        logger_->error("DELETE business is error");
        return false;
    }
    const auto& business = business_map_[id];
    logger_->info("modify business : id = {} , name = {}, phone = {}, address = {}, remark = {}",
                  business->id, business->name.toStdString(),business->phone.toStdString(),
                  business->address.toStdString(),business->remark.toStdString());

    business_map_.erase(id);
    return true;
}
//-----------------------------------------------------------------------
//商品
//-----------------------------------------------------------------------
bool  DataManager::AddProduct(ProductPointer& product)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO product (name,cost_price,sale_price,stock_count,remark,category_id,business_id) "
                            "VALUES (:name,:cost_price,:sale_price,:stock_count,:remark,:category_id,:business_id)");
    query.bindValue(":name", product->name);
    query.bindValue(":cost_price", product->cost_price);
    query.bindValue(":sale_price", product->sale_price);
    query.bindValue(":stock_count", product->stock_count);
    query.bindValue(":remark", product->remark);
    if(product->category){
        query.bindValue(":category_id", product->category->id);
    }else{
        query.bindValue(":category_id", product->category_id);
    }
    if(product->business){
        query.bindValue(":business_id", product->business->id);
    }else{
        query.bindValue(":business_id", product->business_id);
    }
    if(!query.exec()){
        logger_->error("INSERT product is error");
        return false;
    }
    product->id = query.lastInsertId().toInt();
    product_map_[product->id] = product;

    logger_->info("add product : id = {} , name = {}, cost_price = {}, sale_price = {} , "
                  "stock_count = {} , business = {} , remark = {}",
                  product->id, product->name.toStdString(),product->cost_price,
                  product->sale_price,product->stock_count,product->business?product->business->name.toStdString():"",
                  product->remark.toStdString());

    return true;
}

bool  DataManager::ModifyProductStockCount(int id,int count)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE product SET stock_count = ? WHERE id = ?");
    query.bindValue(0, count);
    query.bindValue(1, id);
    if(!query.exec()){
        logger_->error("UPDATE product stock count is error");
        return false;
    }
    auto& old_product = product_map_[id];
    old_product->stock_count = count;
    logger_->info("modify product stock count : id = {} , name = {} , stock_count = {} , business = {}",
                  old_product->id, old_product->name.toStdString(),old_product->stock_count,
                  old_product->business?old_product->business->name.toStdString():"");
    return true;
}

bool  DataManager::ModifyProduct(ProductPointer& product)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE product SET name = ?,cost_price = ?,sale_price = ?,"
                  "stock_count=?,remark=?,category_id=?,business_id=? WHERE id = ?");
    int index = 0;
    query.bindValue(index++, product->name);
    query.bindValue(index++, product->cost_price);
    query.bindValue(index++, product->sale_price);
    query.bindValue(index++, product->stock_count);
    query.bindValue(index++, product->remark);
    if(product->category){
        query.bindValue(index++, product->category->id);
    }else{
        query.bindValue(index++, product->category_id);
    }
    if(product->business){
        query.bindValue(index++, product->business->id);
    }else{
        query.bindValue(index++, product->business_id);
    }
    query.bindValue(index++, product->id);
    if(!query.exec()){
        logger_->error("UPDATE product is error");
        return false;
    }

    auto& old_product = product_map_[product->id];
    old_product->name = product->name;
    old_product->cost_price = product->cost_price;
    old_product->sale_price = product->sale_price;
    old_product->stock_count = product->stock_count;
    old_product->remark = product->remark;
    old_product->category = product->category;
    old_product->category_id = product->category_id;
    old_product->business = product->business;
    old_product->business_id = product->business_id;

    logger_->info("modify product : id = {} , name = {}, cost_price = {}, sale_price = {} , "
                  "stock_count = {} , business = {} , remark = {}",
                  product->id, product->name.toStdString(),product->cost_price,
                  product->sale_price,product->stock_count,product->business?product->business->name.toStdString():"",
                  product->remark.toStdString());

    return true;
}

bool  DataManager::DeleteProduct(int id,bool& is_data_related)
{
    //检查是否可以删除
    auto iter = std::find_if(sale_map_.begin(),sale_map_.end(),[id](const auto& sale_pair)->bool{
        return sale_pair.second->product_id == id;
    });
    if(iter != sale_map_.end()){
        is_data_related = true;
        return false;
    }
    is_data_related = false;

    QSqlQuery query(db_);
    query.prepare("DELETE FROM product WHERE id = ?");
    query.bindValue(0, id);
    if(!query.exec()){
        logger_->error("DELETE product is error");
        return false;
    }

    const auto& product = product_map_[id];
    logger_->info("modify product : id = {} , name = {}, cost_price = {}, sale_price = {} , "
                  "stock_count = {} , business = {} , remark = {}",
                  product->id, product->name.toStdString(),product->cost_price,
                  product->sale_price,product->stock_count,product->business?product->business->name.toStdString():"",
                  product->remark.toStdString());

    product_map_.erase(id);
    return true;
}
//-----------------------------------------------------------------------
//销售
//-----------------------------------------------------------------------
bool  DataManager::AddSale(SalePointer& sale)
{
    QSqlQuery query(db_);
    query.prepare("INSERT INTO sale (total_price,count,date,remark,product_name,product_id) "
                            "VALUES (:total_price,:count,:date,:remark,:product_name,:product_id)");
    query.bindValue(":total_price", sale->total_price);
    query.bindValue(":count", sale->count);
    query.bindValue(":date", sale->date);
    query.bindValue(":remark", sale->remark);
    if(sale->product){
        query.bindValue(":product_name", sale->product->name);
        query.bindValue(":product_id", sale->product->id);
    }else{
        query.bindValue(":product_name", sale->product_name);
        query.bindValue(":product_id", sale->product_id);
    }
    if(!query.exec()){
        logger_->error("INSERT sale is error");
        return false;
    }
    sale->id = query.lastInsertId().toInt();
    sale_map_[sale->id] = sale;

    logger_->info("add sale : id = {} , date = {}, total_price = {}, count = {}, "
                  "product_name = {} , remark = {}",
                  sale->id, sale->date.toString().toStdString(),sale->total_price,
                  sale->count,sale->product?sale->product->name.toStdString():sale->product_name.toStdString(),
                  sale->remark.toStdString());

    return true;
}

bool  DataManager::ModifySale(SalePointer& sale)
{
    QSqlQuery query(db_);
    query.prepare("UPDATE sale SET total_price = ?,count = ?,date = ?,"
                  "remark=?,product_name=?,product_id=? WHERE id = ?");
    int index=0;
    query.bindValue(index++, sale->total_price);
    query.bindValue(index++, sale->count);
    query.bindValue(index++, sale->date);
    query.bindValue(index++, sale->remark);
    if(sale->product){
        query.bindValue(index++, sale->product->name);
        query.bindValue(index++, sale->product->id);
    }else{
        query.bindValue(index++, sale->product_name);
        query.bindValue(index++, sale->product_id);
    }
    query.bindValue(index++, sale->id);
    if(!query.exec()){
        logger_->error("UPDATE sale is error(id={})",sale->id);
        return false;
    }

    auto& old_sale = sale_map_[sale->id];
    old_sale->total_price = sale->total_price;
    old_sale->count = sale->count;
    old_sale->date = sale->date;
    old_sale->remark = sale->remark;
    old_sale->product = sale->product;
    old_sale->product_id = sale->product_id;
    old_sale->product_name = sale->product_name;


    logger_->info("modify sale : id = {} , date = {}, total_price = {}, count = {}, "
                  "product_name = {} , remark = {}",
                  sale->id, sale->date.toString().toStdString(),sale->total_price,
                  sale->count,sale->product?sale->product->name.toStdString():sale->product_name.toStdString(),
                  sale->remark.toStdString());

    return true;
}

bool  DataManager::DeleteSale(int id)
{
    QSqlQuery query(db_);
    query.prepare("DELETE FROM sale WHERE id = ?");
    query.bindValue(0, id);
    if(!query.exec()){
        logger_->error("DELETE sale is error");
        return false;
    }

    const auto& sale = sale_map_[id];
    logger_->info("delete sale : id = {} , date = {}, total_price = {}, count = {}, "
                  "product_name = {} , remark = {}",
                  sale->id, sale->date.toString().toStdString(),sale->total_price,
                  sale->count,sale->product?sale->product->name.toStdString():sale->product_name.toStdString(),
                  sale->remark.toStdString());

    sale_map_.erase(id);
    return true;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

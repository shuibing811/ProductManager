#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H

#include <QDate>
#include <QDateTime>
#include <QString>
#include <memory>

#define ChineseText(text) QString::fromLocal8Bit(text)

struct Carriage{
  int       id;
  QDate     date;
  double    carriage_cost;
  double    purchase_cost;
  QString   remark;
};

struct Business{
  int       id;
  QString   name;
  QString   phone;
  QString   address;
  QString   remark;
};

struct Category{
    int     id;
    QString name;
};

struct Product{
    int     id;
    QString name;
    double  cost_price;
    double  sale_price;
    int     stock_count;
    QString remark;
    //类别
    int     category_id;
    std::shared_ptr<Category> category;
    //商家
    int     business_id;
    std::shared_ptr<Business> business;
};
struct Sale{
    int     id;
    double  total_price;
    int     count;
    QDate   date;
    QString remark;
    //产品ID
    int     product_id;
    QString product_name;
    std::shared_ptr<Product> product;
};

struct User{
    QString name;
    QString password;
    int     permission;
    QDateTime create_time;
    QString remark;
};

using CarriagePointer = std::shared_ptr<Carriage>;
using BusinessPointer = std::shared_ptr<Business>;
using CategoryPointer = std::shared_ptr<Category>;
using ProductPointer = std::shared_ptr<Product>;
using SalePointer = std::shared_ptr<Sale>;
using UserPointer = std::shared_ptr<User>;

#endif // COMMONDEFINE_H

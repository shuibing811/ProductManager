#include "DialogCarriage.h"
#include "ui_DialogCarriage.h"

DialogCarriage::DialogCarriage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCarriage)
{
    ui->setupUi(this);
    ui->dteDate->setDate(QDate::currentDate());
    carriage_id_ = 0;
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DialogCarriage::~DialogCarriage()
{
    delete ui;
}

void DialogCarriage::SetCarriage(const CarriagePointer& carriage)
{
    carriage_id_ = carriage->id;
    ui->dteDate->setDate(carriage->date);
    ui->dsbPurchaseCost->setValue(carriage->purchase_cost);
    ui->dsbCarriageCost->setValue(carriage->carriage_cost);
    ui->teRemark->setPlainText(carriage->remark);
}
CarriagePointer DialogCarriage::GetCarriage()
{
    CarriagePointer carriage = std::make_shared<Carriage>();
    carriage->id = carriage_id_;
    carriage->date = ui->dteDate->date();
    carriage->purchase_cost = ui->dsbPurchaseCost->value();
    carriage->carriage_cost = ui->dsbCarriageCost->value();
    carriage->remark = ui->teRemark->toPlainText();
    return carriage;
}

void DialogCarriage::on_btnOk_clicked()
{
    this->accept();
}

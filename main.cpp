#include "mainwindow.h"
#include <QApplication>
#include "DataManager.h"

#include "DialogLogin.h"

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    g_data_manager.SetLogger(w.GetLogger());
    if(!g_data_manager.Init()){
        QMessageBox::critical(nullptr,u8"错误",u8"加载数据错误");
        return -1;
    }

    DialogLogin login_dialog;
    if(login_dialog.exec() == QDialog::Rejected){
        return -1;
    }

    w.InitData();
    w.show();
    return a.exec();
}

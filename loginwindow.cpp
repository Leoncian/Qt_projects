#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    flag = 0;
    ui->pushButton->hide(); // 默认隐藏 "click me!" 按钮
    test_db();
    sensorMainWindow = nullptr; // Initialize pointer
}

LoginWindow::~LoginWindow()
{
    delete ui;
    if (sensorMainWindow) {
        delete sensorMainWindow;
    }
}


void LoginWindow::on_pushButton_clicked()
{
    if(flag == 0){
        flag = 1;
        ui->label->hide();
        // // 蜂鸣器开启逻辑
        // int fd = open("/sys/devices/platform/leds/leds/beep/brightness", O_WRONLY);
        // if(fd >= 0) {
        //     write(fd, "1", 1); // 假设写入"1"开启蜂鸣器
        //     ::close(fd);
        //     buzzerState = true;
        // }
    } else {
        flag = 0;
        ui->label->show();
        // // 蜂鸣器关闭逻辑
        // int fd = open("/sys/devices/platform/leds/leds/beep/brightness", O_WRONLY);
        // if(fd >= 0) {
        //     write(fd, "0", 1); // 假设写入"0"关闭蜂鸣器
        //     ::close(fd);
        //     buzzerState = false;
        // }
    }
}

void LoginWindow::test_db()
{
    ///1. 设置数据库路径并初始化
    QString path = "./test.db";
    DatabaseManager& db = DatabaseManager::getInstance();
    if(!db.init(path))
    {
        qCritical() << "db init error!";
        return;
    }

    ///2. 创建数据表
    QSqlQuery query(db.getDatabase());
    query.exec("DROP TABLE IF EXISTS test"); //创建前drop清空数据表
    QString create_user = "CREATE TABLE IF NOT EXISTS test("
                          "id INTEGER PRIMARY KEY,"
                          "user_name VARCHAR(20) NOT NULL,"
                          "PASSWD VARCHAR(20) NOT NULL"
                          ")";
    if (query.exec(create_user))
    {
        qDebug() << "create table user succeed!";
    }else{
        qDebug() << "create table user error!";
    }

    ///3. 查询并显示所有表名
    query.exec("select name from sqlite_master where type='table'");
    while(query.next())
        qDebug() << "\n" << query.value(0).toString();

    ///4. 插入测试数据
    query.exec("INSERT INTO test VALUES('1', 'user', '123')");

    ///5. 查询并显示所有数据
    query.exec("SELECT * FROM test");
    query.next();
    qDebug() << query.value(0).toString();
    qDebug() << query.value(1).toString();
    qDebug() << query.value(2).toString();

    // ///6. 条件查询（存在的数据）
    // query.exec("SELECT * FROM test where user_name = 'admin'");
    // query.next();
    // qDebug() << query.value(0).toString();
    // qDebug() << query.value(1).toString();
    // qDebug() << query.value(2).toString();

    ///7. 条件查询（不存在的数据)
    // query.exec("SELECT * FROM test where user_name = 'sss' AND passwd ='ddd'");
    // if(query.next())
    //     qDebug() << "yes";
    // else
    //     qDebug() << "no";

    ///8. 字符串格式化演示
    QString userInfo;

    if(query.next())
    {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        int password = query.value(2).toInt();

        userInfo = QString("ID: %1, USERNAME: %2, PASSWORD: %3")
                       .arg(id)
                       .arg(username)
                       .arg(password);
        qDebug() << userInfo;
    }
}


void LoginWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->text();    // 获取用户名
    ui->lineEdit_2->text();   // 获取密码

    qDebug() << "用户名:" << ui->lineEdit->text();
    qDebug() << "密码:" << ui->lineEdit_2->text();

    // 查询数据库验证用户
    QSqlQuery query(DatabaseManager::getInstance().getDatabase());
    query.prepare("SELECT * FROM test WHERE user_name = ? AND PASSWD = ?");
    query.addBindValue(ui->lineEdit->text());
    query.addBindValue(ui->lineEdit_2->text());

    if(query.exec() && query.next()) {
        qDebug() << "登录成功!";
        QMessageBox::information(this,"欢迎","登录成功");
        // this->hide(); // 登录窗口隐藏，只显示数据可视化窗口
        ui->pushButton->show(); // 登录成功，显示 "click me!" 按钮
        if (!sensorMainWindow) {
            sensorMainWindow = new MainWindow(); // Create the sensor's main window
        }
        sensorMainWindow->show(); // Show the sensor's main window
    } else {
        qDebug() << "登录失败!";
        QMessageBox::information(this,"错误","登录失败");
        ui->pushButton->hide(); // 登录失败，隐藏 "click me!" 按钮
    }
}

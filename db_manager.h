#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QMutex>

// 数据库管理单例类
class DatabaseManager
{
public:
    // 禁止拷贝构造和赋值操作
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // 获取单例实例
    static DatabaseManager& getInstance();

    // 初始化数据库连接
    bool init(const QString& dbPath);

    // 获取数据库连接
    QSqlDatabase getDatabase() const;

    // 检查数据库是否已打开
    bool isOpen() const;

private:
    // 私有构造函数，确保只能通过getInstance()获取实例
    DatabaseManager();
    ~DatabaseManager();

    static QMutex m_mutex;          // 线程安全锁
    static DatabaseManager* m_instance;  // 单例实例
    QSqlDatabase m_db;              // 数据库连接
    bool m_isInitialized;           // 初始化标志
};

#endif // DB_MANAGER_H
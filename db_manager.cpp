#include "db_manager.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

// 初始化静态成员
QMutex DatabaseManager::m_mutex;
DatabaseManager* DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager()
    : m_isInitialized(false)
{
    // 私有构造函数，初始化操作在init()中进行
}

DatabaseManager::~DatabaseManager()
{
    // 关闭数据库连接
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "数据库连接已关闭";
    }
}

DatabaseManager& DatabaseManager::getInstance()
{
    // 双重检查锁定，确保线程安全
    if (!m_instance) {
        QMutexLocker locker(&m_mutex);
        if (!m_instance) {
            m_instance = new DatabaseManager();
        }
    }
    return *m_instance;
}

bool DatabaseManager::init(const QString& dbPath)
{
    QMutexLocker locker(&m_mutex);

    // 如果已经初始化，直接返回
    if (m_isInitialized) {
        return true;
    }

    // 检查SQLite驱动是否可用
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCritical() << "错误: SQLite驱动不可用!";
        return false;
    }

    // 创建数据库连接
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    // 打开数据库
    if (!m_db.open()) {
        qCritical() << "错误: 无法打开数据库:" << m_db.lastError().text();
        return false;
    }

    m_isInitialized = true;
    qDebug() << "成功打开数据库:" << dbPath;
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return m_db;
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}
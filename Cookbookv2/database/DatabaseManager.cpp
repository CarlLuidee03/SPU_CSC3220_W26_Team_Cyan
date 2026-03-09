#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connect(const QString& dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        qDebug() << "Database open error:" << m_db.lastError().text();
        return false;
    }

    return initializeSchema();
}

void DatabaseManager::disconnect()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

bool DatabaseManager::initializeSchema()
{
    QSqlQuery query;

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS recipes (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT,
            category TEXT,
            servings INTEGER,
            prep_minutes INTEGER,
            cook_minutes INTEGER,
            total_minutes INTEGER,
            description TEXT
        )
    )");

    query.exec(R"(
    CREATE TABLE IF NOT EXISTS ingredients (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        recipe_id INTEGER,
        name TEXT,
        quantity REAL DEFAULT 0,
        unit TEXT DEFAULT '',
        FOREIGN KEY(recipe_id) REFERENCES recipes(id) ON DELETE CASCADE
    )
)");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS instructions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            recipe_id INTEGER,
            step_number INTEGER,
            instruction TEXT
        )
    )");

    return true;
}

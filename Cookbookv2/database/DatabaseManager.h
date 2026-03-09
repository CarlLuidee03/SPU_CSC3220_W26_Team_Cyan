#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool connect(const QString& dbPath);
    void disconnect();
    bool isOpen() const;
    QSqlDatabase database() const;

private:
    DatabaseManager() = default;
    bool initializeSchema();

    QSqlDatabase m_db;
};

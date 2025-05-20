#include "DatabaseManager.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace PerfMgmt {
DatabaseManager::DatabaseManager(const QString& dbPath)
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(dbPath);

   if (!db.open())
   {
      qCritical() << "Failed to open database:" << db.lastError().text();
   }
   else
   {
      InitializeDatabase();
   }
}

DatabaseManager::~DatabaseManager()
{
   if (db.isOpen())
   {
      db.close();
   }
}

bool DatabaseManager::InitializeDatabase()
{
   QSqlQuery query;

   QString   createEmployees =
      "CREATE TABLE IF NOT EXISTS employees ("
      "employee_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "name TEXT NOT NULL,"
      "role TEXT NOT NULL CHECK(role IN ('Manager', 'Boss', 'Specialist', 'Technician')),"
      "reports_to INT,"
      "hire_date TEXT,"
      "personnel_code INT,"
      "is_active INT DEFAULT 1,"
      "FOREIGN KEY (reports_to) REFERENCES employees(employee_id));";

   QString createReviews =
      "CREATE TABLE IF NOT EXISTS performance_reviews ("
      "review_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "employee_id INTEGER NOT NULL,"
      "reviewer_id INTEGER NOT NULL,"
      "review_date DATE NOT NULL DEFAULT CURRENT_DATE,"
      "overall_rating REAL,"
      "comments TEXT,"
      "punctuality_rating REAL CHECK(punctuality_rating BETWEEN 1 AND 10),"
      "quality_of_work_rating REAL CHECK(quality_of_work_rating BETWEEN 1 AND 10),"
      "teamwork_rating REAL CHECK(teamwork_rating BETWEEN 1 AND 10),"
      "communication_rating REAL CHECK(communication_rating BETWEEN 1 AND 10),"
      "problem_solving_rating REAL CHECK(problem_solving_rating BETWEEN 1 AND 10),"
      "creativity_rating REAL CHECK(creativity_rating BETWEEN 1 AND 10),"
      "technical_skills_rating REAL CHECK(technical_skills_rating BETWEEN 1 AND 10),"
      "adaptability_rating REAL CHECK(adaptability_rating BETWEEN 1 AND 10),"
      "leadership_rating REAL CHECK(leadership_rating BETWEEN 1 AND 10),"
      "initiative_rating REAL CHECK(initiative_rating BETWEEN 1 AND 10),"
      "FOREIGN KEY (employee_id) REFERENCES employees(employee_id),"
      "FOREIGN KEY (reviewer_id) REFERENCES employees(employee_id));";

   if (!query.exec(createEmployees))
   {
      qCritical() << "Create employees table failed:" << query.lastError().text();
      return false;
   }

   if (!query.exec(createReviews))
   {
      qCritical() << "Create performance_reviews table failed:" << query.lastError().text();
      return false;
   }

   return true;
}

bool DatabaseManager::addEmployee(const Employee& e)
{
   QSqlQuery query;

   query.prepare("INSERT INTO employees (employee_id, name, role, reports_to, hire_date, personnel_code, is_active) "
                 "VALUES (:id, :name, :role, :reportsTo, :hireDate, :personnelCode, :isActive)");

   query.bindValue(":id", e.employeeId);
   query.bindValue(":name", QString::fromStdString(e.name));
   query.bindValue(":role", QString::fromStdString(roleToString(e.role)));
   query.bindValue(":reportsTo", e.reportsTo.has_value() ? QVariant(e.reportsTo.value()) : QVariant(QVariant::Int));
   query.bindValue(":hireDate", QString::fromStdString(e.hireDate));
   query.bindValue(":personnelCode", e.personnelCode);
   query.bindValue(":isActive", e.isActive ? 1 : 0);

   if (!query.exec())
   {
      qCritical() << "[addEmployee] Failed:" << query.lastError().text();
      return false;
   }

   return true;
}

std::optional<Employee> DatabaseManager::getEmployee(int id)
{
   QSqlQuery query;

   query.prepare("SELECT employee_id, name, role, reports_to, hire_date, personnel_code, is_active "
                 "FROM employees WHERE employee_id = :id");
   query.bindValue(":id", id);

   if (!query.exec())
   {
      qCritical() << "[getEmployee] Failed:" << query.lastError().text();
      return std::nullopt;
   }

   if (query.next())
   {
      return parseEmployeeFromQuery(query);
   }

   return std::nullopt;
}

Employee DatabaseManager::parseEmployeeFromQuery(const QSqlQuery& query) const
{
   Employee e;

   e.employeeId = query.value("employee_id").toInt();
   e.name       = query.value("name").toString().toStdString();
   e.role       = stringToRole(query.value("role").toString());

   if (!query.value("reports_to").isNull())
   {
      e.reportsTo = query.value("reports_to").toInt();
   }

   e.hireDate      = query.value("hire_date").toString().toStdString();
   e.personnelCode = query.value("personnel_code").toInt();
   e.isActive      = query.value("is_active").toBool();
   return e;
}
} // namespace PerfMgmt

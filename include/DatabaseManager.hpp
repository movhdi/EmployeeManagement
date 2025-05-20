#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <Models.hpp>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <optional>
#include <vector>

namespace PerfMgmt {
class DatabaseManager {
public:
DatabaseManager(DatabaseManager& other)  = delete;
DatabaseManager(DatabaseManager&& other) = delete;
explicit DatabaseManager(const QString& dbPath);
~DatabaseManager();

bool InitializeDatabase();

// ---- Employee management ----
bool addEmployee(const Employee& employee);
std::optional<Employee> getEmployee(int employeeId);
std::optional<std::vector<Employee> > getAllEmployees();
std::optional<std::vector<Employee> > getEmployeesReportingToHead(int reviewerId);
bool updateEmployee(const Employee& employee);
bool deactivateEmployee(int employeeId);

// ---- Performance Review Management ----
bool addPerformanceReview(const PerformanceReview& review);
std::optional<PerformanceReview> getPerformanceReview(int reviewId);
std::optional<PerformanceReview> getPerformanceForEmployee(int employeeId);

private:
QSqlDatabase db;

Employee parseEmployeeFromQuery(const QSqlQuery& query) const;
PerformanceReview parseReviewFromQuery(const QSqlQuery& query) const;
};
} // namespace PerfMgmt

#endif // DATABASEMANAGER_HPP

#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include <Models.hpp>
#include <sqlite_modern_cpp.h>
#include <string>
#include <vector>

namespace PerfMgmt {

class DatabaseManager {
public:
    DatabaseManager(DatabaseManager& other) = delete;
    DatabaseManager(DatabaseManager&& other) = delete;
    explicit DatabaseManager(const std::string& dbAddress);
    ~DatabaseManager() = default;

    bool InitializeDatabase();

    // ---- Employee management ----

    // 1. addEmployee
    bool addEmployee(const Employee& employee);
    // 2. getEmployee
    Employee getEmployee(int emplyeeId);
    // 3. getAllEmployees
    std::optional<std::vector<Employee>> getAllEmployees();
    // 4. getDirectReports
    std::optional<std::vector<Employee>> getEmployeesReportingToHead(int reviewerId);

    // 5. updateEmployee
    bool updateEmployee(const Employee& employee);

    // 6. deactivateEmployee
    bool deactivateEmployee(int employeeId);

    // ---- Performance Review Management ----

    // 1. addPerformanceReivew
    bool addPerformanceReview(const performanceReview& review);
    // 2. getPerformanceReview by id
    std::optional<performanceReview> getPerformanceReview(int reviewId);
    // 3. getReviewForEmployee
    std::optional<performanceReview> getPerformanceForEmployee(const int& employeeId);
    // 4. getReviewByReviewer
    std::optional<std::vector<performanceReview>> getReviewByReviewe(int reviewerId);
    // 5. updatePerformanceReview
    bool updatePerformanceReview(int reviewId);
    // 6. deletePerformanceReivew
    bool deletePerformanceReview(int reviewId);

private:
    sqlite::database db;
    // lambda getter which is fed into db << operator and populates employee(s)
    std::function<void(int, std::string, std::string, int, std::string, int, bool)>
    getMultipleEmployeeCollector(std::vector<Employee>& employees) const;

    // lambda getter which is fed into db << operator and populates employee
    std::function<void(int, std::string, int, std::string, std::string, int, bool)>
    getSingleEmployeeCollector(Employee& employee) const;

    // lambda getter which is fed into db << operator and populates performanceReview
    std::function<void(int, int, int, std::string, float, std::string, float, float, float, float, float, float, float,
                       float, float, float)>
    getPerformanceReviewCollector(performanceReview& review) const;
    // map database entry to Employee datastructure

    // map performance review to PerformanceReview data structure

protected:
};

} // namespace PerfMgmt

#endif // DATABASEMANAGER_HPP
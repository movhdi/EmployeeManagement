#include <DatabaseManager.hpp>
#include <Models.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    PerfMgmt::DatabaseManager db("databaseExample.db");
    PerfMgmt::Employee employee1(0, 20251207, "George Michael", "20200101", PerfMgmt::Role::Boss, true, std::nullopt);
    PerfMgmt::Employee employee2(1, 20251208, "John Nash", "20200101", PerfMgmt::Role::Specialist, true, 0);
    PerfMgmt::Employee employee3(2, 20251209, "Albert Einstein", "20200101", PerfMgmt::Role::Specialist, true, 0);
    PerfMgmt::Employee employee4(3, 20251210, "Mahmoud Hesabi", "20200101", PerfMgmt::Role::Specialist, true, 0);

    db.addEmployee(employee1);
    db.addEmployee(employee2);
    db.addEmployee(employee3);
    db.addEmployee(employee4);
    // auto response = db.getEmployee(0);
    auto employees_ = db.getAllEmployees();
    if (employees_) {
        for (const auto& employee : employees_.value()) {
            std::cout << employee;
        }
    }

    auto employees = db.getEmployeesReportingToHead(0);
    if (employees) {
        for (const auto& employee : employees.value()) {
            std::cout << employee;
        }
    }
    std::cout << db.getEmployee(1);
    return 0;
}
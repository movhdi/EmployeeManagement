#include <DatabaseManager.hpp>
#include <Models.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    PerfMgmt::DatabaseManager db("databaseExample.db");
    PerfMgmt::Employee employee1(1, 20251207, "George Michael", "20200101", PerfMgmt::Role::Boss, true, std::nullopt);
    PerfMgmt::Employee employee2(2, 20251208, "John Nash", "20200101", PerfMgmt::Role::Specialist, true, 1);
    PerfMgmt::Employee employee3(3, 20251209, "Albert Einstein", "20200101", PerfMgmt::Role::Specialist, true, 1);
    PerfMgmt::Employee employee4(4, 20251210, "Mahmoud Hesabi", "20200101", PerfMgmt::Role::Specialist, true, 1);
    PerfMgmt::Employee employee5(4, 20251211, "Mahdi Movahedi", "20200101", PerfMgmt::Role::Boss, true, 4);

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
    // db.updateEmployee(employee5);
    auto employees = db.getEmployeesReportingToHead(0);
    if (employees) {
        for (const auto& employee : employees.value()) {
            std::cout << employee;
        }
    }
    db.deactivateEmployee(4);
    std::cout << db.getEmployee(4);

    return 0;
}
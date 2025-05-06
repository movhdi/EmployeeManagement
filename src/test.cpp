#include <DatabaseManager.hpp>
#include <Models.hpp>
#include <NetworkManager.hpp>
#include <iostream>

namespace PerfMgmt {

void test_App() {
    DatabaseManager db("databaseExample.db");
    Employee employee1(1, 20251207, "George Michael", "20200101", PerfMgmt::Role::BOSS, true, NULL);
    Employee employee2(2, 20251208, "John Nash", "20200101", PerfMgmt::Role::SPECIALIST, true, 1);
    Employee employee3(3, 20251209, "Albert Einstein", "20200101", PerfMgmt::Role::SPECIALIST, true, 1);
    Employee employee4(4, 20251210, "Mahmoud Hesabi", "20200101", PerfMgmt::Role::SPECIALIST, true, 1);
    Employee employee5(4, 20251211, "Mahdi Movahedi", "20200101", PerfMgmt::Role::BOSS, true, 4);
    db.addEmployee(employee1);
    db.addEmployee(employee2);
    db.addEmployee(employee3);
    db.addEmployee(employee4);

    auto response = db.getEmployee(0);

    auto employees = db.getAllEmployees();

    // if (employees) {
    //     for (const auto& employee : employees.value()) {
    //         std::cout << employee;
    //     }
    // }
    // db.updateEmployee(employee5);
    // auto employees = db.getEmployeesReportingToHead(0);
    // if (employees) {
    //     for (const auto& employee : employees.value()) {
    //         std::cout << employee;
    //     }
    // }

    db.deactivateEmployee(4);

    // if (auto result = db.getEmployee(4)) {
    //     std::cout << result.value();
    // }

    // Do some performance review insertion and extraction
    PerformanceReview reviewForEmployee2(1, 2, 1, "2025-04-27", 9.5f, 9.0f, 8.5f, 8.0f, 9.0f, 9.9f, 10.0f, 10.0f, 8.0f,
                                         7.0f, 10.0f, "He is good!");
    db.addPerformanceReview(reviewForEmployee2);
    auto review = db.getPerformanceForEmployee(2);
    // std::cout << "-----------Review------------" << std::endl;
    // if (review) {
    //     std::cout << review.value();
    // }
    NetworkManager networkManager("127.0.0.1:5000");
    networkManager.fetchAllEmployees();
}

} // namespace PerfMgmt
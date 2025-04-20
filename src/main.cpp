#include <DatabaseManager.hpp>
#include <Models.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    PerfMgmt::DatabaseManager db("databaseExample.db");
    PerfMgmt::Employee employee1(0, 20251207, "George Michael", "20200101", PerfMgmt::Role::Boss, true, std::nullopt);
    PerfMgmt::Employee employee2(1, 20251208, "John Nash", "20200101", PerfMgmt::Role::Specialist, true, 1);
    

    db.addEmployee(employee1);
    db.addEmployee(employee2);
    // auto response = db.getEmployee(0);
    auto employees = db.getAllEmployees();
    std::cout << "---- first employee ----" << std::endl; 
    std::cout << employees.value().front() << std::endl;
    std::cout << "---- second employee ----" << std::endl; 
    std::cout << employees.value().back() << std::endl;
    return 0;
}
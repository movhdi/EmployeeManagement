#include <DatabaseManager.hpp>
#include <Models.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    // PerfMgmt::Employee employee(10021u, 981207,"Mahdi Movahedi", "2023-10-01", PerfMgmt::Role::Boss, true, 100238u);
    PerfMgmt::DatabaseManager db("databaseExample.db");
    PerfMgmt::Employee employeeMovahedi(0, 981207, "Mahdi Movahedi", "13980903", PerfMgmt::Role::Boss, true, std::nullopt);
    
    std::cout << employeeMovahedi << std::endl;

    db.addEmployee(employeeMovahedi);
    auto response = db.getEmployee(0);
    std::cout << response << std::endl;
    return 0;
}
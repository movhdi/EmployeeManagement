#include "DatabaseManager.hpp"
#include <optional>

namespace PerfMgmt {

DatabaseManager::DatabaseManager(const std::string &dbAddress) : db(dbAddress) {
  // additional initializations
  this->InitializeDatabase();
}

bool DatabaseManager::InitializeDatabase() {
  try {
    db << "CREATE TABLE IF NOT EXISTS Employees ("
          "EmployeeID INTEGER PRIMARY KEY AUTOINCREMENT,"
          "Name TEXT NOT NULL,"
          "Role TEXT NOT NULL CHECK(Role IN ('Manager', 'Boss', 'Specialist', "
          "'technician')),"
          "ReportsTo INT,"
          "HireDate TEXT,"
          "PersonnelCode INT,"
          "IsActive INT DEFAULT 1,"
          "FOREIGN KEY (ReportsTo) REFERENCES Employees(EmployeeID)"
          ");";

    db << "CREATE TABLE IF NOT EXISTS PerformanceReviews ("
          "ReviewID INTEGER PRIMARY KEY AUTOINCREMENT,"
          "EmployeeID INTEGER NOT NULL,"
          "ReviewerID INTEGER NOT NULL,"
          "ReviewDate DATE NOT NULL DEFAULT CURRENT_DATE,"
          "OverallRating REAL,"
          "Comments TEXT,"

          "PunctualityRating REAL CHECK(PunctualityRating BETWEEN 1 AND 10),"
          "QualityOfWorkRating REAL CHECK(QualityOfWorkRating BETWEEN 1 AND "
          "10),"
          "TeamworkRating REAL CHECK(TeamworkRating BETWEEN 1 AND 10),"
          "CommunicationRating REAL CHECK(CommunicationRating BETWEEN 1 AND "
          "10),"
          "ProblemSolvingRating REAL CHECK(ProblemSolvingRating BETWEEN 1 AND "
          "10),"
          "CreativityRating REAL CHECK(CreativityRating BETWEEN 1 AND 10),"
          "TechnicalSkillsRating REAL CHECK(TechnicalSkillsRating BETWEEN 1 "
          "AND 10),"
          "AdaptabilityRating REAL CHECK(AdaptabilityRating BETWEEN 1 AND 10),"
          "LeadershipRating REAL CHECK(LeadershipRating BETWEEN 1 AND 10),"
          "InitiativeRating REAL CHECK(InitiativeRating BETWEEN 1 AND 10),"

          "FOREIGN KEY (EmployeeID) REFERENCES Employees(EmployeeID),"
          "FOREIGN KEY (ReviewerID) REFERENCES Employees(EmployeeID)"
          ");";

    db << "CREATE INDEX IF NOT EXISTS idx_employee_name ON Employees(Name);";
    db << "CREATE INDEX IF NOT EXISTS idx_employee_role ON Employees(Role);";
    db << "CREATE INDEX IF NOT EXISTS idx_review_employee_id ON "
          "PerformanceReviews(EmployeeID);";
    db << "CREATE INDEX IF NOT EXISTS idx_review_reviewer_id ON "
          "PerformanceReviews(ReviewerID);";
    db << "CREATE INDEX IF NOT EXISTS idx_review_date ON "
          "PerformanceReviews(ReviewDate);";
  } catch (const std::exception &e) {
    std::cout << "Exception Occured in dataabse initialization" << std::endl;
    std::cerr << e.what() << '\n';
  }
  return 0;
}

bool DatabaseManager::addEmployee(const Employee &employee) {

  bool employeeIsManager = !employee.ReportsTo.has_value();

  try {
    if (employeeIsManager) {
      db << "insert into Employees (EmployeeID, Name, Role, HireDate, "
            "PersonnelCode, IsActive) values (?,?,?,?,?,?);"
         << employee.EmployeeId << employee.Name
         << roleToString(employee.role)
         // << static_cast<int>(employee.ReportsTo.value())
         << employee.HireDate << employee.personnelCode
         << static_cast<int>(employee.isActive);
    }
  } catch (const std::exception &e) {
    std::cout << "Employee is Manager, and add employee failed" << std::endl;
    std::cerr << e.what() << '\n';
  }
  try {
    if (!employeeIsManager) {
      db << "insert into Employees (EmployeeID, Name, Role, ReportsTo, "
            "HireDate, "
            "PersonnelCode, IsActive) values (?,?,?,?,?,?,?);"
         << employee.EmployeeId << employee.Name << roleToString(employee.role)
         << static_cast<int>(employee.ReportsTo.value()) << employee.HireDate
         << employee.personnelCode << static_cast<int>(employee.isActive);
    }

  } catch (const std::exception &e) {
    std::cout << "Employee is not Manager, and add employee failed"
              << std::endl;
    std::cerr << e.what() << '\n';
  }
  return true;
}

Employee DatabaseManager::getEmployee(int emplyeeId) {
  // result
  Employee employeeTemp;

  try {
    db << "SELECT EmployeeID, Name, ReportsTo, Role, HireDate, PersonnelCode, "
          "IsActive FROM Employees WHERE employeeId = ?;"
       << emplyeeId >>
        [&](int EmployeeId, std::string Name, int ReportsTo, std::string Role,
            std::string HireDate, int personnelCode, bool IsActive) {
          employeeTemp.EmployeeId = EmployeeId;
          employeeTemp.Name = Name;
          employeeTemp.ReportsTo = ReportsTo;
          employeeTemp.HireDate = HireDate;
          employeeTemp.role = stringToRole(Role).value();
          employeeTemp.personnelCode = personnelCode;
        };
  } catch (const std::exception &e) {
    std::cout << "Error in geting employee occured" << std::endl;
    std::cerr << e.what() << '\n';
  }

  return employeeTemp;
}

std::optional<std::vector<Employee>> DatabaseManager::getAllEmployees() {
  std::vector<Employee> employees;
  try {
    db << "SELECT * FROM Employees;" >>
        [&](int EmployeeId, std::string Name, std::string role, int reportsTo,
            std::string hireDate, int personnelCode, bool isActive) {
          Employee tempEmployee;
          tempEmployee.EmployeeId = EmployeeId;
          tempEmployee.Name = Name;
          tempEmployee.role = PerfMgmt::stringToRole(role).value();
          tempEmployee.ReportsTo = reportsTo;
          tempEmployee.HireDate = hireDate;
          tempEmployee.isActive = isActive;
          tempEmployee.personnelCode = personnelCode;
          employees.push_back(tempEmployee);
        };
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return employees;
}
std::optional<performanceReview>
DatabaseManager::getEmployeesReportingToHead(int reviewerId) {
  return std::optional<performanceReview>();
}
bool DatabaseManager::updateEmployee(const Employee &employee) { return false; }
bool DatabaseManager::deactivateEmployee(int employeeId) { return false; }
bool DatabaseManager::updatePerformanceReviwe(const performanceReview &review) {
  return false;
}
std::optional<performanceReview>
DatabaseManager::getPerformanceReview(int reviewId) {
  return std::optional<performanceReview>();
}
std::optional<performanceReview>
DatabaseManager::getPerformanceForEmployee(int employeeId) {
  return std::optional<performanceReview>();
}
std::optional<std::vector<performanceReview>>
DatabaseManager::getReviewByReviewe(int reviewerId) {
  return std::optional<std::vector<performanceReview>>();
}
bool DatabaseManager::updateperformanceReview(int reviewId) { return false; }
bool DatabaseManager::deletePerformanceReview(int reviewId) { return false; };

} // namespace PerfMgmt
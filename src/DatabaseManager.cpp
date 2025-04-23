#include "DatabaseManager.hpp"
#include <optional>

namespace PerfMgmt {

DatabaseManager::DatabaseManager(const std::string& dbAddress) : db(dbAddress) {
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
    } catch (const sqlite::sqlite_exception& e) {
        std::cerr << "Database initialization error: " << e.what() << " (code: " << e.get_code() << ")" << std::endl;
        throw; // Re-throw the exception to signal failure
    } catch (const std::exception& e) {
        std::cout << "Exception Occured in dataabse initialization" << std::endl;
        std::cerr << e.what() << '\n';
        throw;
    }
    return 0;
}

bool DatabaseManager::addEmployee(const Employee& employee) {
    if (employee.EmployeeId <= 0) {
        std::cerr << "Invalid employee Id " << std::endl;
        return false;
    }
    try {
        // The following lines is commented out because it runs but raises warning in compile time
        // db << "INSERT INTO Employees (EmployeeID, Name, Role, ReportsTo ,HireDate, "
        //       "PersonnelCode, IsActive) VALUES (?,?,?,?,?,?,?);"
        //    << employee.EmployeeId << employee.Name << roleToString(employee.role)
        //    << (employee.ReportsTo.has_value() ? employee.ReportsTo.value() : NULL) << employee.HireDate
        //    << employee.personnelCode << static_cast<int>(employee.isActive);
        auto stmt =
            db << "INSERT INTO Employees (EmployeeID, Name, Role, ReportsTo, HireDate, PersonnelCode, IsActive) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?);";
        stmt << employee.EmployeeId << employee.Name << roleToString(employee.role);

        if (employee.ReportsTo) {
            stmt << *employee.ReportsTo;
        } else {
            stmt << nullptr;
        }
        stmt << employee.HireDate << employee.personnelCode << static_cast<int>(employee.isActive);
        stmt.execute();
    } catch (const std::exception& e) {
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
            getSingleEmployeeCollector(employeeTemp);
    } catch (const std::exception& e) {
        std::cout << "Error in geting employee occured" << std::endl;
        std::cerr << e.what() << '\n';
    }

    return employeeTemp;
}

std::optional<std::vector<Employee>> DatabaseManager::getAllEmployees() {
    std::vector<Employee> employees;
    auto collector = getMultipleEmployeeCollector(employees);
    try {
        db << "SELECT * FROM Employees;" >> collector;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return employees;
}

std::optional<std::vector<Employee>> DatabaseManager::getEmployeesReportingToHead(const int reviewerId) {
    std::vector<Employee> employees;
    try {
        auto collector = getMultipleEmployeeCollector(employees);
        db << "SELECT * FROM Employees WHERE ReportsTo = ?;" << reviewerId >> collector;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return employees;
}

bool DatabaseManager::updateEmployee(const Employee& employee) {
    if (employee.EmployeeId <= 0) {
        std::cerr << "Invalid employee Id " << std::endl;
        return false;
    }
    try {
        db << "UPDATE Employees SET Name = ?, Role = ?, ReportsTo = ?, HireDate = ?, PersonnelCode = ?, IsActive = ? "
              "WHERE "
              "EmployeeID = ?;"
           << employee.Name << roleToString(employee.role) << employee.ReportsTo << employee.HireDate
           << employee.personnelCode << employee.isActive << employee.EmployeeId;
        std::cout << "employee updated" << std::endl;
    } catch (const sqlite::sqlite_exception& e) {
        std::cerr << "employee update error: " << e.what() << " (code: " << e.get_code() << ")" << std::endl;
        throw; // Re-throw the exception to signal failure
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return true;
}

bool DatabaseManager::deactivateEmployee(int employeeId) {
    return false;
}
bool DatabaseManager::updatePerformanceReviwe(const performanceReview& review) {
    return false;
}
std::optional<performanceReview> DatabaseManager::getPerformanceReview(int reviewId) {
    return std::optional<performanceReview>();
}
std::optional<performanceReview> DatabaseManager::getPerformanceForEmployee(int employeeId) {
    return std::optional<performanceReview>();
}
std::optional<std::vector<performanceReview>> DatabaseManager::getReviewByReviewe(int reviewerId) {
    return std::optional<std::vector<performanceReview>>();
}
bool DatabaseManager::updatePerformanceReview(int reviewId) {
    return false;
}
bool DatabaseManager::deletePerformanceReview(int reviewId) {
    return false;
}

std::function<void(int, std::string, std::string, int, std::string, int, bool)>
DatabaseManager::getMultipleEmployeeCollector(std::vector<Employee>& employees) const {
    return [&employees](int EmployeeId, std::string Name, std::string role, int reportsTo, std::string hireDate,
                        int personnelCode, bool isActive) {
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
}

std::function<void(int, std::string, int, std::string, std::string, int, bool)>
DatabaseManager::getSingleEmployeeCollector(Employee& employee) const {
    return [&](int EmployeeId, std::string Name, int ReportsTo, std::string Role, std::string HireDate,
               int personnelCode, bool IsActive) {
        employee.EmployeeId = EmployeeId;
        employee.Name = Name;
        employee.ReportsTo = ReportsTo;
        employee.HireDate = HireDate;
        employee.role = stringToRole(Role).value();
        employee.personnelCode = personnelCode;
    };
}

} // namespace PerfMgmt
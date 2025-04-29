#include "DatabaseManager.hpp"
#include <optional>

namespace PerfMgmt {

DatabaseManager::DatabaseManager(const std::string& dbAddress) : db(dbAddress) {
    // additional initializations
    this->InitializeDatabase();
}

bool DatabaseManager::InitializeDatabase() {
    try {
        db << "CREATE TABLE IF NOT EXISTS employees ("
              "employee_id INTEGER PRIMARY KEY AUTOINCREMENT,"
              "name TEXT NOT NULL,"
              "role TEXT NOT NULL CHECK(role IN ('Manager', 'Boss', 'Specialist', "
              "'Technician')),"
              "reports_to INT,"
              "hire_date TEXT,"
              "personnel_code INT,"
              "is_active INT DEFAULT 1,"
              "FOREIGN KEY (reports_to) REFERENCES employees(employee_id)"
              ");";

        db << "CREATE TABLE IF NOT EXISTS performance_reviews ("
              "review_id INTEGER PRIMARY KEY AUTOINCREMENT,"
              "employee_id INTEGER NOT NULL,"
              "reviewer_id INTEGER NOT NULL,"
              "review_date DATE NOT NULL DEFAULT CURRENT_DATE,"
              "overall_rating REAL,"
              "comments TEXT,"

              "punctuality_rating REAL CHECK(punctuality_rating BETWEEN 1 AND 10),"
              "quality_of_work_rating REAL CHECK(quality_of_work_rating BETWEEN 1 AND "
              "10),"
              "teamwork_rating REAL CHECK(teamwork_rating BETWEEN 1 AND 10),"
              "communication_rating REAL CHECK(communication_rating BETWEEN 1 AND "
              "10),"
              "problem_solving_rating REAL CHECK(problem_solving_rating BETWEEN 1 AND "
              "10),"
              "creativity_rating REAL CHECK(creativity_rating BETWEEN 1 AND 10),"
              "Technical_skills_rating REAL CHECK(technical_skills_rating BETWEEN 1 "
              "AND 10),"
              "adaptability_rating REAL CHECK(adaptability_rating BETWEEN 1 AND 10),"
              "leadership_rating REAL CHECK(leadership_rating BETWEEN 1 AND 10),"
              "initiative_rating REAL CHECK(initiative_rating BETWEEN 1 AND 10),"

              "FOREIGN KEY (employee_id) REFERENCES employees(employee_id),"
              "FOREIGN KEY (reviewer_id) REFERENCES employees(employee_id)"
              ");";

        db << "CREATE INDEX IF NOT EXISTS idx_employee_name ON employees(name);";
        db << "CREATE INDEX IF NOT EXISTS idx_employee_role ON employees(role);";
        db << "CREATE INDEX IF NOT EXISTS idx_review_employee_id ON "
              "performance_reviews(employee_id);";
        db << "CREATE INDEX IF NOT EXISTS idx_review_reviewer_id ON "
              "performance_reviews(reviewer_id);";
        db << "CREATE INDEX IF NOT EXISTS idx_review_date ON "
              "performance_reviews(review_date);";
        return true;
    } catch (const sqlite::sqlite_exception& e) {
        std::cerr << "[IitializeDatabase] : " << "Database initialization error: " << e.what()
                  << " (code: " << e.get_code() << ")" << std::endl;
        return false;
    }
}

bool DatabaseManager::addEmployee(const Employee& employee) {
    if (employee.employeeId <= 0) {
        std::cerr << "Invalid employee Id " << std::endl;
        return false;
    }
    try {
        auto stmt =
            db << "INSERT INTO employees (employee_id, name, role, reports_to, hire_date, personnel_code, is_active) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?);";
        stmt << employee.employeeId << employee.name << roleToString(employee.role);
        stmt << employee.reportsTo.value_or(SQLITE_NULL);
        stmt << employee.hireDate << employee.personnelCode << static_cast<int>(employee.isActive);
        stmt.execute();
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

std::optional<Employee> DatabaseManager::getEmployee(int emplyeeId) {
    if (emplyeeId <= 0) {
        std::cerr << "Invalid employee id" << std::endl;
        return std::nullopt;
    }

    Employee employeeResult;
    bool isFound{false};

    try {
        db << "SELECT employee_id, name, reports_to, role, hire_date, personnel_code, "
              "is_active FROM employees WHERE employee_id = ?;"
           << emplyeeId >>
            getSingleEmployeeCollector(employeeResult, isFound);
        if (isFound) {
            return employeeResult;
        } else {
            return std::nullopt;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error in geting employee" << e.what() << '\n';
        return std::nullopt;
    }
}

std::optional<std::vector<Employee>> DatabaseManager::getAllEmployees() {
    std::vector<Employee> employees;
    bool isFound{false};

    auto collector = getMultipleEmployeeCollector(employees, isFound);
    try {
        db << "SELECT * FROM employees;" >> collector;
        return employees;
    } catch (const std::exception& e) {
        std::cerr << "[getAllEmployees] " << e.what() << '\n';
        return std::nullopt;
    }
}

std::optional<std::vector<Employee>> DatabaseManager::getEmployeesReportingToHead(const int reviewerId) {
    if (reviewerId <= 0) {
        std::cerr << "Invalid reviewer id" << std::endl;
        return std::nullopt;
    }

    std::vector<Employee> employees;
    bool isFound{false};

    auto collector = getMultipleEmployeeCollector(employees, isFound);
    try {
        db << "SELECT * FROM employees WHERE reports_to = ?;" << reviewerId >> collector;
        if (isFound) {
            return employees;
        } else {
            return std::nullopt;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
}

bool DatabaseManager::updateEmployee(const Employee& employee) {
    if (employee.employeeId <= 0) {
        std::cerr << "Invalid employee Id " << std::endl;
        return false;
    }
    try {
        db << "UPDATE employees SET name = ?, role = ?, reports_to = ?, hire_date = ?, personnel_code = ?, is_active = "
              "? "
              "WHERE "
              "employee_id = ?;"
           << employee.name << roleToString(employee.role) << employee.reportsTo << employee.hireDate
           << employee.personnelCode << employee.isActive << employee.employeeId;
    } catch (const sqlite::sqlite_exception& e) {
        std::cerr << "employee update error: " << e.what() << " (code: " << e.get_code() << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return true;
}

bool DatabaseManager::deactivateEmployee(int employeeId) {
    if (employeeId <= 0) {
        std::cerr << "Invalid employee ID!" << std::endl;
        return false;
    }
    try {
        auto stmt = db << "UPDATE employees SET is_active = ? WHERE employee_id = ?;";
        stmt << 0 << employeeId;
        stmt.execute();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[deactivateEmployee] : " << e.what() << '\n';
    }
    return false;
}

bool DatabaseManager::addPerformanceReview(const PerformanceReview& review) {
    try {
        auto stmt =
            db << "INSERT INTO performance_reviews (review_id, employee_id, reviewer_id, overall_rating, comments,"
                  "punctuality_rating, quality_of_work_rating, teamwork_rating, "
                  "communication_rating, problem_solving_rating, creativity_rating, technical_skills_rating, "
                  "adaptability_rating, leadership_rating, initiative_rating) VALUES (?, ?, ?, ?, ?, ?, "
                  "?, ?, ?, ? ,? ,?, ?, ?, ?);";
        stmt << review.reviewId << review.employeeId << review.reviewerId << review.overallRating.value_or(SQLITE_NULL)
             << review.comments.value_or(nullptr) << review.punctualityRating << review.qualityOfWorkRating
             << review.teamworkRating << review.communicationRating << review.problemSolvingRating
             << review.creativityRating << review.technicalSkillsRating << review.adaptabilityRating
             << review.leadershipRating << review.initiativeRating;
        stmt.execute();
        return true;
    } catch (const sqlite::sqlite_exception& e) {
        std::cerr << "[addPerformanceReview] :" << "Review insertion failure : " << e.what()
                  << " (code: " << e.get_code() << ")" << std::endl;
        return false;
    }
}

std::optional<PerformanceReview> DatabaseManager::getPerformanceReview(const int& reviewId) {
    if (reviewId <= 0) {
        std::cerr << "Invalid review id" << std::endl;
        return std::nullopt;
    }

    PerformanceReview review;
    bool isFound{false};

    auto collector = getPerformanceReviewCollector(review, isFound);
    try {
        db << "SELECT * FROM performance_reviews WHERE review_id = (?);" << reviewId >> collector;
        if (isFound) {
            return review;
        } else {
            return std::nullopt;
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
}

std::optional<PerformanceReview> DatabaseManager::getPerformanceForEmployee(const int& employeeId) {
    PerformanceReview review;
    bool isFound{false};
    auto collector = getPerformanceReviewCollector(review, isFound);
    try {
        db << "SELECT * FROM performance_reviews WHERE employee_id = (?);" << employeeId >> collector;
        if (isFound) {
            return review;
        } else {
            return std::nullopt;
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
}

std::optional<std::vector<PerformanceReview>> DatabaseManager::getReviewByReviewer(int reviewerId) {
    return std::optional<std::vector<PerformanceReview>>();
}

bool DatabaseManager::updatePerformanceReview(int reviewId) {
    return false;
}

bool DatabaseManager::deletePerformanceReview(int reviewId) {
    return false;
}

std::function<void(int, std::string, std::string, int, std::string, int, bool)>
DatabaseManager::getMultipleEmployeeCollector(std::vector<Employee>& employees, bool& isFound) const {
    return [&](int EmployeeId, std::string Name, std::string role, std::optional<int> reportsTo, std::string hireDate,
               int personnelCode, bool isActive) {
        Employee employee;
        employee.employeeId = EmployeeId;
        employee.name = Name;
        employee.role = PerfMgmt::stringToRole(role).value();
        employee.reportsTo = reportsTo;
        employee.hireDate = hireDate;
        employee.isActive = isActive;
        employee.personnelCode = personnelCode;
        employees.push_back(employee);
        isFound = true;
    };
}

std::function<void(int, std::string, int, std::string, std::string, int, bool)>
DatabaseManager::getSingleEmployeeCollector(Employee& employee, bool& isFound) const {
    return [&](int employeeId, std::string name, std::optional<int> reportsTo, std::string roleStr,
               std::string hireDate, int personnelCode, bool isActive) {
        employee.employeeId = employeeId;
        employee.name = name;
        employee.reportsTo = reportsTo;
        employee.hireDate = hireDate;
        employee.role = stringToRole(roleStr).value();
        employee.personnelCode = personnelCode;
        employee.isActive = isActive;
        isFound = true;
    };
}

std::function<void(int, int, int, std::string, float, std::string, float, float, float, float, float, float, float,
                   float, float, float)>
DatabaseManager::getPerformanceReviewCollector(PerformanceReview& review, bool& isFound) const {
    return [&](int ReviewID, int employeeId, int reviewerId, std::string reviewDate, float overallRating,
               std::string comments, float punctualityRating, float qualityOfWorkRating, float teamworkRating,
               float communicationRating, float problemSolvingRating, float creativityRating,
               float technicalSkillRating, float adaptibilityRating, float leadershipRating, float initiativeRating) {
        review.reviewId = ReviewID;
        review.employeeId = employeeId;
        review.reviewerId = reviewerId;
        review.reviewDate = reviewDate;
        review.overallRating = overallRating;
        review.punctualityRating = punctualityRating;
        review.qualityOfWorkRating = qualityOfWorkRating;
        review.teamworkRating = teamworkRating;
        review.communicationRating = communicationRating;
        review.problemSolvingRating = problemSolvingRating;
        review.creativityRating = creativityRating;
        review.technicalSkillsRating = technicalSkillRating;
        review.adaptabilityRating = adaptibilityRating;
        review.leadershipRating = leadershipRating;
        review.initiativeRating = initiativeRating;
        review.comments = comments;
        isFound = true;
    };
}

} // namespace PerfMgmt
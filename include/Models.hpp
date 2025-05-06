#ifndef MODELS_HPP
#define MODELS_HPP
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>

using json = nlohmann::json;
namespace PerfMgmt {

enum class Role {
    MANAGER,
    BOSS,
    SPECIALIST,
    TECHNICIAN
};

inline std::string roleToString(Role role) {
    switch (role) {
    case Role::MANAGER:
        return "Manager";
        break;
    case Role::BOSS:
        return "Boss";
        break;
    case Role::SPECIALIST:
        return "Specialist";
        break;
    case Role::TECHNICIAN:
        return "Technician";
        break;
    default:
        return "unknown";
        break;
    }
}

// Helper function to convert Role to string
inline std::ostream& operator<<(std::ostream& os, Role role) {
    os << roleToString(role);
    return os;
}

// Helper function to convert string to Role enum
inline std::optional<Role> stringToRole(const std::string& roleStr) {
    if (roleStr == "Manager") {
        return Role::MANAGER;
    } else if (roleStr == "Boss") {
        return Role::BOSS;
    } else if (roleStr == "Specialist") {
        return Role::SPECIALIST;
    } else if (roleStr == "Technician") {
        return Role::TECHNICIAN;
    } else {
        return std::nullopt;
    }
}

enum class FunctionTeams {
    HARDWARE,
    SOFTWARE,
    MECHANIC,
    ENGINEERING,
    PRODUCT
};

inline std::string functionTeamsToString(FunctionTeams functionTeam) {
    switch (functionTeam) {
    case FunctionTeams::HARDWARE:
        return "HARDWARE";
        break;
    case FunctionTeams::SOFTWARE:
        return "SOFTWARE";
        break;
    case FunctionTeams::MECHANIC:
        return "MECHANIC";
        break;
    case FunctionTeams::ENGINEERING:
        return "ENGINEERING";
        break;
    case FunctionTeams::PRODUCT:
        return "PRODUCT";
        break;
    default:
        return "unknown";
        break;
    }
}

inline std::optional<FunctionTeams> stringToFunctionTeams(const std::string& functionstr) {
    if (functionstr == "HARDWARE") {
        return FunctionTeams::HARDWARE;
    } else if (functionstr == "SOFTWARE") {
        return FunctionTeams::SOFTWARE;
    } else if (functionstr == "ENGINEERING") {
        return FunctionTeams::ENGINEERING;
    } else if (functionstr == "MECHANIC") {
        return FunctionTeams::MECHANIC;
    } else if (functionstr == "PRODUCT") {
        return FunctionTeams::PRODUCT;
    } else {
        return std::nullopt;
    }
}
struct Employee {
    int employeeId{};
    int personnelCode{};
    std::string name{};
    std::string hireDate{};
    Role role = Role::SPECIALIST; // Default specialist
    // FunctionTeams team;
    bool isActive{true};
    std::optional<int> reportsTo{std::nullopt};

    friend void to_json(json& j, const Employee& employee);
    friend void from_json(const json& j, Employee& employee);

public:
    // Constructor
    Employee(int id, int personnelCode, const std::string& name, const std::string& hireDate, Role role, bool isActive,
             std::optional<int> rt) :
        employeeId{id},
        personnelCode{personnelCode},
        name{name},
        hireDate{hireDate},
        role{role},
        isActive{isActive},
        reportsTo{rt} {
    }
    // Default Constructor
    Employee() {
    }
    ~Employee() {
    }
};

inline std::ostream& operator<<(std::ostream& os, const Employee& emp) {
    os << "------------------" << std::endl;
    os << "Employee ID: " << emp.employeeId << std::endl
       << "Personnel Code: " << emp.personnelCode << std::endl
       << "Name: " << emp.name << std::endl
       << "Hire Date: " << emp.hireDate << std::endl
       << "Role: " << emp.role << std::endl
       << "Employee Status: " << (static_cast<bool>(emp.isActive) ? "Active" : "Not-Active") << std::endl
       << "ReportsTo: " << (emp.reportsTo.has_value() ? std::to_string(*emp.reportsTo) : "null") << std::endl;
    return os;
}

struct PerformanceReview {
    int reviewId{0};
    int employeeId{0};
    int reviewerId{0};
    std::string reviewDate;
    std::optional<float> overallRating;

    float punctualityRating{0.0};
    float qualityOfWorkRating{0.0};
    float communicationRating{0.0};
    float teamworkRating{0.0};
    float technicalSkillsRating{0.0};
    float problemSolvingRating{0.0};
    float creativityRating{0.0};
    float adaptabilityRating{0.0};
    float leadershipRating{0.0};
    float initiativeRating{0.0};
    std::optional<std::string> comments;

    // default constructor
    PerformanceReview() {
    }
    // constructor
    PerformanceReview(int ReviewId, int EmployeeId, int ReviewerId, const std::string& reviewDate,
                      std::optional<float> OveralRating, float PunctualityRating, float QualityOfWorkRating,
                      float CommunicationRating, float TeamworkRating, float TechnicalSkillsRating,
                      float ProblemSolvingRating, float CreativityRating, float AdaptabilityRating,
                      float LeadershipRating, float InitiativeRating, const std::string& Comments) :
        reviewId{ReviewId},
        employeeId{EmployeeId},
        reviewerId{ReviewerId},
        reviewDate{reviewDate},
        overallRating{OveralRating},
        punctualityRating{PunctualityRating},
        qualityOfWorkRating{QualityOfWorkRating},
        communicationRating{CommunicationRating},
        teamworkRating{TeamworkRating},
        technicalSkillsRating{TechnicalSkillsRating},
        problemSolvingRating{ProblemSolvingRating},
        creativityRating{CreativityRating},
        adaptabilityRating{AdaptabilityRating},
        leadershipRating{LeadershipRating},
        initiativeRating{InitiativeRating},
        comments{Comments} {
    }

    // destructor
    ~PerformanceReview() {
    }
};

inline std::ostream& operator<<(std::ostream& os, const PerformanceReview& review) {
    os << "Review ID : " << review.reviewId << std::endl << "Date of Review : " << review.reviewDate << std::endl;
    if (review.overallRating) {
        os << "Overall Rating : " << review.overallRating.value() << std::endl;
    }
    os << "Punctuality Rating : " << review.punctualityRating << std::endl
       << "Quality of Work : " << review.qualityOfWorkRating << std::endl
       << "Communication Rating : " << review.communicationRating << std::endl
       << "Teamwork Rating : " << review.teamworkRating << std::endl
       << "Technical Skill Rating : " << review.technicalSkillsRating << std::endl
       << "Problem Solving : " << review.problemSolvingRating << std::endl
       << "Creativity Rating : " << review.creativityRating << std::endl
       << "Adaptibility Rating : " << review.adaptabilityRating << std::endl
       << "Leadership Rating : " << review.leadershipRating << std::endl
       << "Initiative Rating : " << review.initiativeRating << std::endl;
    if (review.comments) {
        os << "Comments : " << std::endl << review.comments.value_or("") << std::endl;
    }
    return os;
}

} // namespace PerfMgmt
#endif // MODELS_HPP
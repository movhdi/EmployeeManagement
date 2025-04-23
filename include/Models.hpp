#ifndef MODELS_HPP
#define MODELS_HPP
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

namespace PerfMgmt {

enum class Role {
    Manager,
    Boss,
    Specialist,
    technician
};

inline std::string roleToString(Role role) {
    switch (role) {
    case Role::Manager:
        return "Manager";
        break;
    case Role::Boss:
        return "Boss";
        break;
    case Role::Specialist:
        return "Specialist";
        break;
    case Role::technician:
        return "technician";
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
        return Role::Manager;
    } else if (roleStr == "Boss") {
        return Role::Boss;
    } else if (roleStr == "Specialist") {
        return Role::Specialist;
    } else if (roleStr == "technician") {
        return Role::technician;
    } else {
        return std::nullopt;
    }
}

struct Employee {
    int EmployeeId{0};
    int personnelCode{};
    std::string Name{};
    std::string HireDate{};
    Role role = Role::Specialist; // Default specialist
    bool isActive{true};
    std::optional<int> ReportsTo{std::nullopt};

public:
    // Constructor
    Employee(int id, int personnelCode, const std::string& name, const std::string& hireDate, Role role, bool isActive,
             std::optional<int> rt) :
        EmployeeId{id},
        personnelCode{personnelCode},
        Name{name},
        HireDate{hireDate},
        role{role},
        isActive{isActive},
        ReportsTo{rt} {
    }
    // Default Constructor
    Employee() {
    }
    ~Employee() {
    }
};

inline std::ostream& operator<<(std::ostream& os, const Employee& emp) {
    os << "------------------" << std::endl;
    os << "EmployeeId: " << emp.EmployeeId << std::endl
       << "Personnel Code: " << emp.personnelCode << std::endl
       << "Name: " << emp.Name << std::endl
       << "HireDate: " << emp.HireDate << std::endl
       << "Role: " << emp.role << std::endl
       << "isActive: " << emp.isActive << std::endl //(emp.isActive ? "true" : "false") << std::endl
       << "ReportsTo: " << (emp.ReportsTo ? std::to_string(*emp.ReportsTo) : "null") << std::endl;
    return os;
}

struct performanceReview {
    int ReviewId{0};
    int EmployeeId{0};
    int ReviewerId{0};
    std::string reviewDate;
    std::optional<float> OverallRating{std::nullopt};

    float PunctualityRating{0.0};
    float QualityOfWorkRating{0.0};
    float CommunicationRating{0.0};
    float TeamworkRating{0.0};
    float ProblemSolvingRating{0.0};
    float CreativityRating{0.0};
    float AdaptabilityRating{0.0};
    float LeadershipRating{0.0};
    float InitiativeRating{0.0};
    std::string Comments;

    // constructor
    performanceReview(int ReviewId, int EmployeeId, int ReiviewerId, const std::string& reviewDate,
                      std::optional<float> OveralRating, float PunctualityRating, float QualityOfWorkRating,
                      float CommunicatioRating, float TeamworkRating, float ProblemSolvingRating,
                      float CreativityRating, float AdaptabilityRating, float LeadershipRating, float InitiativeRating,
                      const std::string& Comments) :
        ReviewId{ReviewId},
        EmployeeId{EmployeeId},
        ReviewerId{ReviewerId},
        reviewDate{reviewDate},
        OverallRating{OveralRating},
        PunctualityRating{PunctualityRating},
        QualityOfWorkRating{QualityOfWorkRating},
        CommunicationRating{CommunicationRating},
        TeamworkRating{TeamworkRating},
        ProblemSolvingRating{ProblemSolvingRating},
        CreativityRating{CreativityRating},
        AdaptabilityRating{AdaptabilityRating},
        LeadershipRating{LeadershipRating},
        InitiativeRating{InitiativeRating},
        Comments{Comments} {
    }
};

} // namespace PerfMgmt
#endif // MODELS_HPP
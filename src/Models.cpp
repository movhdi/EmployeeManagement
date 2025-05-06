#include <Models.hpp>

namespace PerfMgmt {
void to_json(json& j, const Employee& employee) {
    j = json{{"employeeId", employee.employeeId}, {"name", employee.name},
             {"hireDate", employee.hireDate},     {"personnelCode", employee.personnelCode},
             {"isActive", employee.isActive},     {"role", roleToString(employee.role)}};
    if (employee.reportsTo) {
        j["reportsTo"] = employee.reportsTo.value();
    }
}
void from_json(const json& j, Employee& employee) {
    employee.employeeId = j.at("employeeId").get<int>();
    employee.hireDate = j.at("hireDate").get<std::string>();
    employee.name = j.at("name").get<std::string>();
    employee.personnelCode = j.at("personnelCode").get<int>();
    employee.role = stringToRole(j.at("role").get<std::string>()).value();
    if (j.contains("reportsTo") and !j["reportsTo"].is_null()) {
        employee.reportsTo = j.at("reportsTo").get<int>();
    }
    employee.isActive = j.at("isActive").get<bool>();
    // if (j.contains("team")) {
    //     employee.team = stringToFunctionTeams(j.at("team")).value();
    // }
}
} // namespace PerfMgmt
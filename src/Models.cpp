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
    employee.employeeId = j.at("employeeId");
    employee.hireDate = j.at("hireDate");
    employee.name = j.at("name");
    employee.personnelCode = j.at("personnelCode");
    employee.role = stringToRole(j.at("role")).value();
    if (j.contains("reportsTo")) {
        employee.reportsTo = j.at("reportsTo");
    }
}
} // namespace PerfMgmt
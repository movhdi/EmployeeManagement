#include "Models.hpp"
#include <QJsonObject>
#include <QJsonValue>

namespace PerfMgmt {
QJsonObject employeeToJson(const Employee& employee)
{
   QJsonObject j;

   j.insert("employeeId", employee.employeeId);
   j.insert("name", QString::fromStdString(employee.name));
   j.insert("hireDate", QString::fromStdString(employee.hireDate));
   j.insert("personnelCode", employee.personnelCode);
   j.insert("isActive", employee.isActive);
   j.insert("role", QString::fromStdString(roleToString(employee.role)));

   if (employee.reportsTo.has_value())
   {
      j.insert("reportsTo", *employee.reportsTo);
   }

   // Example if you later use team
   // if (employee.team.has_value()) {
   //     j.insert("team", QString::fromStdString(functionTeamToString(employee.team.value())));
   // }

   return j;
}

Employee jsonToEmployee(const QJsonObject& j)
{
   Employee employee;

   employee.employeeId    = j.value("employeeId").toInt();
   employee.name          = j.value("name").toString().toStdString();
   employee.hireDate      = j.value("hireDate").toString().toStdString();
   employee.personnelCode = j.value("personnelCode").toInt();
   employee.isActive      = j.value("isActive").toBool();

   const auto roleStr = j.value("role").toString().toStdString();
   employee.role = stringToRole(roleStr).value_or(Role::UNKNOWN);

   if (j.contains("reportsTo") && !j.value("reportsTo").isNull())
   {
      employee.reportsTo = j.value("reportsTo").toInt();
   }

   // if (j.contains("team") && !j.value("team").isNull()) {
   //     employee.team = stringToFunctionTeams(j.value("team").toString().toStdString()).value();
   // }

   return employee;
}
} // namespace PerfMgmt

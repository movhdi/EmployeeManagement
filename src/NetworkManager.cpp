#include "NetworkManager.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

namespace PerfMgmt {
NetworkManager::NetworkManager(const std::string& baseUrl)
   : baseUrl(baseUrl), httpClient(baseUrl)
{
}

std::optional<std::vector<Employee> > NetworkManager::fetchAllEmployees()
{
   std::string           body;
   std::vector<Employee> resultVec;

   auto                  res = httpClient.Get("/api/employees", [&](const char* data, size_t data_length) {
         body.append(data, data_length);
         return true;
      });

   if (!res || res->status != 200)
   {
      qWarning() << "[fetchAllEmployees] HTTP error or empty response.";
      return std::nullopt;
   }

   QJsonParseError parseError;
   QJsonDocument   doc = QJsonDocument::fromJson(QByteArray::fromStdString(body), &parseError);

   if (parseError.error != QJsonParseError::NoError || !doc.isArray())
   {
      qWarning() << "[fetchAllEmployees] JSON parse error:" << parseError.errorString();
      return std::nullopt;
   }

   QJsonArray jsonArray = doc.array();
   for (const QJsonValue& val : jsonArray)
   {
      if (!val.isObject())
         continue;

      auto empOpt = parseEmployeeJson(val.toObject());
      if (empOpt.has_value())
      {
         resultVec.push_back(empOpt.value());
      }
   }

   for (const auto& item : resultVec)
   {
      std::cout << item << "\n\n";
   }

   return resultVec;
}

std::optional<Employee> NetworkManager::parseEmployeeJson(const QJsonObject& json)
{
   try {
      Employee e;
      e.employeeId    = json["id"].toInt();
      e.name          = json["name"].toString().toStdString();
      e.role          = stringToRole(json["role"].toString().toStdString()).value_or(Role::Specialist);
      e.hireDate      = json["hireDate"].toString().toStdString();
      e.personnelCode = json["personnelCode"].toInt();
      e.isActive      = json["isActive"].toBool();

      if (json.contains("reportsTo") && !json["reportsTo"].isNull())
      {
         e.reportsTo = json["reportsTo"].toInt();
      }

      return e;
   } catch (...) {
      qWarning() << "[parseEmployeeJson] Failed to parse Employee JSON.";
      return std::nullopt;
   }
}

std::optional<httplib::Result> NetworkManager::makeGetRequest(const std::string& path)
{
   auto res = httpClient.Get(path.c_str());

   if (!res || res->status != 200)
   {
      qWarning() << "[makeGetRequest] Failed GET:" << path.c_str();
      return std::nullopt;
   }
   return res;
}

std::optional<httplib::Result> NetworkManager::makePostRequest(const std::string& path, const std::string& body,
                                                               const std::string& contentType)
{
   auto res = httpClient.Post(path.c_str(), body, contentType.c_str());

   if (!res || res->status != 200)
   {
      qWarning() << "[makePostRequest] Failed POST:" << path.c_str();
      return std::nullopt;
   }
   return res;
}

bool NetworkManager::isServerReachable()
{
   auto res = httpClient.Get("/api/ping");

   return res && res->status == 200;
}
} // namespace PerfMgmt

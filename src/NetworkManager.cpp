#include "NetworkManager.hpp"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QDebug>
#include <QUrl>

namespace PerfMgmt {
NetworkManager::NetworkManager(const std::string& baseUrl)
   : baseUrl(QString::fromStdString(baseUrl))
{
}

std::optional<QByteArray> NetworkManager::makeGetRequest(const QString& path)
{
   QNetworkRequest request(QUrl(baseUrl + path));
   QNetworkReply   * reply = networkManager.get(request);

   QEventLoop      loop;

   QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   loop.exec();

   if (reply->error() != QNetworkReply::NoError)
   {
      qWarning() << "[makeGetRequest] Failed GET:" << path << reply->errorString();
      reply->deleteLater();
      return std::nullopt;
   }

   QByteArray data = reply->readAll();
   reply->deleteLater();
   return data;
}

std::optional<QByteArray> NetworkManager::makePostRequest(const QString& path, const QByteArray& body, const QString& contentType)
{
   QNetworkRequest request(QUrl(baseUrl + path));

   request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);

   QNetworkReply* reply = networkManager.post(request, body);

   QEventLoop   loop;
   QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
   loop.exec();

   if (reply->error() != QNetworkReply::NoError)
   {
      qWarning() << "[makePostRequest] Failed POST:" << path << reply->errorString();
      reply->deleteLater();
      return std::nullopt;
   }

   QByteArray data = reply->readAll();
   reply->deleteLater();
   return data;
}

bool NetworkManager::isServerReachable()
{
   auto result = makeGetRequest("/api/ping");

   return result.has_value();
}

std::optional<std::vector<Employee> > NetworkManager::fetchAllEmployees()
{
   auto resData = makeGetRequest("/api/employees");

   if (!resData.has_value())
      return std::nullopt;

   QJsonParseError parseError;
   QJsonDocument   doc = QJsonDocument::fromJson(resData.value(), &parseError);
   if (parseError.error != QJsonParseError::NoError || !doc.isArray())
   {
      qWarning() << "[fetchAllEmployees] JSON parse error:" << parseError.errorString();
      return std::nullopt;
   }

   std::vector<Employee> resultVec;
   QJsonArray            jsonArray = doc.array();
   for (const QJsonValue& val : jsonArray)
   {
      if (!val.isObject())
         continue;

      auto empOpt = parseEmployeeJson(val.toObject());
      if (empOpt.has_value())
         resultVec.push_back(empOpt.value());
   }

   return resultVec;
}

std::optional<Employee> NetworkManager::parseEmployeeJson(const QJsonObject& json)
{
   try {
      Employee e;
      e.employeeId    = json["id"].toInt();
      e.name          = json["name"].toString().toStdString();
      e.role          = stringToRole(json["role"].toString().toStdString()).value_or(Role::SPECIALIST);
      e.hireDate      = json["hireDate"].toString().toStdString();
      e.personnelCode = json["personnelCode"].toInt();
      e.isActive      = json["isActive"].toBool();

      if (json.contains("reportsTo") && !json["reportsTo"].isNull())
         e.reportsTo = json["reportsTo"].toInt();

      return e;
   } catch (...) {
      qWarning() << "[parseEmployeeJson] Failed to parse Employee JSON.";
      return std::nullopt;
   }
}
} // namespace PerfMgmt

#ifndef NETWORKMANAGERHPP
#define NETWORKMANAGERHPP

#include <Models.hpp>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <optional>
#include <string>
#include <vector>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

namespace PerfMgmt {
class NetworkManager {
public:
NetworkManager() = delete;
explicit NetworkManager(const std::string& serverBaseUrl);

bool isServerReachable();

std::optional<std::vector<Employee> > fetchAllEmployees();
std::optional<Employee> fetchSingleEmployee(int employeeId);
std::optional<int> sendNewEmployee(const Employee& employee);
bool sendUpdatedEmployee(int serverEmployeeId, const Employee& employee);

std::optional<std::vector<PerformanceReview> > fetchReviewsForEmployee(int serverEmployeeId);
std::optional<int> sendNewReview(int serverEmployeeId, const PerformanceReview& review);
bool updateReview(int serverReviewId, const PerformanceReview& review);

~NetworkManager() = default;

private:
QString baseUrl;
QNetworkAccessManager networkManager;

std::optional<Employee> parseEmployeeJson(const QJsonObject& jEmp);
std::optional<PerformanceReview> parsePerformanceReview(const QJsonObject& jPerf);

std::optional<QByteArray> makeGetRequest(const QString& path);
std::optional<QByteArray> makePostRequest(const QString& path, const QByteArray& body,
                                          const QString& contentType);
};
} // namespace PerfMgmt

#endif // NETWORKMANAGERHPP

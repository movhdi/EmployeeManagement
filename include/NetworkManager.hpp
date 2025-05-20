#ifndef NETWORKMANAGERHPP
#define NETWORKMANAGERHPP

#include <Models.hpp>
#include <httplib.h>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <optional>
#include <string>
#include <vector>

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
std::string baseUrl;
httplib::Client httpClient;

std::optional<Employee> parseEmployeeJson(const QJsonObject& jEmp);
std::optional<PerformanceReview> parsePerformanceReview(const QJsonObject& jPerf);

std::optional<httplib::Result> makeGetRequest(const std::string& path);
std::optional<httplib::Result> makePostRequest(const std::string& path, const std::string& body,
                                               const std::string& conentType);
};
} // namespace PerfMgmt

#endif // NETWORKMANAGERHPP

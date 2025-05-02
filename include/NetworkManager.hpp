#ifndef NETWORKMANAGERHPP
#define NETWORKMANAGERHPP
#include <Models.hpp>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

using json = nlohmann::json;

namespace PerfMgmt {

class NetworkManager {
public:
    // httpllib::Client does not have default constructor
    NetworkManager() = delete;
    // Constructor, takes the server base URL of server API
    explicit NetworkManager(const std::string& serverBaseUrl);

    // check if server reachable
    bool isServerReachable();

    // ----- Employee API Calls -----

    // Fetch all employees from the server
    std::optional<std::vector<Employee>> fetchAllEmployees();

    // Fetch a single employee by server Id
    std::optional<Employee> fetchSingleEmployee(int employeeId);

    // send a new employee to the server
    // returns the server assigned Id if successful
    std::optional<int> sendNewEmployee(const Employee& employee);

    // Update an existing employee on the server
    bool sendUpdatedEmployee(int serverEmployeeId, const Employee& employee);

    // ----- Review API Calls -----

    // Fetch reviews for a specific employee from the server
    std::optional<std::vector<PerformanceReview>> fetchReviewsForEmployee(int serverEmployeeId);

    // send a new review to the server
    std::optional<int> sendNewReview(int serverEmployeeId, const PerformanceReview& review);

    // update an existing review to the server
    bool updateReview(int serverReviewId, const PerformanceReview& review);
    ~NetworkManager() {};

private:
    std::string baseUrl;
    httplib::Client httpClient;

    // Helper function to parse JSON responses to Employee and PerformanceReview model
    std::optional<Employee> parseEmployeeJson(const json& jEmp);
    std::optional<PerformanceReview> parsePerformanceReview(const json& jPerf);

    // Helper function to handle common request/error logic
    std::optional<httplib::Result> makeGetRequest(const std::string& path);
    std::optional<httplib::Result> makePostRequest(const std::string& path, const std::string& body,
                                                   const std::string& conentType);

    // Helper for PUT, DELETE

protected:
};

} // namespace PerfMgmt
#endif
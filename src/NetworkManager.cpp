#include <NetworkManager.hpp>
#include <string>

using json = nlohmann::json;

namespace PerfMgmt {
NetworkManager::NetworkManager(const std::string& baseUrl) : httpClient(baseUrl) {
}
std::optional<std::vector<Employee>> NetworkManager::fetchAllEmployees() {

    std::string body;
    std::vector<Employee> resutlVec;
    auto res = httpClient.Get("/api/employees", [&](const char* data, size_t data_length) {
        body.append(data, data_length);
        return true;
    });

    try {
        json j = json::parse(body);
        // std::cout << std::setw(4) << j << "\n\n";
        for (const auto& jsonItem : j) {
            // resutlVec.emplace_back(Employee(static_cast<int>(jsonItem["id"]), 2025121, std::string(jsonItem["name"]),
            //                                 std::string(jsonItem["hireDate"]),
            //                                 stringToRole(std::string(jsonItem["role"])).value(),
            //                                 jsonItem["isActive"], std::nullopt));
            Employee empTemp;
            from_json(jsonItem, empTemp);
            resutlVec.emplace_back(empTemp);
        }
    } catch (const std::exception& e) {
        std::cerr << "[fetchAllEmployees] :" << e.what() << '\n';
        return std::nullopt;
    }
    for (const auto& item : resutlVec) {
        std::cout << item << "\n\n";
    }
    return resutlVec;
}
} // namespace PerfMgmt
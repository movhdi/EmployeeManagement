#include <NetworkManager.hpp>

namespace PerfMgmt {
NetworkManager::NetworkManager(const std::string& baseUrl) : httpClient(baseUrl) {
}
} // namespace PerfMgmt
#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) : search_server_(search_server) { }

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    const auto result = search_server_.FindTopDocuments(raw_query, status);
    AddRequest(result.size());
    return result;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    const auto result = search_server_.FindTopDocuments(raw_query);
    AddRequest(result.size());
    return result;
}

int RequestQueue::GetNoResultRequests() const {
    return no_results_requests_;
}

void RequestQueue::AddRequest(int result_size) {
    ++current_time_;
    requests_.push_back({ current_time_, result_size });
    if (result_size == 0) { ++no_results_requests_; }
    if (current_time_ > min_in_day_) {
        requests_.pop_front();
        if (requests_.front().results == 0) {
            --no_results_requests_;
        }
    }
}
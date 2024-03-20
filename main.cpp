#include "document.h"
#include "paginator.h"
#include "read_input_functions.h"
#include "string_processing.h"
#include "search_server.h"
#include "request_queue.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    SearchServer search_server("and in at"s);
    RequestQueue request_queue(search_server);

    search_server.AddDocument(1, "Blackwood cat curly tail"s, DocumentStatus::BANNED, { 7, 2, 7 });
    search_server.AddDocument(2, "giant dog and fancy filled"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
    search_server.AddDocument(3, "big filled Blackwood collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
    search_server.AddDocument(4, "Lord giant Blackwood"s, DocumentStatus::BANNED, { 1, 3, 2 });
    search_server.AddDocument(5, "big dog filled Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    search_server.AddDocument(6, "Blackwood into a mysterious and dangerous world"s, DocumentStatus::BANNED, { 7, 2, 7 });
    search_server.AddDocument(7, "filled with giant Blackwood"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
    search_server.AddDocument(8, "Lord giant Blackwood "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
    search_server.AddDocument(9, "treaty Blackwood to giant and believes"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
    search_server.AddDocument(10, "The story giant"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
    search_server.AddDocument(11, "Blackwood Lucien Thornbear 2"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
    search_server.AddDocument(12, "Prince Lucien Thornbear"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
    search_server.AddDocument(13, "Prince Lucien Thornbear"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

    auto result = search_server.FindTopDocuments("filled with giant Blackwood");
    for (auto& r : result) {
        cout << r << endl;
    }
    cout << "-------------------" << endl;
    auto result2 = search_server.FindTopDocuments("filled with giant Blackwood Eugene", DocumentStatus::BANNED);
    for (auto& r : result2) {
        cout << r << endl;
    }
    cout << "-------------------" << endl;
    auto result3 = search_server.FindTopDocuments("filled with giant Blackwood Eugene",
        [](int document_id, DocumentStatus document_status, int rating) {
            return rating > 2; });
    for (auto& r : result3) {
        cout << r << endl;
    }
    return 0;
}
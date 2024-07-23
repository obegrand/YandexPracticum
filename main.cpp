#include <iostream>
#include <list>
#include <set>
#include <string_view>
#include <vector>

using namespace std;

template <typename InputIt1, typename InputIt2>
void Merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, std::ostream& out) {
    while (first1 != last1 && first2 != last2) {
        if (std::less<>()(*first1, *first2)) {
            out << *first1++ << std::endl;
        }
        else {
            out << *first2++ << std::endl;
        }
    }
    while (first1 != last1) {
        out << *first1++ << std::endl;
    }
    while (first2 != last2) {
        out << *first2++ << std::endl;
    }
}

template <typename Container1, typename Container2>
void MergeSomething(const Container1& src1, const Container2& src2, std::ostream& out) {
    Merge(std::cbegin(src1), std::cend(src1), std::cbegin(src2), std::cend(src2), out);
}

template <typename T>
void MergeHalves(const std::vector<T>& src, std::ostream& out) {
    size_t mid = (src.size() + 1) / 2;
    Merge(src.cbegin(), src.cbegin() + mid, src.cbegin() + mid, src.cend(), out);
}

int main() {
    vector<int> v1{ 60, 70, 80, 90 };
    vector<int> v2{ 65, 75, 85, 95 };
    vector<int> combined{ 60, 70, 80, 90, 65, 75, 85, 95 };
    list<double> my_list{ 0.1, 72.5, 82.11, 1e+30 };
    string_view my_string = "ACNZ"sv;
    set<unsigned> my_set{ 20u, 77u, 81u };

    // пока функция MergeSomething реализована пока только для векторов
    cout << "Merging vectors:"sv << endl;
    MergeSomething(v1, v2, cout);

    cout << "Merging vector and list:"sv << endl;
    MergeSomething(v1, my_list, cout);

    cout << "Merging string and list:"sv << endl;
    MergeSomething(my_string, my_list, cout);

    cout << "Merging set and vector:"sv << endl;
    MergeSomething(my_set, v2, cout);

    cout << "Merging vector halves:"sv << endl;
    MergeHalves(combined, cout);
}
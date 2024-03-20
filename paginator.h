#pragma once
#include <vector>

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator pages_begin, Iterator pages_end, size_t page_size) {
        for (size_t left = distance(pages_begin, pages_end); left > 0;) {
            const size_t current_page_size = std::min(page_size, left);
            const Iterator current_pages_end = next(pages_begin, current_page_size);
            pages_.push_back({ pages_begin, current_pages_end });
            left -= current_page_size;
            pages_begin = current_pages_end;
        }
    }

    auto begin() const {
        return pages_.begin();
    }

    auto end() const {
        return pages_.end();
    }
private:
    std::vector<std::pair<Iterator, Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const std::pair<Iterator, Iterator>& range) {
    for (Iterator it = range.first; it != range.second; ++it) {
        out << *it;
    }
    return out;
}
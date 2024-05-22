#include <cassert>

#include "bimap.h"

int main() {
    using namespace std::literals;

    BiMap bimap;
    {
        const bool cat_added = bimap.Add("Cat"sv, "Koshka"sv);

        assert(cat_added);

        assert(bimap.FindValue("Cat"sv) == "Koshka"sv);
        assert(bimap.FindKey("Koshka"sv) == "Cat"sv);
        assert(!bimap.FindKey("Cat"sv));
        assert(!bimap.FindValue("Koshka"sv));

        const bool kitty_added = bimap.Add("Cat"sv, "Kitty"sv);

        assert(!kitty_added);
        assert(bimap.FindValue("Cat"sv) == "Koshka"sv);
        assert(!bimap.FindValue("Kitty"sv));
    }

    BiMap bimap_copy(bimap);
    assert(bimap_copy.FindValue("Cat"sv) == "Koshka"sv);
    assert(bimap_copy.FindKey("Koshka"sv) == "Cat"sv);

    bimap_copy.Add("Dog"sv, "Sobaka"sv);
    assert(bimap_copy.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap_copy.FindKey("Sobaka"sv) == "Dog"sv);
    assert(!bimap.FindKey("Sobaka"sv));
    assert(!bimap.FindValue("Dog"sv));

    bimap = std::move(bimap_copy);
    assert(bimap.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap.FindKey("Sobaka"sv) == "Dog"sv);

    BiMap moved_bimap(std::move(bimap));
    assert(moved_bimap.FindValue("Dog"sv) == "Sobaka"sv);
    assert(moved_bimap.FindKey("Sobaka"sv) == "Dog"sv);

    BiMap bimap_to_assign;
    bimap_to_assign = moved_bimap;
    assert(bimap_to_assign.FindValue("Dog"sv) == "Sobaka"sv);
    assert(bimap_to_assign.FindKey("Sobaka"sv) == "Dog"sv);
}
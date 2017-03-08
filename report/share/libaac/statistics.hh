#ifndef AAC_STATISTICS_HH
#define AAC_STATISTICS_HH

#include "aac/configs.hh"
#include <cstddef>

namespace aac {
    class Statistics final {
    public:
        void clear(Counter*);
        Statistics() { clear(nullptr); }
        Statistics(Counter* bootstrap) {
            clear(bootstrap);
        }

        Counter total() const;
        Counter find(Counter) const;
        Interval symbol(std::size_t) const;
        void update(std::size_t);
        bool frozen() const;

    private:
        // Cumulative frequency counts for
        // the symbols in the alphabet+EOF.
        Counter frequencies[SYMBOLS] {  };
        bool frequencies_frozen { false };
    };
}

#endif

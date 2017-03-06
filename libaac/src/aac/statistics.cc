#include "aac/statistics.hh"

void aac::Statistics::clear(Counter* bootstrap) {
    if (bootstrap == nullptr) {
        for (std::size_t i { 0 }; i < SYMBOLS; ++i)
            frequencies[i] = (i + 1); // Cumu freq.
    } else {
        frequencies[0] = bootstrap[0];
        // Load in existing statistics on the data.
        for (std::size_t i { 1 }; i < SYMBOLS; ++i)
            frequencies[i] = bootstrap[i] + frequencies[i - 1];
        frequencies_frozen = true;
    }
}

aac::Counter aac::Statistics::total() const {
    return frequencies[END_OF_FILE];
}

aac::Interval aac::Statistics::symbol(std::size_t symbol) const {
    if (symbol == 0) return aac::Interval { 0.0, frequencies[0] };
    return aac::Interval { frequencies[symbol - 1],
                           frequencies[symbol] };
}

void aac::Statistics::update(std::size_t symbol) {
    if (frequencies_frozen) return;
    // Update the cumulative frquencies.
    for (; symbol < SYMBOLS; ++symbol) {
        if (++frequencies[symbol] >= COUNT_BOUND)
            frequencies_frozen = true;
    }
}

bool aac::Statistics::frozen() const {
    return frequencies_frozen;
}

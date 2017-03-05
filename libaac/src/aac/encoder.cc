#include "aac/encoder.hh"

std::size_t aac::Encoder::step(Symbol symbol, Byte* buffer,
                               std::size_t coming_bit) {
    Range range = upper - lower + 1;
    Interval interval { statistics.symbol(symbol) };
    Counter denominator { statistics.total() };
    statistics.update(symbol);

    upper = lower + (range * interval.second / denominator) - 1;
    lower = lower + (range * interval.first  / denominator);

    std::size_t bits { 0 };
    while (true) {
        if (upper < TWO_FOURTHS) {
            bits += write(0, buffer,
                 coming_bit + bits);
        } else if (lower >= TWO_FOURTHS) {
            bits += write(1, buffer,
                 coming_bit + bits);
        } else if (lower   >= ONE_FOURTHS
                   && upper < TRE_FOURTHS) {
            ++pending_results;
            lower -= ONE_FOURTHS;
            upper -= ONE_FOURTHS;
        } else break;

        upper = upper << 1;
        lower = lower << 1;
        upper += 1;
        upper &= UPPER_BOUND;
        lower &= UPPER_BOUND;
    }

    return bits;
}

std::size_t aac::Encoder::exit(Byte* buffer, std::size_t coming_bit) {
    ++pending_results;
    std::size_t bits { 0 };
    if (lower < ONE_FOURTHS)
        bits += write(1, buffer,
             coming_bit + bits);
    else bits += write(0, buffer,
              coming_bit + bits);
    return bits;
}

std::size_t aac::Encoder::write(bool state, Byte* buffer,
                                std::size_t coming_bit) {
    std::size_t bits_written { 1 };
    std::size_t byte { coming_bit >> 3 };
    std::size_t bit { coming_bit - byte*8 };

    Byte data { buffer[byte] };
    data = data &  ~(1 << bit);
    data = data | state << bit;
    buffer[byte] = data;

    ++bits_written;
    ++coming_bit;

    while (pending_results--) {
        byte = coming_bit >> 3;
        bit = coming_bit - byte*8;

        data = buffer[byte];
        data = data &  ~(1 << bit);
        data = data | !state << bit;
        buffer[byte] = data;

        ++bits_written;
        ++coming_bit;
    } pending_results = 0;
    return bits_written-1;
}

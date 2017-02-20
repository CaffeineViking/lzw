#include "lzw/dictionary.hh"

void lzw::Dictionary::initialize() {
    // We cheat a bit here and don't construct the tree.
    for (std::size_t i { 0 };  i < ALPHABET_SIZE; ++i) {
        heads[i] = static_cast<Byte>(i);
        prefixes[i] = EMPTY_STRING;
        rights[i] = EMPTY_STRING;
        lefts[i] = EMPTY_STRING;
        roots[i] = EMPTY_STRING;
    }

    heads[EMPTY_STRING] = 0;
    prefixes[EMPTY_STRING] = EMPTY_STRING;
    rights[EMPTY_STRING] = EMPTY_STRING;
    lefts[EMPTY_STRING] = EMPTY_STRING;
    roots[EMPTY_STRING] = EMPTY_STRING;
}

void lzw::Dictionary::reset() {
    head = ALPHABET_SIZE;
    initialize();
}

lzw::Index lzw::Dictionary::insert(lzw::Index prefix, lzw::Byte head) {
    heads[this->head] = head;
    prefixes[this->head] = prefix;
    rights[this->head] = EMPTY_STRING;
    lefts[this->head] = EMPTY_STRING;
    roots[this->head] = EMPTY_STRING;
    return (this->head)++;
}

lzw::Byte* lzw::Dictionary::traverse(lzw::Byte* buffer, Index index) const {
    lzw::Index current { index };
    while (current != EMPTY_STRING) {
        *(--buffer) = heads[current];
        current  = prefixes[current];
    }

    return buffer;
}

lzw::Index lzw::Dictionary::search(lzw::Index prefix, lzw::Byte head) {
    if (prefix == EMPTY_STRING) return static_cast<Index>(head);
    Index entry { EMPTY_STRING }; // Either inserted or existing entries.
    if (roots[prefix] == EMPTY_STRING) entry = insert_root(prefix, head);
    else entry = insert_into_tree(prefix, head, roots[prefix]);
    return entry;
}

lzw::Index lzw::Dictionary::insert_root(lzw::Index prefix, lzw::Byte head) {
    Index entry { insert(prefix, head) };
    roots[prefix] = entry;
    return entry;
}

lzw::Index lzw::Dictionary::insert_into_tree(lzw::Index prefix, lzw::Byte head, Index root) {
    lzw::Index current { root };
    lzw::Index previous { root };
    while (current != EMPTY_STRING) {
        previous = current; // Needed in order to link tree.
        if (head < heads[current]) current = lefts[current];
        else if (head > heads[current]) current = rights[current];
        else return current;
    }

    Index entry { insert(prefix, head) };
    // Below we link the new entry to the binary tree.
    if (head < heads[previous]) lefts[previous] = entry;
    else rights[previous] = entry;
    return entry;
}

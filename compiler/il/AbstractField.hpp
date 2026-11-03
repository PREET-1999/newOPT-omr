#ifndef ABSTRACT_FIELD_HPP
#define ABSTRACT_FIELD_HPP

#include <string>

// A field identity that is stable across compilations (replaces TR::SymbolReference*).
// Shared by the working PTG and MethodSummary.
class AbstractField {
public:
    int fieldId; // e.g. "A.f"; "[]" for array elements

    AbstractField(int id) : fieldId(id) {}

    bool operator<(const AbstractField &other) const;
    bool operator==(const AbstractField &other) const;

    std::string toString() const;
};

#endif // ABSTRACT_FIELD_HPP
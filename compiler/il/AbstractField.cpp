#include "AbstractField.hpp"

bool AbstractField::operator<(const AbstractField &other) const { return fieldId < other.fieldId; }

bool AbstractField::operator==(const AbstractField &other) const { return fieldId == other.fieldId; }

std::string AbstractField::toString() const { return "field#" + std::to_string(fieldId); }

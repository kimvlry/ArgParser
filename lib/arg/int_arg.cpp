#include "int_arg.h"

IntArg::IntArg() {
  type_ = 'i';
}

int IntArg::Value() const {
  return value_;
}

const std::vector<int>& IntArg::ValueVector() const {
  return value_vector_;
}

void IntArg::SetValue(int val) {
  value_ = val;
  if (value_location_ != nullptr) {
    *value_location_ = value_;
  }
}

void IntArg::SetMultiValue(int val) {
  value_vector_.push_back(val);
  ++multivalues_given_;
  if (value_vector_location_ != nullptr) {
    *value_vector_location_ = value_vector_;
  }
}

IntArg& IntArg::StoreValue(int& location) {
  value_location_ = &location;
  return *this;
}

IntArg& IntArg::StoreValues(std::vector<int>& location) {
  value_vector_location_ = &location;
  return *this;
}

IntArg& IntArg::MultiValue() {
  is_multivalue_ = true;
  return *this;
}

IntArg& IntArg::MultiValue(int minimum_required) {
  is_multivalue_ = true;
  multivalue_min_required_ = minimum_required;
  return *this;
}

IntArg& IntArg::Positional() {
  is_positional_ = true;
  return *this;
}

#include "string_arg.h"

StrArg::StrArg() {
  type_ = 's';
}

const std::string& StrArg::Value() const {
  return value_;
}

const std::vector<std::string>& StrArg::ValueVector() const {
  return value_vector_;
}

void StrArg::SetValue(const std::string& val) {
  value_ = val;
  if (value_location_ != nullptr) {
    *value_location_ = value_;
  }
}

void StrArg::SetMultiValue(const std::string& val) {
  value_vector_.push_back(val);
  ++multivalues_given_;
  if (value_vector_location_ != nullptr) {
    *value_vector_location_ = value_vector_;
  }
}

StrArg& StrArg::Default(const char* string) {
  is_default_ = true;
  value_ = string;
  return *this;
}

StrArg& StrArg::StoreValue(std::string& location) {
  value_location_ = &location;
  return *this;
}

StrArg& StrArg::StoreValues(std::vector<std::string>& location) {
  value_vector_location_ = &location;
  return *this;
}

StrArg& StrArg::MultiValue() {
  is_multivalue_ = true;
  return *this;
}

StrArg& StrArg::MultiValue(int minimum_required) {
  is_multivalue_ = true;
  multivalue_min_required_ = minimum_required;
  return *this;
}

StrArg& StrArg::Positional() {
  is_positional_ = true;
  return *this;
}

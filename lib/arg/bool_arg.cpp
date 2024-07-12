#include "bool_arg.h"

void BoolArg::SetTrueFlag() {
  is_default_ = true;
  if (value_location_) {
    *value_location_ = true;
  }
  value_ = true;
}

BoolArg::BoolArg() {
  type_ = 'b';
}

bool BoolArg::GetlValue() const {
  return value_;
}

BoolArg& BoolArg::Default(bool val) {
  value_ = val;
  is_default_ = true;
  return *this;
}

BoolArg& BoolArg::StoreValue(bool& location) {
  value_location_ = &location;
  return *this;
}

BoolArg& BoolArg::MultiValue() {
  is_multivalue_ = true;
  return *this;
}

BoolArg& BoolArg::Positional() {
  is_positional_ = true;
  return *this;
}

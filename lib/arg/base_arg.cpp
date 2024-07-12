#include "base_arg.h"

char BaseArg::Type() const {
  return type_;
}

bool BaseArg::IsMultiValue() const {
  return is_multivalue_;
}

bool BaseArg::IsPositional() const {
  return is_positional_;
}

int BaseArg::MultiValueArgMinimum() const {
  return multivalue_min_required_;
}

int BaseArg::GivenMultiValuesCounter() const {
  return multivalues_given_;
}

void BaseArg::MarkAsDefault() {
  is_default_ = true;
}

void BaseArg::MarkAsParsed() {
  is_parsed_ = true;
}

void BaseArg::ResetGivenMultivaluesCounter() {
  multivalues_given_ = 0;
}

bool BaseArg::IsValid() const {
  return is_default_ + is_parsed_;
}

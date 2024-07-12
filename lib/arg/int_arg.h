#pragma once
#include "base_arg.h"

class IntArg : public BaseArg {
  friend class argparser::ArgParser;

public:
  IntArg();
  ~IntArg() override = default;

  int Value() const;
  const std::vector<int>& ValueVector() const;

  IntArg& StoreValue(int& location);
  IntArg& StoreValues(std::vector<int>& location);
  IntArg& MultiValue();
  IntArg& MultiValue(int minimum_required);
  IntArg& Positional();

private:
  int value_;
  int* value_location_ = nullptr;
  std::vector<int> value_vector_;
  std::vector<int>* value_vector_location_ = nullptr;

  void SetValue(int val);
  void SetMultiValue(int val);
};

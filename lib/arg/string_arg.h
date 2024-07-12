#pragma once
#include "base_arg.h"

class StrArg : public BaseArg {
  friend class argparser::ArgParser;

public:
  StrArg();
  ~StrArg() override = default;

  const std::string& Value() const;
  const std::vector<std::string>& ValueVector() const;

  StrArg& Default(const char* string);
  StrArg& StoreValue(std::string& location);
  StrArg& StoreValues(std::vector<std::string>& location);
  StrArg& MultiValue();
  StrArg& MultiValue(int minimum_required);
  StrArg& Positional();

private:
  std::string value_;
  std::string* value_location_ = nullptr;
  std::vector<std::string> value_vector_;
  std::vector<std::string>* value_vector_location_ = nullptr;

  void SetValue(const std::string& val);
  void SetMultiValue(const std::string& val);
};

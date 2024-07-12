#pragma once
#include "base_arg.h"

class BoolArg : public BaseArg {
  friend class argparser::ArgParser;

public:
  BoolArg();
  ~BoolArg() override = default;

  bool GetlValue() const;

  BoolArg& Default(bool val);
  BoolArg& StoreValue(bool& location);
  BoolArg& MultiValue();
  BoolArg& Positional();

private:
  bool value_;
  bool* value_location_ = nullptr;

  void SetTrueFlag();
};

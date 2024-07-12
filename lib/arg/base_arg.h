#pragma once
#include <string>
#include <vector>

namespace argparser {
class ArgParser;
}

class BaseArg {
  friend class argparser::ArgParser;

public:
  virtual ~BaseArg() = default;

  char Type() const;
  bool IsMultiValue() const;
  bool IsPositional() const;
  int MultiValueArgMinimum() const;
  int GivenMultiValuesCounter() const;
  bool IsValid() const;

protected:
  char type_;
  bool is_parsed_ = false;
  bool is_default_ = false;
  bool is_multivalue_ = false;
  bool is_positional_ = false;
  int multivalue_min_required_ = 0;
  int multivalues_given_ = 0;

private:
  void MarkAsDefault();
  void MarkAsParsed();
  void ResetGivenMultivaluesCounter();
};

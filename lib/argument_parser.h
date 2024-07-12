#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include "arg/base_arg.h"
#include "arg/int_arg.h"
#include "arg/bool_arg.h"
#include "arg/string_arg.h"

namespace argparser {
struct WhileParsing {
  char short_;
  std::string full_;
  std::string value_;
  std::vector<char> flags_;
  std::set<std::string> multivalue_args_to_check_;
  std::vector<int> positional_values_;
  bool catched_positional_ = false;
};

class ArgParser {
public:
  explicit ArgParser(std::string name);

  std::unordered_map<std::string, BaseArg*>& FullKeyAccess();
  [[nodiscard]] const std::string& ProgramName() const;
  [[nodiscard]] const std::string& ProgramDescription() const;
  [[nodiscard]] const std::string& HelpCommandDescription() const;
  [[nodiscard]] int ArgsAmount() const;

  bool RegisterValue(bool is_multivalue);
  bool CheckPositionals();
  bool CheckMultivalueArgs();
  bool ValidateArgs();
  bool Parse(std::vector<std::string> input);
  bool Parse(int argc, char** argv);

  BaseArg& AddHelp(char shortkey, const char* fullkey, const char* description);
  [[nodiscard]] bool Help() const;
  [[nodiscard]] std::string ShowHelpMsg() const;

  BaseArg& AddArg(BaseArg* arg,
                  char type,
                  char shortkey,
                  const char* fullkey,
                  const char* description);

  IntArg& AddIntArgument(const char* fullkey);
  IntArg& AddIntArgument(char shortkey, const char* fullkey);
  IntArg& AddIntArgument(const char* fullkey, const char* description);
  IntArg& AddIntArgument(char shortkey,
                         const char* fullkey,
                         const char* description);

  StrArg& AddStringArgument(const char* fullkey);
  StrArg& AddStringArgument(char shortkey, const char* fullkey);
  StrArg& AddStringArgument(const char* fullkey, const char* description);
  StrArg& AddStringArgument(char shortkey,
                            const char* fullkey,
                            const char* description);

  BoolArg& AddFlag(const char* fullkey);
  BoolArg& AddFlag(char shortkey, const char* fullkey);
  BoolArg& AddFlag(const char* fullkey, const char* description);
  BoolArg& AddFlag(char shortkey, const char* fullkey, const char* description);

  int GetIntValue(const char* fullkey);
  int GetIntValue(const char* fullkey, int ix);
  std::string GetStringValue(const char* fullkey);
  std::string GetStringValue(const char* fullkey, int ix);
  bool GetFlag(const char* fullkey);

private:
  std::string app_name_;
  std::string app_description_;
  std::string help_command_description_;
  std::vector<std::string> arguments_descriptions_;

  std::unordered_map<char, std::string> by_shortkey_;
  std::unordered_map<std::string, BaseArg*> by_fullkey_;

  WhileParsing data;

  void AddToHelpMsg(char shortkey,
                    const std::string& fullkey,
                    const std::string& description);
  void SetHelpCommandDescription(const std::string& description);
  void SetProgramDescription(const std::string& description);
  void CheckIfExists(const char* fullkey);
};
} // namespace argparser

bool IsANumber(const std::string& str);

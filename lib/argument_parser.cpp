#include "argument_parser.h"

argparser::ArgParser::ArgParser(std::string name) {
  app_name_ = std::move(name);
}

std::unordered_map<std::string, BaseArg*>&
argparser::ArgParser::FullKeyAccess() {
  return by_fullkey_;
}

const std::string& argparser::ArgParser::ProgramName() const {
  return app_name_;
}

const std::string& argparser::ArgParser::ProgramDescription() const {
  return app_description_;
}

const std::string& argparser::ArgParser::HelpCommandDescription() const {
  return help_command_description_;
}

void argparser::ArgParser::SetHelpCommandDescription(
  const std::string& description) {
  help_command_description_ = description;
}

void argparser::ArgParser::SetProgramDescription(
  const std::string& description) {
  app_description_ = description;
}

[[nodiscard]] int argparser::ArgParser::ArgsAmount() const {
  return by_fullkey_.size();
}

bool argparser::ArgParser::RegisterValue(bool is_multivalue) {
  if (by_fullkey_[data.full_]->Type() == 's') {
    if (is_multivalue) {
      dynamic_cast<StrArg*>(by_fullkey_[data.full_])->SetMultiValue(data.value_);
      data.multivalue_args_to_check_.insert(data.full_);
    }
    else {
      dynamic_cast<StrArg*>(by_fullkey_[data.full_])->SetValue(data.value_);
    }
    by_fullkey_[data.full_]->MarkAsParsed();
    return true;
  }
  else if (by_fullkey_[data.full_]->Type() == 'i') {
    if (IsANumber(data.value_)) {
      if (is_multivalue) {
        dynamic_cast<IntArg*>(by_fullkey_[data.full_])->SetMultiValue(stoi(data.value_));
        data.multivalue_args_to_check_.insert(data.full_);
      }
      else {
        dynamic_cast<IntArg*>(by_fullkey_[data.full_])->SetValue(stoi(data.value_));
      }
      by_fullkey_[data.full_]->MarkAsParsed();
      return true;
    }
    return false;
  }
  return false;
}

bool argparser::ArgParser::CheckPositionals() {
  bool got_positional_arg = false;
  std::unordered_map<std::string, BaseArg*>::iterator it;
  for (it = by_fullkey_.begin(); it != by_fullkey_.end(); ++it) {
    if (it->second->IsPositional()) {
      got_positional_arg = true;
      if (it->second->IsMultiValue()) {
        for (int pos_val : data.positional_values_) {
          IntArg* i_arg = dynamic_cast<IntArg*>(it->second);
          i_arg->SetMultiValue(pos_val);
        }
        it->second->MarkAsParsed();
        data.multivalue_args_to_check_.insert(it->first);
      }
      else {
        if (data.positional_values_.size() > 1) {
          return false;
        }
        else {
          IntArg* i_arg = dynamic_cast<IntArg*>(it->second);
          i_arg->SetValue(data.positional_values_[0]);
          it->second->MarkAsParsed();
        }
      }
    }
  }
  if (!got_positional_arg) {
    return false;
  }
  return true;
}

bool argparser::ArgParser::CheckMultivalueArgs() {
  for (const std::string& s : data.multivalue_args_to_check_) {
    if (by_fullkey_[s]->MultiValueArgMinimum() > 0) {
      if (by_fullkey_[s]->GivenMultiValuesCounter() <
        by_fullkey_[s]->MultiValueArgMinimum()) {
        return false;
      }
    }
    by_fullkey_[s]->ResetGivenMultivaluesCounter();
  }
  return true;
}

bool argparser::ArgParser::ValidateArgs() {
  int count_valid_args = 0;
  std::unordered_map<std::string, BaseArg*>::iterator it;
  for (it = by_fullkey_.begin(); it != by_fullkey_.end(); ++it) {
    count_valid_args += it->second->IsValid();
  }
  return ArgsAmount() - count_valid_args == 0;
}

bool argparser::ArgParser::Parse(std::vector<std::string> vector) {
  bool success;
  for (int i = 1; i < vector.size(); ++i) {
    std::string word = vector[i];
    int cursor;

    // FULL-KEY CASE
    if (word.length() > 2 && word[0] == '-' && word[1] == '-') {
      cursor = 2;
      while (cursor < word.size() / sizeof(char) && word[cursor] != '=') {
        data.full_.push_back(word[cursor]);
        ++cursor;
      }
      if (by_fullkey_.find(data.full_) != by_fullkey_.end()) {
        //--param=value case
        if (word[cursor] == '=') {
          ++cursor;
          while (cursor < word.length()) {
            data.value_.push_back(word[cursor]);
            ++cursor;
          }
          by_fullkey_[data.full_]->IsMultiValue()
              ? success = RegisterValue(true)
              : success = RegisterValue(false);
          if (!success) {
            return false;
          }
        }
        else {
          if (by_fullkey_[data.full_]->Type() == 'b') {
            if (data.full_ == "help") {
              ShowHelpMsg();
              return true;
            }
            dynamic_cast<BoolArg*>(by_fullkey_[data.full_])->SetTrueFlag();
          }
        }
      }
      else {
        return false;
      }
      data.full_.clear();
      data.value_.clear();
    }

    // SHORT-KEY CASE
    else if (word.length() >= 2 && word[0] == '-' && word[1] != '-') {
      cursor = 1;
      // -f case
      if (word.length() == 2) {
        data.short_ = word[cursor];
        if (by_shortkey_.find(data.short_) != by_shortkey_.end()) {
          std::string key = by_shortkey_[data.short_];
          if (by_fullkey_[key]->Type() == 'b') {
            dynamic_cast<BoolArg*>(by_fullkey_[key])->SetTrueFlag();
          }
          else if (by_fullkey_[key]->Type() == 'h') {
            ShowHelpMsg();
            return true;
          }
          else {
            return false;
          }
        }
        else {
          return false;
        }
      }
      // -p=value case
      else {
        if (word[cursor + 1] == '=') {
          data.short_ = word[cursor];
          if (by_shortkey_.find(data.short_) != by_shortkey_.end() &&
            cursor + 1 < word.size() - 1) {
            cursor += 2;
            while (cursor < word.length()) {
              data.value_.push_back(word[cursor]);
              ++cursor;
            }
            data.full_ = by_shortkey_[data.short_];
            by_fullkey_[data.full_]->IsMultiValue()
                ? success = RegisterValue(true)
                : success = RegisterValue(false);
            if (!success) {
              return false;
            }
          }
          else {
            return false;
          }
        }
        // -abc - several flags case
        else {
          while (cursor < word.length()) {
            if (by_shortkey_.find(word[cursor]) !=
              by_shortkey_.end()) {
              data.full_ = by_shortkey_[word[cursor]];
              BoolArg* b_arg = dynamic_cast<BoolArg*>(by_fullkey_[data.full_]);
              b_arg->SetTrueFlag();
              data.flags_.push_back(word[cursor]);
              ++cursor;
            }
            else {
              return false;
            }
          }
        }
      }
    }
    // POSITIONAL ARG CASE
    else {
      data.catched_positional_ = true;
      if (IsANumber(word)) {
        data.positional_values_.push_back(stoi(word));
        ++i;
        while (i < vector.size()) {
          std::string word_candidate = vector[i];
          if (IsANumber(word_candidate)) {
            data.positional_values_.push_back(stoi(word_candidate));
            ++i;
          }
          else {
            --i;
            break;
          }
        }
      }
    }
  }
  if (data.catched_positional_) {
    success = CheckPositionals();
    if (!success) {
      return false;
    }
  }
  success = CheckMultivalueArgs();
  if (!success) {
    return false;
  }
  success = ValidateArgs();
  return success;
}

bool argparser::ArgParser::Parse(int argc, char** argv) {
  return Parse(std::vector<std::string>(argv, argv + argc));
}

void argparser::ArgParser::AddToHelpMsg(char shortkey,
                                        const std::string& fullkey,
                                        const std::string& description) {
  int buff = 1000;
  char* helpline = new char[buff];
  sprintf(helpline, "-%c,  --%s,  %s\n", shortkey, fullkey.c_str(), description.c_str());
  arguments_descriptions_.emplace_back(helpline);
  delete[] helpline;
}

bool argparser::ArgParser::Help() const {
  return !arguments_descriptions_.empty();
}

BaseArg& argparser::ArgParser::AddHelp(char shortkey,
                                       const char* fullkey,
                                       const char* string2) {
  SetProgramDescription(string2);
  SetHelpCommandDescription("--help display help message and exit\n");
  AddToHelpMsg(shortkey, fullkey, string2);
  auto* arg = new BoolArg();
  arg->Default(true);
  AddToHelpMsg(' ', fullkey, "-");
  by_fullkey_[fullkey] = arg;
  by_shortkey_[shortkey] = fullkey;
  return *arg;
}

std::string argparser::ArgParser::ShowHelpMsg() const {
  if (Help()) {
    std::cout << ProgramName() << '\n';
    std::cout << ProgramDescription() << '\n';
    std::cout << '\n';
    for (const auto& i : arguments_descriptions_) {
      std::cout << i;
    }
    std::cout << '\n';
    std::cout << HelpCommandDescription() << '\n';
    return " ";
  }
  return "no help message provided";
}

void argparser::ArgParser::CheckIfExists(const char* fullkey) {
  if (by_fullkey_.find(fullkey) != by_fullkey_.end()) {
    throw std::runtime_error(std::string("argument with key ") + fullkey + " already exists");
  }
}

BaseArg& argparser::ArgParser::AddArg(BaseArg* arg,
                                      char type,
                                      const char shortkey,
                                      const char* fullkey,
                                      const char* description) {
  arg->type_ = type;
  AddToHelpMsg(shortkey, fullkey, description);
  by_fullkey_[fullkey] = arg;
  if (shortkey != ' ') {
    by_shortkey_[shortkey] = fullkey;
  }
  return *arg;
}

//                                     ADD INT ARG

IntArg& argparser::ArgParser::AddIntArgument(const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<IntArg&>(AddArg(new IntArg, 'i', ' ', fullkey, ""));
}

IntArg& argparser::ArgParser::AddIntArgument(const char shortkey,
                                             const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<IntArg&>(AddArg(new IntArg, 'i', shortkey, fullkey, ""));
}

IntArg& argparser::ArgParser::AddIntArgument(const char* fullkey,
                                             const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<IntArg&>(AddArg(new IntArg, 'i', ' ', fullkey, description));
}

IntArg& argparser::ArgParser::AddIntArgument(const char shortkey,
                                             const char* fullkey,
                                             const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<IntArg&>(AddArg(new IntArg, 'i', shortkey, fullkey, description));
}

//                                     ADD STR ARG

StrArg& argparser::ArgParser::AddStringArgument(const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<StrArg&>(AddArg(new StrArg, 's', ' ', fullkey, ""));
}

StrArg& argparser::ArgParser::AddStringArgument(char shortkey,
                                                const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<StrArg&>(AddArg(new StrArg, 's', shortkey, fullkey, ""));
}

StrArg& argparser::ArgParser::AddStringArgument(const char* fullkey,
                                                const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<StrArg&>(AddArg(new StrArg, 's', ' ', fullkey, description));
}

StrArg& argparser::ArgParser::AddStringArgument(char shortkey,
                                                const char* fullkey,
                                                const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<StrArg&>(AddArg(new StrArg, 's', shortkey, fullkey, description));
}

//                                     ADD BOOL ARG

BoolArg& argparser::ArgParser::AddFlag(const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<BoolArg&>(AddArg(new BoolArg, 'b', ' ', fullkey, ""));
}

BoolArg& argparser::ArgParser::AddFlag(char shortkey, const char* fullkey) {
  CheckIfExists(fullkey);
  return dynamic_cast<BoolArg&>(AddArg(new BoolArg, 'b', shortkey, fullkey, ""));
}

BoolArg& argparser::ArgParser::AddFlag(const char* fullkey,
                                       const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<BoolArg&>(AddArg(new BoolArg, 'b', ' ', fullkey, description));
}

BoolArg& argparser::ArgParser::AddFlag(char shortkey,
                                       const char* fullkey,
                                       const char* description) {
  CheckIfExists(fullkey);
  return dynamic_cast<BoolArg&>(AddArg(new BoolArg, 'b', shortkey, fullkey, description));
}

//                                     GETTERS

int argparser::ArgParser::GetIntValue(const char* fullkey) {
  return dynamic_cast<IntArg*>(by_fullkey_[fullkey])->Value();
}

int argparser::ArgParser::GetIntValue(const char* fullkey, int ix) {
  return dynamic_cast<IntArg*>(by_fullkey_[fullkey])->ValueVector()[ix];
}

std::string argparser::ArgParser::GetStringValue(const char* fullkey) {
  return dynamic_cast<StrArg*>(by_fullkey_[fullkey])->Value();
}

std::string argparser::ArgParser::GetStringValue(const char* fullkey, int ix) {
  return dynamic_cast<StrArg*>(by_fullkey_[fullkey])->ValueVector()[ix];
}

bool argparser::ArgParser::GetFlag(const char* fullkey) {
  return dynamic_cast<BoolArg*>(by_fullkey_[fullkey])->GetlValue();
}

bool IsANumber(const std::string& str) {
  for (char i : str) {
    if (!isdigit(i)) {
      return false;
    }
  }
  return true;
}

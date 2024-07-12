#include <functional>
#include <lib/argument_parser.h>

#include <iostream>
#include <numeric>

struct Options {
  bool sum = false;
  bool mult = false;
};

int main(int argc, char** argv) {
  Options opt;
  std::vector<int> values;

  argparser::ArgParser parser("Program");
  parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
  parser.AddFlag("sum", "add args").StoreValue(opt.sum).Default(false);
  parser.AddFlag("mult", "multiply args").StoreValue(opt.mult).Default(false);
  parser.AddHelp('h', "help", "Program accumulate arguments");

  if (!parser.Parse(argc, argv)) {
    std::cout << "Wrong argument" << std::endl;
    std::cout << parser.ShowHelpMsg() << std::endl;
    return 1;
  }

  if (parser.Help()) {
    std::cout << parser.ShowHelpMsg() << std::endl;
  }

  if (opt.sum) {
    std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
  }
  else if (opt.mult) {
    std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) <<
        std::endl;
  }
  else {
    std::cout << "No one options had chosen" << std::endl;
    std::cout << parser.ShowHelpMsg();
    return 1;
  }

  return 0;
}

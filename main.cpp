#include <iostream>
#include <fstream>
#include "cxxopts.h"
#include "CnfFormula.h"
#include "GriddlerPuzzle.h"

void FormulaMain() {
    CnfFormula formula;
    Clause c1{list<Literal> {
            Literal {"x1", true}, Literal {"x2", true}
    }};
    formula.AddClause(c1);
    for (auto x : formula.Solve())
        cout << x.first << " " << x.second << endl;
}

void GriddlerSimpleMain() {
    Row row1(0, 2), row2(1, 2);
    Column col1(0, 2), col2(1, 2);

    row1.AddBlock(2);
    row2.AddBlock(1);
    col1.AddBlock(2);
    col2.AddBlock(1);

    GriddlerPuzzle griddler(2, 2);
    griddler.AddRow(row1);
    griddler.AddRow(row2);
    griddler.AddColumn(col1);
    griddler.AddColumn(col2);

    vector<vector<bool>> solution = griddler.Solve();
    cout << solution[0][0] << " " << solution[0][1] << endl;
    cout << solution[1][0] << " " << solution[1][1] << endl;
}

void GriddlerDolphinMain() {
    ifstream f("inputDolphin.txt");
    auto griddler = GriddlerPuzzle::Parse(f);

    griddler.SolveAndPrint(cout);
}

void GriddlerFilechooseMain() {
    cout << "What is the input file?" << endl;
    string path;
    cin >> path;
    ifstream f(path);
    if (!f.is_open()) {
        cerr << "Could not open file" << endl;
        throw invalid_argument("Could not open file");
    }

    auto griddler = GriddlerPuzzle::Parse(f);
    griddler.SolveAndPrint(cout);
}

int GriddlerMain(int argc, char **argv) {
    string input_file_path, output_file_path, output_style;
    cxxopts::Options options("Griddler",
                             "Solve Griddler problems using MiniSat SAT solver.\n"
                                     "To understand the file format, see the examples.");
    cxxopts::OptionAdder options_adder = options.add_options();
    options_adder("i,input-file", "The file with the Griddler problem. Defaults to stdin.",
                  cxxopts::value<string>(input_file_path));
    options_adder("o,output-file", "The file to the print the solution. Defaults to stdout.",
                  cxxopts::value<string>(output_file_path));
    options_adder("output-style", R"(The style of the output - "O" (default), "bricks", or "bits")",
                  cxxopts::value<string>(output_style));
    options_adder("h,help", "Display this help");
    try {
        options.parse(argc, argv);
    }
    catch(const cxxopts::option_not_exists_exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    if (options.count("help") > 0) {
        cout << options.help();
        return 0;
    }

    istream *input;
    ostream *output;
    PrintStyle style;
    if (options.count("output-style") == 0 || output_style == "O") style = PRINTSTYLE_O;
    else if (output_style == "bricks") style = PRINTSTYLE_BRICKS;
    else if (output_style == "bits") style = PRINTSTYLE_BITS;
    else {
        cerr << "Unknown print style." << endl;
        return 1;
    }

    ifstream input_file(input_file_path);
    if (options.count("input-file") == 0) input = &cin;
    else {
        if (!input_file.is_open()) {
            cerr << "Couldn't open input file." << endl;
            return 1;
        }
        input = &input_file;
    }

    ofstream output_file(output_file_path);
    if (options.count("output-file") == 0) output = &cout;
    else {
        if (!output_file.is_open()) {
            cerr << "Couldn't open output file." << endl;
            return 1;
        }
        output = &output_file;
    }

    try {
        auto griddler = GriddlerPuzzle::Parse(*input);
        griddler.SolveAndPrint(*output, style);
    }
    catch (const invalid_argument &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    catch (const domain_error &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return 0;
}

int main(int argc, char **argv) {
    return GriddlerMain(argc, argv);
}
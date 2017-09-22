#include <sstream>
#include "GriddlerPuzzle.h"

using namespace std;

GriddlerPuzzle::GriddlerPuzzle(unsigned rows_count, unsigned columns_count) {
    for (unsigned i = 0; i < rows_count; i++)
        rows_.emplace_back(i, columns_count);
    for (unsigned i = 0; i < columns_count; i++)
        columns_.emplace_back(i, rows_count);
}

void GriddlerPuzzle::AddRow(const Row &row) {
    rows_[row.GetIndex()] = row;
}

void GriddlerPuzzle::AddColumn(const Column &col) {
    columns_[col.GetIndex()] = col;
}

vector<vector<bool>> GriddlerPuzzle::Solve() const {
    CnfFormula formula;
    for (auto &row : rows_)
        row.AddClauses(formula);
    for (auto &col: columns_)
        col.AddClauses(formula);
    map<string, bool> solution_map = formula.Solve();
    if (solution_map.empty()) {
        throw domain_error("Couldn't find a solution!!");
    }

    vector<vector<bool>> solution;
    for (unsigned row = 0; row < rows_.size(); row++) {
        vector<bool> current_row;
        for (unsigned col = 0; col < columns_.size(); col++) {
            ostringstream variable_name;
            variable_name << "r" << row;
            variable_name << "c" << col;
            current_row.push_back(solution_map[variable_name.str()]);
        }
        solution.push_back(current_row);
    }
    return solution;
}

GriddlerPuzzle GriddlerPuzzle::Parse(istream &input) {
    char c;
    input >> c;
    if (c != 'g') throw invalid_argument("Griddler input should start with the character g");
    unsigned height, width;
    input >> height;
    input >> width;
    GriddlerPuzzle griddler(height, width);
    for (auto &row : griddler.rows_)
        row.Parse(input);
    for (auto &column: griddler.columns_)
        column.Parse(input);
    return griddler;
}

void GriddlerPuzzle::SolveAndPrint(ostream &out, PrintStyle style) const {
    vector<vector<bool>> solution = Solve();
    for (auto &row : solution) {
        for (auto cell : row) {
            switch (style) {
                case PRINTSTYLE_O:
                    if (cell) out << 'O';
                    else out << '_';
                    break;
                case PRINTSTYLE_BRICKS:
                    if (cell) out << char(178);
                    else out << ' ';
                    break;
                case PRINTSTYLE_BITS:
                    if (cell) out << "1\t";
                    else out << "0\t";
                    break;
            }
        }
        out << endl;
    }
}

#ifndef GRIDDLER_GRIDDLERPUZZLE_H
#define GRIDDLER_GRIDDLERPUZZLE_H

#include "Line.h"
#include <vector>

using namespace std;


enum PrintStyle {
    PRINTSTYLE_O,
    PRINTSTYLE_BRICKS,
    PRINTSTYLE_BITS
};


class GriddlerPuzzle {
public:
    GriddlerPuzzle(unsigned rows_count, unsigned columns_count);

    void AddRow(const Row &row);

    void AddColumn(const Column &col);

    vector<vector<bool>> Solve() const;

    void SolveAndPrint(ostream &out, PrintStyle style = PRINTSTYLE_O) const;

    static GriddlerPuzzle Parse(istream &input);

private:

    vector<Row> rows_;
    vector<Column> columns_;
};


#endif //GRIDDLER_GRIDDLERPUZZLE_H

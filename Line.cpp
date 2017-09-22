#include <sstream>
#include <iostream>
#include "Line.h"

unsigned Line::BlockMaxStart(unsigned block_size) const {
    return length_ - block_size;
}

void Line::AddBlockStartsSomewhereClauses(CnfFormula &formula) const {
    unsigned block_index = 0;
    for (unsigned block_size : blocks_) {
        list<Literal> literals;
        for (unsigned start = 0; start <= BlockMaxStart(block_size); start++)
            literals.emplace_back(BlockStartVariableName(block_index, start), true);
        formula.AddClause(Clause(literals));
        block_index++;
    }
}

void Line::AddBlockDoesntDuplicateClauses(CnfFormula &formula) const {
    unsigned block_index = 0;
    for (unsigned block_size : blocks_) {
        for (unsigned position = 0; position <= BlockMaxStart(block_size); position++)
            for (unsigned greater_position = position + 1; greater_position <= BlockMaxStart(block_size); greater_position++) {
                list<Literal> literals;
                literals.emplace_back(BlockStartVariableName(block_index, position), false);
                literals.emplace_back(BlockStartVariableName(block_index, greater_position), false);
                formula.AddClause(Clause(literals));
            }
        block_index++;
    }
}

void Line::AddBlockToCellClauses(CnfFormula &formula) const {
    unsigned block_index = 0;
    for (unsigned block_size : blocks_) {
        for (unsigned start = 0; start <= BlockMaxStart(block_size); start++) {
            for (unsigned index = 0; index < block_size; index++) {
                list<Literal> literals;
                literals.emplace_back(BlockStartVariableName(block_index, start), false);
                literals.emplace_back(CellVariableName(start + index), true);
                formula.AddClause(Clause(literals));
            }
        }
        block_index++;
    }
}

void Line::AddCellToBlockClauses(CnfFormula &formula) const {
    for (unsigned cell = 0; cell < length_; cell++) {
        list<Literal> literals;
        literals.emplace_back(CellVariableName(cell), false);
        unsigned block_i = 0;
        for (unsigned block : blocks_) {
            for (unsigned position = 0; position <= BlockMaxStart(block) && position <= cell; position++) {
                if (position + block > cell)
                    literals.emplace_back(BlockStartVariableName(block_i, position), true);
            }
            block_i++;
        }
        formula.AddClause(Clause(literals));
    }
}

void Line::AddBlockOrderClauses(CnfFormula &formula) const {
    if (blocks_.size() < 2) return;

    unsigned block_index = 0;
    auto block_i = blocks_.begin(), next_block_i = blocks_.begin();
    next_block_i++;

    while (next_block_i != blocks_.end()) {
        for (unsigned first_start = 0; first_start <= BlockMaxStart(*block_i); first_start++)
            for (unsigned second_start = 0; second_start <= BlockMaxStart(*next_block_i); second_start++) {
                if (second_start < first_start + *block_i + 1) {
                    list<Literal> literals;
                    literals.emplace_back(BlockStartVariableName(block_index, first_start), false);
                    literals.emplace_back(BlockStartVariableName(block_index + 1, second_start), false);
                    formula.AddClause(Clause(literals));
                }
            }

        block_index++;
        block_i++;
        next_block_i++;
    }
}

void Line::AddClauses(CnfFormula &formula) const {
    AddBlockStartsSomewhereClauses(formula);
    AddBlockDoesntDuplicateClauses(formula);
    AddBlockToCellClauses(formula);
    AddCellToBlockClauses(formula);
    AddBlockOrderClauses(formula);
}

string Row::CellVariableName(unsigned cell_index) const {
    ostringstream name;
    name << "r" << this->GetIndex();
    name << "c" << cell_index;
    return name.str();
}

string Column::CellVariableName(unsigned cell_index) const {
    ostringstream name;
    name << "r" << cell_index;
    name << "c" << this->GetIndex();
    return name.str();
}

string Row::BlockStartVariableName(unsigned block_index, unsigned start_index) const {
    ostringstream name;
    name << "r" << this->GetIndex();
    name << "b" << block_index;
    name << "s" << start_index;
    return name.str();
}

string Column::BlockStartVariableName(unsigned block_index, unsigned start_index) const {
    ostringstream name;
    name << "c" << this->GetIndex();
    name << "b" << block_index;
    name << "s" << start_index;
    return name.str();
}

void Line::Parse(istream &input) {
    string input_type;
    input >> input_type;
    if (input_type != ParseStartString())
        throw invalid_argument("Expected line type " + ParseStartString() + " but got " + input_type);

    while (true) {
        unsigned block;
        input >> block;
        if (block == 0) break;
        AddBlock(block);
    }
}
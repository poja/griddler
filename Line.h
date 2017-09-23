#ifndef GRIDDLER_LINE_H
#define GRIDDLER_LINE_H

#include <list>
#include <ostream>
#include "CnfFormula.h"

using namespace std;

class Line {
public:
    explicit Line(unsigned index, unsigned length) : index_(index), length_(length) {}
    int GetIndex() const { return index_; }
    virtual void AddClauses(CnfFormula &formula) const;
    void AddBlock(unsigned size) { blocks_.push_back(size); }
    void Parse(istream& input);
protected:
    virtual string BlockStartVariableName(unsigned block_index, unsigned start_index) const = 0;
    virtual string CellVariableName(unsigned index) const = 0;
    virtual string ParseStartString() const = 0;
private:
    virtual void AddBlockStartsSomewhereClauses(CnfFormula &formula) const;
    virtual void AddBlockDoesntDuplicateClauses(CnfFormula &formula) const;
    virtual void AddBlockToCellClauses(CnfFormula &formula) const;
    virtual void AddCellToBlockClauses(CnfFormula &formula) const;
    virtual void AddBlockOrderClauses(CnfFormula &formula) const;
    unsigned BlockMaxStart(unsigned block_size) const;

    list<unsigned> blocks_;
    unsigned index_;
    unsigned length_;
};

class Row : public Line {
public:
    explicit Row(unsigned index, unsigned length): Line(index, length) {}
protected:
    string BlockStartVariableName(unsigned block_index, unsigned start_index) const override;
    string CellVariableName(unsigned cell_index) const override;
    string ParseStartString() const override { return "r"; };
};

class Column : public Line {
public:
    explicit Column(unsigned index, unsigned length): Line(index, length) {}
protected:
    string BlockStartVariableName(unsigned block_index, unsigned start_index) const override;
    string CellVariableName(unsigned cell_index) const override;
    string ParseStartString() const override { return "c"; };
};

#endif //GRIDDLER_LINE_H

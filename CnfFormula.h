#ifndef GRIDDLER_CNFFORMULA_H
#define GRIDDLER_CNFFORMULA_H

#include <list>
#include <map>
#include <vector>
#include <ostream>

using namespace std;

struct Literal {
    Literal(const string& name, bool sign) : name_(string(name)), sign_(sign) {}
    const string name_;
    const bool sign_;
};

class Clause {
public:
    explicit Clause(const list<Literal> &literals);
    inline const list<Literal> GetLiterals() const { return literals_; }
private:
    const list<Literal> literals_;
};

class CnfFormula {
public:
    CnfFormula();
    void AddClause(Clause c);
    map<string, bool> Solve() const;
    ostream& PrettyPrint(ostream& o) const;
    friend ostream& operator<<(ostream& o, const CnfFormula& formula);
private:
    list<Clause> clauses_;
    vector<string> variables_vector_;
    map<string, int> variables_map_;
};


#endif //GRIDDLER_CNFFORMULA_H

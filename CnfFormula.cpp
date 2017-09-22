#include "CnfFormula.h"
#include <climits>
#include <fstream>
#include <limits>
#include <iostream>

CnfFormula::CnfFormula() = default;

void CnfFormula::AddClause(const Clause c) {
    for (auto literal : c.GetLiterals()) {
        const string &name = literal.name_;
        if (variables_map_.count(name) == 0) {
            variables_vector_.push_back(name);

            unsigned long size = variables_vector_.size();
            if (size > INT_MAX)
                throw out_of_range("Too many variables");
            int index = (int) variables_vector_.size() - 1;
            variables_map_[name] = index;
        }
    }
    clauses_.push_back(c);
}

// Prints the formula in DIMACS format
ostream &operator<<(ostream &o, const CnfFormula &formula) {
    o << "p cnf " << formula.variables_vector_.size() << " " << formula.clauses_.size() << std::endl;
    for (auto &clause : formula.clauses_) {
        for (auto &literal : clause.GetLiterals()) {
            if (!literal.sign_) o << '-';
            o << formula.variables_map_.at(literal.name_) + 1 << " ";
        }
        o << '0' << std::endl;
    }
    return o;
}

vector<bool> ParseSatOutput(const string &sat_output_path) {
    ifstream output_file;
    output_file.open(sat_output_path);
    output_file.ignore(numeric_limits<streamsize>::max(), '\n');
    vector<bool> solution;
    int val;
    do {
        output_file >> val;
        if (val != 0)
            solution.push_back(val > 0);
    } while (val != 0);
    return solution;
}

map<string, bool> CnfFormula::Solve() const {
    string sat_input_path = "sat_work/input.txt";
    string sat_output_path = "sat_work/output.txt";
    ofstream input_file;
    input_file.open(sat_input_path);
    input_file << *this;
    input_file.close();
    system(("../minisat/cmake-build-debug/SAT.exe " + sat_input_path + " " + sat_output_path).c_str()); // TODO Fix this
    vector<bool> values = ParseSatOutput(sat_output_path);
    map<string, bool> solution;
    for (unsigned int i = 0; i < values.size(); i++) {
        string name = variables_vector_[i];
        solution[name] = values[i];
    }
    return solution;
}

Clause::Clause(const list<Literal> &literals) : literals_(list<Literal>(literals)) {
}

ostream& CnfFormula::PrettyPrint(ostream& out) const {
    for (auto& clause : clauses_) {
        out << "( ";
        for (auto& literal : clause.GetLiterals()) {
            if (!literal.sign_) out << "!";
            out << literal.name_ << " ";
        }
        out << " )";
    }
    out << endl;
    return out;
}
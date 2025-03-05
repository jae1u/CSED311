#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <iostream>
#include <iomanip>
#include <ios>
#include <string>
#include <sstream>
#include <memory>
#include <verilated.h>
#include <verilated_vcd_c.h>

// TODO: implement max simulation time
constexpr int DEFAULT_MAX_SIM_TIME = 200;


// === Simulator : bass class
template<class M>
class Simulator {
public:
    Simulator(std::string name) : mod_name(name), ctx(), m(&ctx), trace() {}
    int main();
    void init();
    void step(unsigned n = 1);
    void step_one();
    bool check(int expected, int actual, std::string msg);
    void success(std::string msg);
    void fail(std::string msg);
    void report();
    virtual void body() = 0;
    ~Simulator();
public:
    int n_success = 0;
    int n_fail = 0;
    int t = 0;
    std::string mod_name;
    VerilatedContext ctx;
    M m;
    VerilatedVcdC trace;
};

template <class M>
int Simulator<M>::main() {
    init();
    body();
    report();
    return n_fail > 0;
}

template <class M>
void Simulator<M>::init() {
    Verilated::mkdir("logs");

    ctx.debug(0);
    ctx.randReset(2);
    ctx.traceEverOn(true);
    m.trace(&trace, 99); // trace 99 levels of hierarchy

    trace.open((std::string("logs/") + mod_name + ".vcd").c_str());

    std::cout << std::string(90, '=') << "\n";
    std::cout << "\033[1;34m" << "MODULE  " << "\033[0m" << mod_name << "\n";
}

template <class M>
void Simulator<M>::step(unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        step_one();
    }
}

template<class M>
void Simulator<M>::step_one() {
    m.eval();
    trace.dump(t);
    ++t;
}

template <class M>
bool Simulator<M>::check(int expected, int actual, std::string msg) {
    bool cond = expected == actual;
    if (!cond) {
        std::ostringstream fail_msg;
        fail_msg << std::left
            << std::setw(50) << msg
            << "expected: " << std::setw(5) << expected
            << "actual: " << std::setw(5) << actual;
        fail(fail_msg.str());
    } else {
        success(msg);
    }
    return cond;
}

template <class M>
void Simulator<M>::success(std::string msg) {
    ++n_success;
    std::cout 
        << "\033[32m" <<  "  PASS  " << "\033[0m" << std::left
        << std::setw(78) << msg
        << "@ " << std::setw(3) << t
        << "\n";
}

template <class M>
void Simulator<M>::fail(std::string msg) {
    ++n_fail;
    std::cout
        << "\033[31m" << "! FAIL  " << "\033[0m" << std::left
        << std::setw(78) << msg
        << "@ " << std::setw(3) << t
        << "\n";   
}


template <class M>
void Simulator<M>::report() {
    std::cout << "\n";
    std::cout 
        << "simulation done. total time: " << t << "\n";
    if (n_fail == 0) {
        std::cout 
            << "PASS "
            << "(" << n_success << "/" << n_success << ")\n";
    } else {
        std::cout 
            << "FAIL "
            << "(failed: " << n_fail
            << ", passed: " << n_success
            << ", total: " << n_fail + n_success << ")"
            << "\n";   
    }
}

template <class M>
Simulator<M>::~Simulator() {
    trace.close();
    m.final();
}

// === CombiSimulator : simulator for combinational logic
template <class M>
class CombiSimulator : public Simulator<M> {
public:
    CombiSimulator(std::string name) : Simulator<M>(name) {}
};


// === SeqSimulator : simulator for sequential logic
template <class M>
class SeqSimulator : public Simulator<M> {
public:
    SeqSimulator(std::string name, int max_sim_time = DEFAULT_MAX_SIM_TIME, bool posedge = true) 
        : Simulator<M>(name), max_sim_time(max_sim_time), posedge(posedge) {};
    void init();
    void step_one();
    void step(unsigned n = 1);
    void clock(unsigned n = 1);
public:
    int max_sim_time;
    bool posedge;
};

template<class M>
void SeqSimulator<M>::init() {
    this->m.clk = (int)!posedge;
    Simulator<M>::init();
}

template <class M>
void SeqSimulator<M>::step_one() {
    this->m.clk ^= 1;
    this->m.eval();
    this->trace.dump(this->t);
    ++this->t;
}

template <class M>
void SeqSimulator<M>::step(unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        step_one();
    }
}


template <class M>
void SeqSimulator<M>::clock(unsigned n) {
    this->step(2*n);
}

#define SIMULATOR_MAIN(cls) \
int main() { \
    cls ## Simulator s; \
    return s.main(); \
}

#define SEQ_SIMULATOR(cls, name, bdy) \
class cls ## Simulator : public SeqSimulator<V ## name> { \
public: \
    cls ## Simulator() : SeqSimulator(#name) {} \
    virtual void body() bdy \
}; \
SIMULATOR_MAIN(cls)

#define COMBI_SIMULATOR(cls, name, bdy) \
class cls ## Simulator : public CombiSimulator<V ## name> { \
public: \
    cls ## Simulator() : CombiSimulator(#name) {} \
    virtual void body() bdy \
}; \
SIMULATOR_MAIN(cls)


#endif

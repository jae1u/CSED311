#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <iostream>
#include <iomanip>
#include <ios>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <verilated.h>
#include <verilated_vcd_c.h>


// === Simulator : bass class
struct TestCaseResult {
public:
    bool pass;
    std::string msg = "";
};

template<class M>
class Simulator {
public:
    typedef TestCaseResult (*t_testfn)(M& m, Simulator& s);
    typedef void (*t_hookfn)(M& m, Simulator& s);
public:
    Simulator(std::string name) : mod_name(name), testcases(), ctx(), m(&ctx), trace() {}
    int main();
    void init();
    void run();
    void step(unsigned n = 1);
    virtual void step_once();
    TestCaseResult check(int expected, int actual, std::string msg);
    void success(std::string msg, unsigned indent = 0);
    void fail(std::string msg, unsigned indent = 0);
    void report();
    void add_case(std::string name, t_testfn f);
    void register_setup_fn(t_hookfn f);
    void register_teardown_fn(t_hookfn f);
    virtual ~Simulator();
protected:
    std::string success_msg(std::string msg, unsigned indent);
    std::string fail_msg(std::string msg, unsigned indent);
public:
    int n_success = 0;
    int n_fail = 0;
    int t = 0;
    t_hookfn setupfn = nullptr;
    t_hookfn teardownfn = nullptr;
    std::string mod_name;
    std::map<std::string, t_testfn> testcases;
    VerilatedContext ctx;
    M m;
    VerilatedVcdC trace;
};

template <class M>
int Simulator<M>::main() {
    init();
    run();
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

    std::cout 
        << "\033[1;34m" << "MODULE  " << "\033[0m" 
        << mod_name << " "
        << std::string(90 - 9 - mod_name.size(), '=') << "\n";
}

template <class M>
void Simulator<M>::run() {
    for (auto const& [casename, fn] : testcases) {
        if (setupfn != nullptr) setupfn(m, *this);
        TestCaseResult result = fn(m, *this);
        step(2); // to record last state
        if (result.pass) {
            ++n_success;
        } else {
            fail(std::string("CASE: ") + casename);
            fail(result.msg, 4);
            ++n_fail;
        }
        if (teardownfn != nullptr) teardownfn(m, *this);
    }
}

template <class M>
void Simulator<M>::step(unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        step_once();
    }
}

template<class M>
void Simulator<M>::step_once() {
    m.eval();
    trace.dump(t);
    ++t;
}

template <class M>
TestCaseResult Simulator<M>::check(int expected, int actual, std::string msg) {
    bool cond = expected == actual;
    if (!cond) {
        std::ostringstream fail_msg;
        fail_msg << std::left
            << std::setw(50 - 4) << msg
            << "expected: " << std::setw(5) << expected
            << "actual: " << std::setw(5) << actual;
        return {false, fail_msg.str()};
    }
    return {true, ""};
}

template<class M>
void Simulator<M>::success(std::string msg, unsigned indent) {
    std::cout << success_msg(msg, indent);
}

template<class M>
void Simulator<M>::fail(std::string msg, unsigned indent) {
    std::cout << fail_msg(msg, indent);
}

template <class M>
std::string Simulator<M>::success_msg(std::string msg, unsigned indent) {
    std::ostringstream out;
    out << std::string(indent, ' ')
        << "\033[32m" <<  "  PASS  " << "\033[0m" << std::left
        << std::setw(78 - indent) << msg
        << "@ " << std::setw(3) << t
        << "\n";
    return out.str();
}

template <class M>
std::string Simulator<M>::fail_msg(std::string msg, unsigned indent) {
    std::ostringstream out;
    out << std::string(indent, ' ')
        << "\033[31m" << "! FAIL  " << "\033[0m" << std::left
        << std::setw(78 - indent) << msg
        << "@ " << std::setw(3) << t
        << "\n";
    return out.str();
}


template <class M>
void Simulator<M>::report() {
    if (n_fail == 0) {
        std::cout 
            << "ALL PASS "
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
void Simulator<M>::add_case(std::string name, t_testfn f) {
    testcases.insert({name, f});
}

template <class M>
void Simulator<M>::register_setup_fn(t_hookfn f) {
    setupfn = f;
}

template <class M>
void Simulator<M>::register_teardown_fn(t_hookfn f) {
    teardownfn = f;
}


template <class M>
Simulator<M>::~Simulator() {
    trace.close();
    m.final();
}


#define SIM_COMBI(name, body) \
class CombiSimulator : public Simulator<V ## name> { \
public: \
    CombiSimulator() : Simulator(#name) {} \
    void setup(); \
    void teardown(); \
}; \
int main() { \
    CombiSimulator s; \
    do body while(0); \
    return s.main(); \
}

#define SIM_SETUP(body) \
s.register_setup_fn([](auto m, auto s) body);

#define SIM_TEARDOWN(body) \
s.register_teardown_fn([](auto m, auto s) body);

#define SIM_CASE(casename, body) \
s.add_case(casename, [](auto m, auto s) { \
    TestCaseResult result; \
    do body while(0); \
    return TestCaseResult {true, ""}; \
});

#define SIM_CHECK(actual, expected, msg) \
result = s.check(expected, actual, msg); \
if (!result.pass) return result;

#define SIM_CHECK_PORT(port, expected) \
result = s.check(expected, m.port, "." #port); \
if (!result.pass) return result;


// === SeqSimulator : simulator for sequential logic
template <class M>
class SeqSimulator : public Simulator<M> {
public:
    SeqSimulator(std::string name) : Simulator<M>(name) {}
    void step_once();
    void tick(unsigned n = 1);
};

template <class M>
void SeqSimulator<M>::step_once() {
    this->m.clk ^= 1;
    Simulator<M>::step_once();
}

template <class M>
void SeqSimulator<M>::tick(unsigned n) {
    this->step(2*n);
}

#define SIM_SEQ(name, body) \
class MySeqSimulator : public SeqSimulator<V ## name> { \
public: \
    MySeqSimulator() : SeqSimulator(#name) {} \
    void setup(); \
    void teardown(); \
}; \
int main() { \
    MySeqSimulator s; \
    do body while(0); \
    return s.main(); \
}

#define SIM_SEQ_SETUP(body) \
s.register_setup_fn([](auto m, auto s_) { \
    MySeqSimulator& s = dynamic_cast<MySeqSimulator&>(s_); \
    do body while(0); \
});

#define SIM_SEQ_TEARDOWN(body) \
s.register_teardown_fn([](auto m, auto s) body);

#define SIM_SEQ_CASE(casename, body) \
s.add_case(casename, [](auto m, auto s_) { \
    TestCaseResult result; \
    MySeqSimulator& s = dynamic_cast<MySeqSimulator&>(s_); \
    do body while(0); \
    return TestCaseResult {true, ""}; \
});


#endif

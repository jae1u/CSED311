# 워크플로우 튜토리얼

## 새 프로젝트 만들기
`skeleton` 디렉토리를 복사해서 새로운 프로젝트를 시작하세요. 

프로젝트 디렉토리의 구조는 다음과 같습니다.
```
project
    \- def
        \- defs.v
    \- mod
        \- nand.v
    \- test
        \- Simulator.h
        \- tb_nand.cpp
    \- test_mod
    \- Makefile
```
- `def`: ``\`include``되어 사용되는 베릴로그 파일들
- `mod`: 베릴로그 모듈 소스 파일들
- `test`: 테스트벤치 파일들
- `test_mod`: 실제 설계에 사용되지 않고 테스트를 위해서 존재하는 베릴로그 모듈들

## 새 모듈 만들기
`mod` 디렉토리에 새 파일을 생성하고 모듈을 정의히세요.

```verilog
/* mod/and.v */

module and(
    input a,
    input b,
    output c
);
    assign c = a & b;
endmodule
```

- `make verilate` 명령어를 입력해 모든 모듈들을 린팅할 수 있습니다.
- 모듈 이름으로는 snake_case를 사용하세요.

## 테스트벤치 작성하기
`test` 디렉토리에 `tb_<모듈명>.cpp` 파일을 생성하고, 모듈의 타입에 따라 테스트벤치 코드를 다음과 같이 작성하세요.

```cpp
/* mod/tb_and.cpp */

#include "Simulator.h"  // 시뮬레이션 도우미 라이브러리를 불러옵니다.
#include "Vand.h"   // verilate된 모듈을 불러옵니다.

// 아래 메크로를 통해 테스트벤치를 정의합니다.
// 첫번째와 두번째 인자에는 모듈의 이름을 각각 PascalCase와 snake_case로 입력합니다.
COMBI_SIMULATOR(Simulator, simulator, {
    // 이곳에 테스트 스크립트를 작성하세요.
    // 테스트는 1) 모듈 입력값 정의 -> 2) step() 호출 -> 3) 모듈 출력값 확인 -> 4) 모듈 입력값 초기화 순의 구조를 가집니다.

    m.a = 0;                // 1) 모듈 입력값 정의
    m.b = 0;
    step();                 // 2) 시뮬레이션을 한 단계 진행시킵니다. 시뮬레이션 시간도 1만큼 늘어납니다.
    check(0, m.c, "0 0");   // 3) 모듈의 출력값을 check() 함수를 통해 확인합니다. 
                            //    해당 함수는 앞의 두 인자의 값이 다르면 세번째 인자로 받은 메시지를 출력합니다.
    m.a = 0;                // 4) 모듈 입력값 초기화그리고
    m.b = 0;

    // ...
})  
```

`make build` 명령어를 호출하면 모든 테스트벤치가 `obj_dir/V<모듈명>` 로 컴파일됩니다.
`make run` 명령어를 통해 모든 컴파일된 테스트벤치들을 실행할 수 있습니다. 

새로운 모듈을 작성할 때에는 다음과 같은 순서를 따를 것을 권장합니다. 만약 모듈의 이름이 `and.v`라면,

1. `tb_and.cpp` 파일을 생성하고 테스트벤치 작성
2. `and.v` 파일을 생성하고 모듈 정의 작성
3. `make build && obj_dir/Vand` 명령으로 모듈이 테스트벤치를 통과하는지 확인
    - 통과한다면, `make run`으로 모든 테스트가 통과되는지 확인
    - 문제가 발생한다면 `logs/and.vcd` 파일을 활용해 디버깅 -> 오류 수정 -> 다시 테스트 -> 반복...

## 여러 모듈 한번에 테스트하기
위와 같이 각각의 모듈의 작동을 테스트하는 유닛 테스트(unit test) 뿐만 아니라, 여러 모듈이 같이 잘 동작하는지 확인하는 통합 테스트(integration test)도 중요합니다.
(see: https://www.reddit.com/r/ProgrammerHumor/comments/isidkn)

통합 테스트를 위해서는 `test_mod` 디렉토리에 테스트하고자 하는 모듈들을 사용하는 새로운 상위 모듈을 만드세요.
```verilog
/* test_mod/alu.v */

module alu(
    input a,
    input b,
    input sel,
    output c,
);
    wire and_out, or_out;
    and and_module(
        .a(a),
        .b(b),
        .c(and_out)
    );
    or or_mdoule(
        .a(a),
        .b(b),
        .c(or_out)
    );
    assign c = (~sel & and_out) | (sel & or_out);
endmodule
```

그리고 일반 모듈과 동일한 방법으로 테스트벤치를 작성해서 통합 테스트를 수행할 수 있습니다.


# Makefile 커멘드 레퍼런스
- `make verilate`: 모든 베릴로그 파일들을 `verilator`를 이용해서 린트 및 C++ 소스로 컴파일
- `make build`: 모든 테스트벤치와 모듈들을 컴파일. 컴파일이 필요한 파일들만 컴파일됩니다. `make verilate`를 자동으로 실행합니다.
- `make` == `make run`: 모든 테스트벤치를 실행합니다. `make build`를 자동으로 실행합니다.

# 시뮬레이션 도우미(Simulator.h)의 내부 구조
시뮬레이션 도우미는 베이스 클래스 `Simulator`과 해당 클래스의 자식 클래스 `CombiSimulator`, `SeqSimulator`로 이루어집니다.

`Simulator`는 테스트벤치를 간결하게 작성하기 위한 여러 메소드들과 멤버 변수를 가지고 있습니다.

- `init()`: 시뮬레이션 준비. 반드시 호출하세요.
- `body()`: 시뮬레이션 내용. 사용자가 정의해야 합니다.
- `step(n)`: 시뮬레이션 n단계(기본: 1) 진행
- `check(expected, actual, msg)`: `expected` 값이 `actual` 값과 같은지 확인하고 stdout에 결과를 `msg`와 함께 출력
- `success(msg)`: 성공 메시지를 출력
- `fail(msg)`: 실패 메시지를 출력
- `report()`: 시뮬레이션 결과 요약을 출력
- `t`: 현재 시뮬레이션 시간
- `m`: 시뮬레이션 대상 모듈

`CombiSimulator`는 combinational logic 테스트를 위한 서브클래스, `SeqSimulator`는 sequential logic 테스트를 위한 서브클래스입니다. `SeqSimulator`는 다음과 같은 추가적인 메소드들을 가집니다.

- `clock()`: `=step(2)`
- `reset()`: 모듈에 리셋 신호를 보냅니다. 총 2사이클이 소요됩니다.

시뮬레이션은 `CombiSimulator` 또는 `SeqSimulator`를 확장한 클래스를 만들고, 그 클래스의 `main()` 메소드를 호출함으로써 실행할 수 있습니다. `main()` 메소드는 `init()`, `body()`후 `report()`를 순서대로 호출하고, `main` 함수가 반환할 수 있는 리턴 코드를 반환합니다.

```cpp
#include "Simulator.h"
#include "Vmy_module.h"

class MyModuleSimulator : public CombiSimulator<Vmy_module> {
public:
    MyModuleSimulator() : CombiSimulator("my_module") {}
    virtual void body() {
        // 시뮬레이션 스크립트
    }
}

int main() {
    MyModuleSimulator s;
    return s.main();
}
```

`COMBI_SIMULATOR`와 `SEQ_SIMULATOR` 메크로를 이용해 위 코드를 더욱 간결하게 쓸 수 있습니다.

```cpp
// (includes)

COMBI_SIMULATOR(MyModule, my_module, {
    // 시뮬레이션 스크립트
})
```

CombiSimulator의 예제로는 `Lab1/vending_machine_working` 프로젝트의 `tb_item_dispenser.cpp`를, SeqSimulator의 예제로는 `tb_timer.cpp`를 참조하세요.


import os
import shutil
import subprocess

tests = [
    ("basic", "./student_tb/answer_basic.txt", "./student_tb/basic_mem.txt"),
    ("ifelse", "./student_tb/answer_ifelse.txt", "./student_tb/ifelse_mem.txt"),
    ("loop", "./student_tb/answer_loop.txt", "./student_tb/loop_mem.txt"),
    ("non-controlflow", "./student_tb/answer_non-controlflow_with_data_forwarding.txt", "./student_tb/non-controlflow_mem.txt"),
    ("recursive", "./student_tb/answer_recursive.txt", "./student_tb/recursive_mem.txt"),
    ("naive_matmul", "./Lab5_student_tb/naive_ripes_result.txt", "./Lab5_student_tb/naive_matmul_unroll.mem"),
    ("opt_matmul", "./Lab5_student_tb/opt_ripes_result.txt", "./Lab5_student_tb/opt_matmul_unroll.mem"),
]

for name, txt_path, mem_path in tests:
    shutil.copy(txt_path, "test.txt")
    shutil.copy(mem_path, "test.mem")

    proc = subprocess.run("make clean && make", shell=True, capture_output=True, text=True)
    output = proc.stdout.strip().splitlines()

    if output:
        print(f"==== {name} ====")
        print(output[-35])
        print(output[-1])
        print()

os.remove("test.txt")
os.remove("test.mem")

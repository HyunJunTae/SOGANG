#!/usr/bin/python3
import sys, os, subprocess
from os import path
from subprocess import PIPE


ROOT_DIR = path.dirname(path.abspath(__file__))
CONFIG_FILE = "config"
MAX_RUNTIME = 5
DELAY_PENALTY = 0.8
SOLVE_STRING = "You passed the challenge!"


def run_cmd(cmd_str, check=True, timeout=None):
    args = cmd_str.split()
    p = subprocess.run(args, check=check, stdout=PIPE, stderr=PIPE,
                       timeout=timeout)
    return "".join(map(chr, p.stdout))


def check(problem_name, submit_file, point, is_delay):
    src_path = path.join(ROOT_DIR, problem_name, submit_file)
    if not path.isfile(src_path): # Not submitted
        return (" ", 0.0)

    # Ensure that the python source file has 'x' permission.
    os.chmod(src_path, 0o755)

    # Backup current working directory, and move to problem directory. This is
    # necessary because the script uses relative path to run the target program.
    orig_cwd = os.getcwd()
    os.chdir(path.join(ROOT_DIR, problem_name))

    try:
        output = run_cmd("./%s" % submit_file)
        if SOLVE_STRING in output:
            grading_str = "O"
        else:
            grading_str = "X"
    except subprocess.TimeoutExpired:
        grading_str = "T"
    except subprocess.CalledProcessError as e:
        grading_str = "E"

    os.chdir(orig_cwd)

    obtained_point = point if grading_str == "O" else 0
    if is_delay:
        grading_str += " (Delay)"
        obtained_point *= DELAY_PENALTY
    return grading_str, obtained_point


def parse_config():
    f = open(path.join(ROOT_DIR, CONFIG_FILE))
    problem_list = []
    for line in f:
        tokens = line.strip().split()
        problem_name = tokens[0]
        submit_file = tokens[1]
        point = int(tokens[2])
        problem_list.append((problem_name, submit_file, point))
    f.close()
    return problem_list


def main():
    if len(sys.argv) not in [1, 2]:
        # --delay or --normal option is hidden.
        print("Usage: %s" % sys.argv[0])
        exit(1)

    delay_flag = False
    csv_flag = False
    if len(sys.argv) == 2:
        csv_flag = True
        if sys.argv[1] == "--delay":
            delay_flag = True
        elif sys.argv[1] == "--normal":
            pass # Nothing to do if it's --normal.
        else:
            print("Invalid option: %s" % sys.argv[1])
            exit(1)

    problem_list = parse_config()
    for (problem_name, submit_file, point) in problem_list:
        grading_str, obtained_point = \
            check(problem_name, submit_file, point, delay_flag)
        if csv_flag:
            print("%s, %.1f, " % (grading_str, obtained_point), end="")
        else:
            print("[*] %s: %s" % (problem_name, grading_str))


if __name__ == "__main__":
    main()

#! /usr/bin/env nix-shell
#! nix-shell -i python3 -p python3 tcsh

from __future__ import annotations
from dataclasses import dataclass

import difflib
import subprocess
import sys


@dataclass
class Result:
    stdout: str
    stderr: str
    exit_code: int


def run_shell(shell_cmd, user_cmd, timeout=2) -> Result:
    process = subprocess.Popen(
        shell_cmd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    try:
        stdout, stderr = process.communicate(user_cmd + "\n", timeout=timeout)
    except subprocess.TimeoutExpired:
        process.kill()
        stdout, stderr = process.communicate()

    exit_code = process.returncode
    return Result(stdout.strip(), stderr.strip(), exit_code)


def print_diff(cmd: str, out: Result, exp: Result):
    print(f"\nFail: \033[34m{cmd!r}\033[0m") # ]]

    if out.stdout != exp.stdout:
        print("\n  \033[36m--- STDOUT diff ---\033[0m") # ]]
        diff = difflib.unified_diff(
            out.stdout.splitlines(), exp.stdout.splitlines(),
            fromfile="tcsh", tofile="42sh", lineterm="")
        print("  " + "\n  ".join(list(diff)[2:]))

    if out.stderr != exp.stderr:
        print("\n  \033[36m--- STDERR diff ---\033[0m") # ]]
        diff = difflib.unified_diff(
            out.stderr.splitlines(), exp.stderr.splitlines(),
            fromfile="tcsh", tofile="42sh", lineterm="")
        print("  " + "\n  ".join(list(diff)[2:]))

    elif out.exit_code != exp.exit_code:
        print("\n  \033[36m--- EXIT CODE mismatch ---\033[0m\n" # ]]
            f"42sh: {out.exit_code} | tcsh: {exp.exit_code}")

    print("\n")


class Test:

    def __init__(
        self,
        key: str,
        name: str,
        cmds: list[str],
        depends_on: tuple[str, ...] = (),
    ):
        self.key = key
        self.name = name
        self.cmds = cmds
        self.depends_on = depends_on
        self.has_run = False

    def _run_each_cmd(self, cmd: str, tested_bin):
        result_42sh = run_shell([tested_bin], cmd)
        result_tcsh = run_shell(["tcsh"], cmd)

        if result_42sh.exit_code == 84 and result_tcsh.exit_code != 0:
            result_tcsh.exit_code = 84

        if result_42sh == result_tcsh:
            print("\033[32m.\033[0m", end='') # ]]
            return None

        print("\033[31m.\033[0m", end='') # ]]
        return cmd, result_42sh, result_tcsh

    def run(self, test_map, tested_bin) -> bool:
        if self.has_run:
            return True

        self.has_run = True

        success = True
        for dep_name in self.depends_on:
            dep = test_map.get(dep_name)

            if dep is None:
                print("\033[33mWarning\033[0m:" # ]]
                    "Missing dependency:", dep_name)
                continue

            if not dep.has_run:
                success &= dep.run(test_map, tested_bin)
                if not success:
                    return False

        print(self.name, end=" ")
        failures = []

        for cmd in self.cmds:
            if (failure := self._run_each_cmd(cmd, tested_bin)) is not None:
                failures.append(failure)
        if not failures:
            print(" \033[32mOK\033[0m") # ]]
            return True
        else:
            print()
        for fail in failures:
            print_diff(*fail)
        return False

def main():
    from validation_tests import TESTS
 
    test_map = {test.key: test for test in TESTS}

    success = True
    for test in TESTS:
        success &= test.run(
            test_map=test_map,
            tested_bin=sys.argv[1] if len(sys.argv) > 1 else "./42sh"
        )

    return not success

if __name__ == "__main__":
    sys.exit(main())

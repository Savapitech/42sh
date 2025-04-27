from validator import Test


TESTS = [
  Test(
    key="SIMPLE",
    name="simple commands",
    cmds=[
        "ls\n",
        "uname\n",
        "who\n",
    ]
  ),
  Test(
    key="ARGS",
    name="multiple arguments",
    cmds=[
        "echo kek\n",
        "ls --all\n",
        "cat -e Makefile\n",
        "find . -type f\n",
        "ls -a . . .\n",
        "wc --chars --words --lines Makefile\n",
    ],
    depends_on=("SIMPLE",)
  ),
  Test(
    key="BAD CMD",
    name="invalid commands",
    cmds=[
        "plopdjksjesi\n",
        "ls -PQRT\n",
    ],
    depends_on=("ARGS",)
  ),
  Test(
    key="PATH",
    name="path handing",
    cmds=[
        "/bin/sh --version\n",
        "/../bin/sh --version\n",
        "~/../../bin/sh --version\n",
        "fixtures/exec.sh\n",
    ],
    depends_on=("ARGS",)
  ),
  Test(
    key="ENV",
    name="setenv / unsetenv",
    cmds=[
        "setenv Hello plop\n",
        "setenv 1 nope\n",
        "setenv @\n",
    ],
    depends_on=("ARGS",)
  ),
  Test(
    key="PWD",
    name="pwd",
    cmds=["pwd\n"],
    depends_on=("SIMPLE",)
  ),
  Test(
    key="SEMICOLON",
    name="semicolon",
    cmds=[
        ";\n",
        "echo a; echo b\n",
        "echo a;; echo b\n",
        "echo a; echo b;\n",
    ],
    depends_on=("ARGS",)
  ),
  Test(
    key="CD",
    name="cd builtin",
    cmds=[
        "cd /; pwd\n",
        "cd; pwd\n",
        "cd ~; pwd\n",
        "cd .; pwd\n",
        "cd ..; pwd\n",
        "cd /..; pwd\n",
        "cd a\n",
        "cd . .\n",
        "cd src/; pwd\n",
    ],
    depends_on=("ARGS", "PWD", "SEMICOLON")
  ),
  Test(
    key="FMT",
    name="line formatting (space and tabs)",
    cmds=[
        " echo plop\n",
        "\techo plop\n",
        " \techo plop\n",
        "echo\tplop\n",
        "echo    plop\n",
        "echo\t\tplop\n",
        "echo plop\t \n",
    ],
    depends_on=("ARGS",)
  ),
]

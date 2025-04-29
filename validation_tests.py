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
        "/bin/ls\n",
        "/../bin/ls\n",
        # "~/../../bin/sh --version\n",
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

    Test(
        key="REDIR",
        name="I/O redirections",
        cmds=[
            "echo plop > out.txt\n",
            "cat < out.txt\n",
            "echo again >> out.txt\n",
            "cat < out.txt\n",
            "cat nofile.txt\n",
            "echo test > /no/perm/file\n",
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="PIPE",
        name="pipes",
        cmds=[
            "echo plop | cat\n",
            "echo kek | grep kek\n",
            "ls | grep Makefile\n",
            "who | wc -l\n",
            "ls | | cat\n",  # Syntax error
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="ENV_EXPANSION",
        name="environment variable expansion",
        cmds=[
            "echo $HOME\n",
            "echo $PATH\n",
            "setenv TESTVAR bonjour;echo $TESTVAR;unsetenv TESTVAR;echo $TESTVAR\n",
        ],
        depends_on=("ENV","SEMICOLON",)
    ),

    Test(
        key="EXIT",
        name="exit command",
        cmds=[
            "exit\n",
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="MULTICMD",
        name="multiple commands per line",
        cmds=[
            "echo one ; echo two ; echo three\n",
            "ls ; pwd ; whoami\n",
            "cd src/ ; pwd ; cd - ; pwd\n",
        ],
        depends_on=("SEMICOLON",)
    ),

    Test(
        key="PARSING_ERR",
        name="bad parsing cases",
        cmds=[
            "ls |\n",
            "ls >\n",
            "; ls\n",
            ";; ls\n",
            "ls ;; ls\n",
        ],
        depends_on=("PIPE", "REDIR", "SEMICOLON",)
    ),
]

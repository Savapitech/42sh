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
        "~/../../bin/ls\n",
        "setenv PATH /\nbin/ls\n",
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
            "ls | | cat\n",
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

    Test(
        key="ALIAS",
        name="alias",
        cmds=[
            "ll\nalias ll ls\nll\n",
            "alias ls ls\nls\nalias\n",
            "alias a b\nalias b a\na\n",
            "ll\n",
        ],
        depends_on=()
    ),

    Test(
        key="AND_OR",
        name="and or",
        cmds=[
            "ls&&ls\n",
            "bb&&ls\n",
            "bb&&bb\n",
            "ls&&bb\n",
            "ls||ls\n",
            "bb||ls\n",
            "bb||bb\n",
            "ls||bb\n",
            "&&\n",
            "ls && ls || ls && ls\n",
            "ls && ls b || ls && ls\n",
            "ls && ls || ls && ls b\n",
            "ls && ls || ls && b\n",
            "ls bb && ls || ls\n",
        ],
        depends_on=()
    ),

    Test(
        key="BACKTICKS",
        name="backticks",
        cmds=[
            "echo `ls`\n",
            "echo `ls\n",
            "echo ls`\n",
            "echo ls `ls`\n",
        ],
        depends_on=()
    ),

    Test(
        key="IF",
        name="if",
        cmds=[
            "if (5 > 2) ls\n",
            "if (56 < 2) ls\n",
            "if (1 == 1) then\necho YES\nelse\necho NO\nendif\n",
            "if (1 < 2) then\necho YES\nelse\necho NO\nendif\nif 1 ls\n",
            "if (1 != 1) then\necho YES\nelse\necho NO\nendif\n",
            "if\n",
            "if 0\n",
        ],
        depends_on=()
    ),

    Test(
        key="EXPR",
        name="expr builtin command",
        cmds=[
            "expr 1 + 2",
            "expr 4 - 2",
            #"expr 3 \\* 5",
            #"expr 5 = 5",
            #"expr 5 \\< 10",
            #"expr 5 \\> 3",
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="TILDE",
        name="tilde",
        cmds=[
            "echo ~",
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="GLOB",
        name="globbing",
        cmds=[
            "ls *\n",
            "echo *.nix\n",
            "echo *file*\n", 
            "echo fixtures/*.sh\n",
            "echo doesnotexist*\n",
            "ls *.c *.h\n",
            "echo *.txt > out.txt\n",
            "cat < out.txt\n",
            ],
        depends_on=("REDIR",)
    ),

    Test(
        key="REPEAT",
        name="repeat loop",
        cmds=[
            "repeat 3 echo plop\n",
            "repeat 3\n",
            "repeat\n",
            ],
        depends_on=("ARGS",)
    ),

    Test(
        key="FOREACH",
        name="foreach loop",
        cmds=[
            "foreach i (1 2 3)\necho $i\nend\n",
            "foreach i",
            "foreach",
        ],
        depends_on=("ARGS",)
    ),

    Test(
        key="WHICH",
        name="which/where commands",
        cmds=[
            "which ls\n",
            "which cd\n",
            "which echo\n",
            "where ls\n",
            "where cd\n",
            "where echo\n",
            "alias ll ls\nwhere ll\n",
            #"alias ll ls\nwhich ll\n",
        ],
        depends_on=("ALIAS",)
    ),

    Test(
        key="BUILTIN_ERR",
        name="invalid builtin usage",
        cmds=[
            "cd too many args here\n",
            "unsetenv\n",
            "setenv ONLYKEY\n",
            "echo\n",
            "echo a\n",
            "echo a b\n",
            "echo ab\n",
            "echo -e\n",
            "echo -n\n",
            "echo aa -e\n",
            "echo -n aa\n",
            "echo $PATH\n",
            ],
        depends_on=("CD", "ENV")
    ),

    Test(
        key="SPECIAL_VARS",
        name="special variables",
        cmds=[
            "echo $term\n",
            "echo $cwd\n",
            "ncd /\n",
            ],
        depends_on=()
    ),
]

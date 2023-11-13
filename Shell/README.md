# Shell

## Pipeline
- [x] Lexer (Task 5_1)
- [ ] Parser (Task 5_2 — 5_all) — in progress
- [ ] Interpreter (Task 5_2 — 5_all)

```
<shell_cmd> ::= <cmd_list>
<cmd_list> ::= <pipe> {["&" | "&&" | "||" | ";"] <pipe>} ["&" | ";"]
<pipe> ::= <cmd> {"|" <cmd>}
<cmd> ::= (<cmd_list>) <io_redireсtion>
<io_redireсtion> ::= ["<" <file_name>] [[">" | "»"] <file_name>] |
    [[">" | "»"] <file_name>] ["<" <file_name>]
```
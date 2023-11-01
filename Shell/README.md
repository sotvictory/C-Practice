# Shell

## Pipeline
- [x] Lexer
- [ ] Parser
- [ ] Interpreter

## Lexer L-Graph

## Parser BNF
```
<shell_cmd> ::= <cmd_list>
<cmd_list> ::= <pipe> {["&" | "&&" | "||" | ";"] <pipe>} ["&" | ";"]
<pipe> ::= <cmd> {"|" <cmd>}
<cmd> ::= (<cmd_list>) <io_redireсtion>
<io_redireсtion> ::= ["<" <file_name>] [[">" | "»"] <file_name>] |
    [[">" | "»"] <file_name>] ["<" <file_name>]
```
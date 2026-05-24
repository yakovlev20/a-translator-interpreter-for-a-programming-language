### 1. DFA State Mapping

| State | Type | Description / Token Caught |
| :--- | :--- | :--- |
| **S0** | Start | Initial state / Reading whitespace |
| **S1** | Accepting | **Identifier / Keyword** (Letter followed by letters/digits) |
| **S2** | Accepting | **Integer** (Sequence of digits) |
| **S3** | Trapping | Read a `.` after an integer; expecting fractional digits |
| **S4** | Accepting | **Float** (Digits, period, digits) |
| **S5** | Trapping | Read `:` ; expecting `=` to complete assignment |
| **S6** | Accepting | **Assignment Operator** (`:=`) |
| **S7** | Trapping | Read `!` ; expecting `=` to complete inequality |
| **S8** | Accepting | **Inequality Operator** (`!=`) |
| **S9** | Accepting | **Single-character operators/punctuation** (`=`, `<`, `>`, `+`, `-`, `*`, `/`, `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`) |
| **SE** | Error | Lexical error state |

---

### 2. Finite Automata Transition Table

*   **[Letter]** = `[a-zA-Z]`
*   **[Digit]** = `[0-9]`
*   **\*Term\*** = Terminate current token, emit it, do **not** consume the character, and reset the DFA pointer back to **S0**.

| Current State | [Letter] | [Digit] | `.` | `:` | `!` | `=` | `+`, `-`, `*`, `/`, `(`, `)`, `{`, `}`, `[`, `]`, `,`, `;` | `<`, `>` | Whitespace |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **S0** | S1 | S2 | SE | S5 | S7 | S9 | S9 | S9 | S0 |
| **S1 (ID)** | S1 | S1 | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |
| **S2 (Int)** | *Term* | S2 | S3 | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |
| **S3** | SE | S4 | SE | SE | SE | SE | SE | SE | SE |
| **S4 (Float)**| *Term* | S4 | SE | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |
| **S5** | SE | SE | SE | SE | SE | S6 | SE | SE | SE |
| **S6 (:=)** | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |
| **S7** | SE | SE | SE | SE | SE | S8 | SE | SE | SE |
| **S8 (!=)** | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |
| **S9 (Op)** | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* | *Term* |




### Greibach Normal Form (GNF) Conversion

# --- Main Block and Structure ---
A -> begin X_End
X_End -> { X_Brace
X_Brace -> } end | var B_Var X_Brace | const B_Const X_Brace | array B_Arr X_Brace | [ID] B_Id X_Brace | while B_While X_Brace | if B_If X_Brace | print B_Print X_Brace | get B_Get X_Brace

# --- Statement Disambiguation (B Rules) ---
B_Var   -> [ID] H_Rule := G
B_Const -> [ID] H_Rule := G
B_Arr   -> [ID] H_Rule := G
B_Id    -> H_Rule := G
B_While -> ( C ) { X_Brace
B_If    -> ( C ) then { X_Brace E

# --- Conditionals & Arrays (E, H, G, J, K) ---
E -> else { X_Brace
H -> [ Q ] | := G
G -> { J K } | Q_Terminal Q_Tail | ( Q ) Q_Tail | [NUM] Z Q_Tail | [ID] Q_Tail
K -> , J K | } 

J -> Q_Terminal Q_Tail | ( Q ) Q_Tail | [NUM] Z Q_Tail | [ID] Q_Tail | [ID] | [NUM] Z

# --- Input / Output (W Rules) ---
B_Print -> ( Q ) | ( [ID] )
B_Get   -> ( [ID] )

# --- Conditionals (C Rules) ---
C -> Q_Terminal Q_Tail Comp_Op Q | ( Q ) Q_Tail Comp_Op Q | [NUM] Z Q_Tail Comp_Op Q | [ID] Q_Tail Comp_Op Q
Comp_Op -> < | > | = | !=

# --- Expressions (Q, T, F Rules in GNF) ---
# Left recursion in Q -> Q + T and T -> T * F is converted to right-linear structures
Q        -> Q_Terminal Q_Tail | ( Q ) Q_Tail | [NUM] Z Q_Tail | [ID] Q_Tail
Q_Terminal -> ( Q ) | [NUM] Z | [ID]

Q_Tail   -> + T Q_Tail | - T Q_Tail | * F T_Tail Q_Tail | / F T_Tail Q_Tail | \lambda
T_Tail   -> * F T_Tail | / F T_Tail | \lambda

# --- Numbers & Identifiers ---
Z -> . [NUM] | \lambda
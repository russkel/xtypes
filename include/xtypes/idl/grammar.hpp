/*
 * Copyright 2019, Proyectos y Sistemas de Mantenimiento SL (eProsima).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef EPROSIMA_XTYPES_IDL_GRAMMAR_HPP_
#define EPROSIMA_XTYPES_IDL_GRAMMAR_HPP_

namespace eprosima {
namespace xtypes {
namespace idl {

/// \brief Gives a peg grammar for IDL 4.2
/// \return The idl peg grammar.
inline const char* idl_grammar()
{
    static const char* IDL_GRAMMAR = R"(
# Tested at: https://yhirose.github.io/cpp-peglib/

# Building Block Core Data Types:
DOCUMENT <- WS? SPECIFICATION WS?
SPECIFICATION <- DEFINITION+
DEFINITION <- (MODULE_DCL / CONST_DCL / TYPE_DCL / ANNOTATION_DCL) SEMICOLON / PREPROCESSOR_DIRECTIVE

MODULE_DCL <- (ANNOTATION_APPL)? KW_MODULE IDENTIFIER OPEN_BRACE DEFINITION+ CLOSE_BRACE
#SCOPED_NAME <- DOUBLE_COLON IDENTIFIER / (IDENTIFIER DOUBLE_COLON)* IDENTIFIER
SCOPED_NAME <- < IDENTIFIER DOUBLE_COLON SCOPED_NAME / DOUBLE_COLON SCOPED_NAME / IDENTIFIER >

PREPROCESSOR_DIRECTIVE <- WS? < '#' (NL_ESCAPE / !EOL .)* > EOL
EOL <- '\r\n' / '\n' / '\r' / !.
NL_ESCAPE <- '\\' EOL

COMMENT <- LINE_COMMENT / BLOCK_COMMENT
LINE_COMMENT <- [ \n]* KW_LINE_COMMENT (NL_ESCAPE / !EOL .)* EOL WS?
BLOCK_COMMENT <- [ \n]* KW_OPEN_BLOCK_COMMENT (!KW_CLOSE_BLOCK_COMMENT .)* KW_CLOSE_BLOCK_COMMENT WS?

IDENTIFIER <- [a-zA-Z_][0-9a-zA-Z_]*

CONST_DCL <- KW_CONST CONST_TYPE WS? IDENTIFIER ~EQUAL_OP CONST_EXPR

CONST_TYPE <-
    FLOAT_TYPE /
    DOUBLE_TYPE /
    LONG_DOUBLE_TYPE /
    FIXED_PT_CONST_TYPE /
    INTEGER_TYPE /
    CHAR_TYPE /
    WIDE_CHAR_TYPE /
    BOOLEAN_TYPE /
    STRING_TYPE /
    WIDE_STRING_TYPE /
    SCOPED_NAME

#CONST_EXPR <- OR_EXPR
#OR_EXPR <- XOR_EXPR OR_OP OR_EXPR / XOR_EXPR

CONST_EXPR <- XOR_EXPR OR_OP CONST_EXPR / XOR_EXPR
XOR_EXPR <- AND_EXPR XOR_OP XOR_EXPR / AND_EXPR
AND_EXPR <- SHIFT_EXPR AND_OP AND_EXPR / SHIFT_EXPR
SHIFT_EXPR <- ADD_EXPR ANY_SHITF_OP SHIFT_EXPR / ADD_EXPR
ADD_EXPR <- MULT_EXPR ANY_ADD_OP ADD_EXPR / MULT_EXPR
MULT_EXPR <- UNARY_EXPR ANY_MULT_OP MULT_EXPR / UNARY_EXPR
UNARY_EXPR <- UNARY_OP PRIMARY_EXPR / PRIMARY_EXPR
PRIMARY_EXPR <- OPEN_PARENTHESES CONST_EXPR CLOSE_PARENTHESES / SCOPED_OR_LITERAL
SCOPED_OR_LITERAL <- LITERAL / SCOPED_NAME

ANY_SHITF_OP <- LSHIFT_OP / RSHIFT_OP
ANY_ADD_OP <- ADD_OP / SUB_OP
ANY_MULT_OP <- MULT_OP / DIV_OP / MOD_OP

LITERAL <- FLOAT_LITERAL /
    FIXED_PT_LITERAL /
    INTEGER_LITERAL /
    CHAR_LITERAL /
    WIDE_CHAR_LITERAL /
    BOOLEAN_LITERAL /
    STRING_LITERAL /
    WIDE_STRING_LITERAL

POSITIVE_INT_CONST <- INTEGER_LITERAL / SCOPED_NAME

TYPE_DCL <- CONSTR_TYPE_DLC / NATIVE_DCL / TYPEDEF_DLC
TYPE_SPEC <- WS? (TEMPLATE_TYPE_SPEC / SIMPLE_TYPE_SPEC) WS?
SIMPLE_TYPE_SPEC <- BASE_TYPE_SPEC / SCOPED_NAME
BASE_TYPE_SPEC <-
    FLOAT_TYPE /
    DOUBLE_TYPE /
    LONG_DOUBLE_TYPE /
    INTEGER_TYPE /
    CHAR_TYPE /
    WIDE_CHAR_TYPE /
    BOOLEAN_TYPE /
    OCTET_TYPE /
    ANY_TYPE
FLOAT_TYPE <- KW_FLOAT
DOUBLE_TYPE <- KW_DOUBLE
LONG_DOUBLE_TYPE <- KW_LONG_DOUBLE
INTEGER_TYPE <- SIGNED_INT / UNSIGNED_INT
SIGNED_INT <- SIGNED_SHORT_INT / SIGNED_LONGLONG_INT / SIGNED_LONG_INT / SIGNED_TINY_INT
UNSIGNED_INT <- UNSIGNED_SHORT_INT / UNSIGNED_LONGLONG_INT / UNSIGNED_LONG_INT / UNSIGNED_TINY_INT
SIGNED_SHORT_INT <- KW_SHORT / KW_INT16
SIGNED_LONG_INT <- KW_LONG / KW_INT32
SIGNED_LONGLONG_INT <- KW_LONG_LONG / KW_INT64
UNSIGNED_SHORT_INT <- KW_UNSIGNED_SHORT / KW_UINT16
UNSIGNED_LONG_INT <- KW_UNSIGNED_LONG / KW_UINT32
UNSIGNED_LONGLONG_INT <- KW_UNSIGNED_LONG_LONG / KW_UINT64
SIGNED_TINY_INT <- KW_INT8
UNSIGNED_TINY_INT <- KW_UINT8
CHAR_TYPE <- KW_CHAR
WIDE_CHAR_TYPE <- KW_WCHAR
BOOLEAN_TYPE <- KW_BOOLEAN
OCTET_TYPE <- KW_OCTET
ANY_TYPE <- KW_ANY
TEMPLATE_TYPE_SPEC <- MAP_TYPE / SEQUENCE_TYPE / STRING_TYPE / WIDE_STRING_TYPE / FIXED_PT_TYPE
SEQUENCE_TYPE <- KW_SEQUENCE OPEN_ANG_BRACKET SEQUENCE_INNER_TYPE SEQUENCE_SIZE CLOSE_ANG_BRACKET
SEQUENCE_INNER_TYPE <- TYPE_SPEC
SEQUENCE_SIZE <- COLLECTION_SIZE?
STRING_TYPE <- KW_STRING STRING_SIZE?
WIDE_STRING_TYPE <- KW_WSTRING WSTRING_SIZE?
FIXED_PT_TYPE <- KW_FIXED OPEN_ANG_BRACKET POSITIVE_INT_CONST COMMA POSITIVE_INT_CONST CLOSE_ANG_BRACKET
FIXED_PT_CONST_TYPE <- KW_FIXED

STRING_SIZE <- OPEN_ANG_BRACKET < POSITIVE_INT_CONST > CLOSE_ANG_BRACKET
WSTRING_SIZE <- OPEN_ANG_BRACKET < POSITIVE_INT_CONST > CLOSE_ANG_BRACKET
COLLECTION_SIZE <- COMMA POSITIVE_INT_CONST

CONSTR_TYPE_DLC <- (ANNOTATION_APPL)? (STRUCT_DCL / UNION_DCL / ENUM_DCL / BITSET_DCL / BITMASK_DCL)
STRUCT_DCL <- STRUCT_DEF / STRUCT_FORWARD_DCL
STRUCT_DEF <- KW_STRUCT IDENTIFIER INHERITANCE? OPEN_BRACE MEMBER* CLOSE_BRACE
INHERITANCE <- COLON SCOPED_NAME
MEMBER <- (ANNOTATION_APPL)? TYPE_SPEC DECLARATORS SEMICOLON
STRUCT_FORWARD_DCL <- KW_STRUCT < IDENTIFIER >
UNION_DCL <- UNION_DEF / UNION_FORWARD_DCL
UNION_DEF <- KW_UNION IDENTIFIER KW_SWITCH OPEN_PARENTHESES SWITCH_TYPE_SPEC CLOSE_PARENTHESES
    OPEN_BRACE SWITCH_BODY CLOSE_BRACE
SWITCH_TYPE_SPEC <- INTEGER_TYPE / CHAR_TYPE / BOOLEAN_TYPE / WIDE_CHAR_TYPE / OCTET_TYPE / SCOPED_NAME
SWITCH_BODY <- CASE+
CASE <- CASE_LABEL+ ELEMENT_SPEC SEMICOLON
CASE_LABEL <- KW_CASE < CONST_EXPR > COLON / < KW_DEFAULT > COLON
ELEMENT_SPEC <- (ANNOTATION_APPL)? TYPE_SPEC DECLARATOR
UNION_FORWARD_DCL <- KW_UNION < IDENTIFIER >
ENUM_DCL <- KW_ENUM IDENTIFIER OPEN_BRACE ENUMERATOR (COMMA ENUMERATOR)* CLOSE_BRACE
ENUMERATOR <- (ANNOTATION_APPL)? IDENTIFIER
ARRAY_DECLARATOR <- IDENTIFIER FIXED_ARRAY_SIZE+
FIXED_ARRAY_SIZE <- OPEN_BRACKET POSITIVE_INT_CONST CLOSE_BRACKET
NATIVE_DCL <- KW_NATIVE SIMPLE_DECLARATOR
SIMPLE_DECLARATOR <- IDENTIFIER
TYPEDEF_DLC <- KW_TYPEDEF TYPE_DECLARATOR
TYPE_DECLARATOR <- (CONSTR_TYPE_DLC / TEMPLATE_TYPE_SPEC / SIMPLE_TYPE_SPEC) WS? ANY_DECLARATORS
ANY_DECLARATORS <- ANY_DECLARATOR (COMMA ANY_DECLARATOR)*
ANY_DECLARATOR <- ARRAY_DECLARATOR / SIMPLE_DECLARATOR /
DECLARATORS <- DECLARATOR (COMMA DECLARATOR)*
DECLARATOR <- ARRAY_DECLARATOR / SIMPLE_DECLARATOR

# XTYPES
MAP_TYPE <- KW_MAP OPEN_ANG_BRACKET MAP_KEY_TYPE COMMA MAP_INNER_TYPE MAP_SIZE CLOSE_ANG_BRACKET
MAP_INNER_TYPE <- TYPE_SPEC
MAP_KEY_TYPE <- TYPE_SPEC
MAP_SIZE <- COLLECTION_SIZE?
BITSET_DCL <- KW_BITSET IDENTIFIER INHERITANCE? OPEN_BRACE BITFIELD* CLOSE_BRACE
BITFIELD <- (ANNOTATION_APPL)? BITFIELD_SPEC IDENTIFIER* SEMICOLON
BITFIELD_SPEC <- KW_BITFIELD OPEN_ANG_BRACKET POSITIVE_INT_CONST (COMMA DESTINATION_TYPE)? CLOSE_ANG_BRACKET
DESTINATION_TYPE <- BOOLEAN_TYPE / OCTET_TYPE / INTEGER_TYPE
BITMASK_DCL <- KW_BITMASK IDENTIFIER OPEN_BRACE BIT_VALUE (COMMA BIT_VALUE)* CLOSE_BRACE
BIT_VALUE <- (ANNOTATION_APPL WS?)? IDENTIFIER

# Annotations
ANNOTATION_DCL <- ANNOTATION_HEADER OPEN_BRACE ANNOTATION_BODY CLOSE_BRACE
ANNOTATION_HEADER <- KW_ANNOTATION IDENTIFIER
ANNOTATION_BODY <- (ANNOTATION_MEMBER / ENUM_DCL SEMICOLON / CONST_DCL SEMICOLON / TYPEDEF_DLC SEMICOLON)*
ANNOTATION_MEMBER <- WS? ANNOTATION_MEMBER_TYPE WS? SIMPLE_DECLARATOR (KW_DEFAULT CONST_EXPR)? SEMICOLON
ANNOTATION_MEMBER_TYPE <- CONST_TYPE / ANY_CONST_TYPE / SCOPED_NAME
ANY_CONST_TYPE <- KW_ANY
ANNOTATION_APPL <- "@" SCOPED_NAME ( OPEN_PARENTHESES ANNOTATION_APPL_PARAMS CLOSE_PARENTHESES )?
ANNOTATION_APPL_PARAMS <- ANNOTATION_APPL_PARAM ( COMMA ANNOTATION_APPL_PARAM )* / CONST_EXPR
ANNOTATION_APPL_PARAM <- IDENTIFIER EQUAL_OP CONST_EXPR

# LITERALS
BOOLEAN_LITERAL <- < "TRUE" / "FALSE" >
INTEGER_LITERAL <- < OCT_LITERAL / HEX_LITERAL / DEC_LITERAL >
DEC_LITERAL <- [1-9] DECIMAL_DIGIT* / '0'
OCT_LITERAL <- '0' OCTAL_DIGIT+
HEX_LITERAL <- '0' [xX] HEX_DIGIT+
CHAR_LITERAL <- < "'" (!["'"\\] .)+ "'" > / "'" ESCAPE_SEQUENCE "'"
WIDE_CHAR_LITERAL <- < 'L' CHAR_LITERAL >
# String literals must avoid '\0' inside them. Check after parsing!
STRING_LITERAL <- ( SUBSTRING_LITERAL )+
SUBSTRING_LITERAL <- < '"' (ESCAPE_SEQUENCE / (!['"'\\] .))* '"' > STRING_WS
~STRING_WS <- '"' WS? '"' / WS?
WIDE_STRING_LITERAL <- ('L' WIDE_SUBSTRING_LITERAL)+
WIDE_SUBSTRING_LITERAL <- < SUBSTRING_LITERAL >
FLOAT_LITERAL <- <
    DECIMAL_DIGIT+ '.' DECIMAL_DIGIT* EXP_OR_SUFFIX? /
    '.' DECIMAL_DIGIT+ EXP_OR_SUFFIX? /
    DECIMAL_DIGIT+ EXP_OR_SUFFIX
    >
FIXED_PT_LITERAL <- < FLOAT_LITERAL >

EXP_OR_SUFFIX <- EXPONENT / FLOAT_TYPE_SUFFIX
EXPONENT <- < [eE] [-+]? DECIMAL_DIGIT+ >
FLOAT_TYPE_SUFFIX <- [dD]

# ESCAPE SEQUENCES
~ESCAPE_SEQUENCE <- ES_NEW_LINE /
    ES_H_TAB /
    ES_V_TAB /
    ES_BACKSPACE /
    ES_CARRIAGE_RETURN /
    ES_FORM_FEED /
    ES_ALERT /
    ES_BACKSLASH /
    ES_QUESTION_MARK /
    ES_SINGLE_QUOTE /
    ES_DOUBLE_QUOTE /
    ES_OCTAL /
    ES_HEX /
    ES_UNICODE

~ES_NEW_LINE <- "\\n"
~ES_H_TAB <- "\\t"
~ES_V_TAB <- "\\v"
~ES_BACKSPACE <- "\\b"
~ES_CARRIAGE_RETURN <- "\\r"
~ES_FORM_FEED <- "\\f"
~ES_ALERT <- "\\a"
~ES_BACKSLASH <- "\\\\"
~ES_QUESTION_MARK <- "\\?"
~ES_SINGLE_QUOTE <- "\\'"
~ES_DOUBLE_QUOTE <- '\\"'
~ES_OCTAL <- "\\" ES_OCTAL_NUMBER
~ES_HEX <- "\\x" ES_HEX_NUMBER
~ES_UNICODE <- "\\u" ES_UNICODE_CHAR
~ES_OCTAL_NUMBER <- OCTAL_DIGIT OCTAL_DIGIT? OCTAL_DIGIT?
~ES_HEX_NUMBER <- HEX_DIGIT HEX_DIGIT?
~ES_UNICODE_CHAR <- HEX_DIGIT HEX_DIGIT? HEX_DIGIT? HEX_DIGIT?

# Auxiliar tokens
~END_KW <- !IDENTIFIER
# KEYWORDS
~KW_CONST <- WS? < "const" > END_KW WS?
~KW_MODULE <- WS? < "module" > END_KW WS?
~KW_SEQUENCE <- WS? < "sequence" > END_KW WS?
~KW_STRING <- WS? < "string" > END_KW WS?
~KW_WSTRING <- WS? < "wstring" > END_KW WS?
~KW_FIXED <- WS? < "fixed" > END_KW WS?
~KW_UNION <- WS? < "union" > END_KW WS?
~KW_STRUCT <- WS? < "struct" > END_KW WS?
~KW_ENUM <- WS? < "enum" > END_KW WS?
~KW_SWITCH <- WS? < "switch" > END_KW WS?
~KW_CASE <- WS? < "case" > END_KW WS?
~KW_DEFAULT <- WS? < "default" > END_KW WS?
~KW_NATIVE <- WS? < "native" > END_KW WS?
~KW_TYPEDEF <- WS? < "typedef" > END_KW WS?
~KW_ANY <- WS? < "any" > END_KW WS?
~KW_MAP <- WS? < "map" > END_KW WS?
~KW_BITSET <- WS? < "bitset" > END_KW WS?
~KW_BITFIELD <- WS? < "bitfield" > END_KW WS?
~KW_BITMASK <- WS? < "bitmask" > END_KW WS?
~KW_ANNOTATION <- WS? < "@annotation" > END_KW WS?
~KW_SHORT <-  WS? < "short" > END_KW WS?
~KW_INT16 <-  WS? < "int16" > END_KW WS?
~KW_LONG <-  WS? < "long" > END_KW WS?
~KW_LONG_LONG <-  WS? < "long" WS "long" > END_KW WS?
~KW_INT32 <-  WS? < "int32" > END_KW WS?
~KW_INT64 <-  WS? < "int64" > END_KW WS?
~KW_UNSIGNED_LONG <-  WS? < "unsigned" WS "long" > END_KW WS?
~KW_UNSIGNED_LONG_LONG <-  WS? < "unsigned" WS "long" WS "long" > END_KW WS?
~KW_UNSIGNED_SHORT <-  WS? < "unsigned" WS "short" > END_KW WS?
~KW_UINT16 <-  WS? < "uint16" > END_KW WS?
~KW_UINT32 <-  WS? < "uint32" > END_KW WS?
~KW_UINT64 <-  WS? < "uint64" > END_KW WS?
~KW_INT8 <-  WS? < "int8" > END_KW WS?
~KW_UINT8 <-  WS? < "uint8" > END_KW WS?
~KW_CHAR <-  WS? < "char" > END_KW WS?
~KW_WCHAR <-  WS? < "wchar" > END_KW WS?
~KW_BOOLEAN <-  WS? < "boolean" > END_KW WS?
~KW_OCTET <-  WS? < "octet" > END_KW WS?
~KW_FLOAT <-  WS? < "float" > END_KW WS?
~KW_DOUBLE <-  WS? < "double" > END_KW WS?
~KW_LONG_DOUBLE <- WS? < "long" WS "double" > END_KW WS?
~KW_LINE_COMMENT <- < "//" >
~KW_OPEN_BLOCK_COMMENT <- < "/*" >
~KW_CLOSE_BLOCK_COMMENT <- < "*/" >
# SYMBOLS
~WS <- COMMENT / [ \t\r\n]+
~SEMICOLON <- WS? < ';' > WS?
~COLON <- WS? < ':' > WS?
~DOUBLE_COLON <- "::"
~COMMA <- WS? < ',' > WS?
~DOT <- '.'
~OPEN_PARENTHESES <- WS? < '(' > WS?
~CLOSE_PARENTHESES <- WS? < ')' > WS?
~OPEN_BRACKET <- WS? < '[' > WS?
~CLOSE_BRACKET <- WS? < ']' > WS?
~OPEN_ANG_BRACKET <- WS? < '<' > WS?
~CLOSE_ANG_BRACKET <- WS? < '>' > WS?
~OPEN_BRACE <- WS? < '{' > WS?
~CLOSE_BRACE <- WS? < '}' > WS?
# OPERATORS
EQUAL_OP <- WS? < '=' > WS?
OR_OP <- WS? < '|' > WS?
XOR_OP <- WS? < '^' > WS?
AND_OP <- WS? < '&' > WS?
LSHIFT_OP <- WS? < "<<" > WS?
RSHIFT_OP <- WS? < ">>" > WS?
ADD_OP <- WS? < '+' > WS?
SUB_OP <- WS? < '-' > WS?
MULT_OP <- WS? < '*' > WS?
DIV_OP <- WS? < '/' > WS?
MOD_OP <- WS? < '%' > WS?
NEG_OP <- WS? < '~' > WS?
UNARY_OP <- SUB_OP / ADD_OP / NEG_OP
# DIGITS
DECIMAL_DIGIT <- [0-9]
OCTAL_DIGIT <- [0-7]
HEX_DIGIT <- [0-9a-fA-F]
)";
    return IDL_GRAMMAR;
}

}
}
}

#endif //EPROSIMA_XTYPES_IDL_GRAMMAR_HPP_

#!/bin/bash

# ==================== CONFIGURATION ====================
VERSION="1.0.1"

# ==================== COLORS ====================
ESC='\033'
CYAN="${ESC}[36m"
RED="${ESC}[31m"
RESET="${ESC}[0m"
BOLD="${ESC}[1m"
TAB="\t"

# ==================== FUNCTIONS ====================

print_header() {
    echo
    echo -e "${CYAN}"
    repeat_char "=" 37
    echo -e "${TAB}VECTOR TYPE GENERATOR"
    repeat_char "=" 37
    echo -e "${RESET}"
    echo
}

print_footer() {
    local elapsed=$1
    echo
    echo -e "${CYAN}"
    repeat_char "=" 37
    echo -e "${TAB}Done!"
    echo -e "${TAB}Spent time: ${elapsed}s"
    repeat_char "=" 37
    echo -e "${RESET}"
    echo
}

print_error() {
    echo -e "${RED}[ERROR] $1${RESET}" >&2
    exit 1
}

repeat_char() {
    local char=$1
    local count=$2
    printf "%${count}s" | tr " " "$char"
}

strlen() {
    echo ${#1}
}

generate_alias() {
    local input="$1"
    local original="$input"
    local output=""
    local ptr_count=0
    local type_part=""
    local temp="$input"
    
    # ===== 1. Separate type name from pointers =====
    # Count leading pointers
    while [[ "$temp" =~ ^\* ]]; do
        ((ptr_count++))
        temp="${temp:1}"
        temp="${temp## }"
    done
    
    type_part="$temp"
    
    # Count trailing pointers
    while [[ "$type_part" =~ \*$ ]]; do
        ((ptr_count++))
        type_part="${type_part%\*}"
        type_part="${type_part%% }"
    done
    
    # ===== 2. Parse type qualifiers =====
    local has_unsigned=0 has_signed=0 has_short=0 has_long=0 has_longlong=0 has_struct=0
    local working="$type_part"
    local remaining="$working"
    
    # Check for struct
    if [[ "$remaining" == *"struct"* ]]; then
        has_struct=1
        remaining="${remaining/struct/}"
    fi
    
    # Check for long long
    if [[ "$remaining" == *"long long"* ]]; then
        has_longlong=1
        remaining="${remaining/long long/}"
    # Check for long
    elif [[ "$remaining" == *"long"* ]]; then
        has_long=1
        remaining="${remaining/long/}"
    fi
    
    # Check for short
    if [[ "$remaining" == *"short"* ]]; then
        has_short=1
        remaining="${remaining/short/}"
    fi
    
    # Check for unsigned
    if [[ "$remaining" == *"unsigned"* ]]; then
        has_unsigned=1
        remaining="${remaining/unsigned/}"
    fi
    
    # Check for signed
    if [[ "$remaining" == *"signed"* ]]; then
        has_signed=1
        remaining="${remaining/signed/}"
    fi
    
    # Remove all spaces
    remaining="$(echo "$remaining" | tr -d ' ')"
    
    # ===== 3. Determine type name =====
    local type_name=""
    if [[ -n "$remaining" ]]; then
        type_name="$remaining"
    else
        if [[ $has_longlong -eq 1 ]] || [[ $has_long -eq 1 ]]; then
            type_name=""
        elif [[ $has_short -eq 1 ]]; then
            type_name="short"
        elif [[ $has_unsigned -eq 1 ]] || [[ $has_signed -eq 1 ]]; then
            type_name="int"
        elif [[ $has_struct -eq 1 ]]; then
            type_name="struct"
        fi
    fi
    
    # ===== 4. Build base name =====
    local basename=""
    [[ $has_unsigned -eq 1 ]] && basename="u${basename}"
    [[ $has_signed -eq 1 ]] && basename="s${basename}"
    [[ $has_short -eq 1 ]] && basename="short_${basename}"
    [[ $has_long -eq 1 ]] && basename="l${basename}"
    [[ $has_longlong -eq 1 ]] && basename="ll${basename}"
    
    if [[ -n "$type_name" ]]; then
        basename="${basename}${type_name}"
    fi
    
    # ===== 5. Add _ptr for pointers =====
    output="$basename"
    if [[ $ptr_count -gt 0 ]]; then
        for ((i=1; i<=ptr_count; i++)); do
            output="${output}_ptr"
        done
    fi
    
    # Remove double underscores
    output="${output//__/_}"
    
    echo "$output"
}

# ==================== MAIN SCRIPT ====================

SRC_OVERRIDE=""
while [[ $# -gt 0 ]]; do
    case $1 in
        --src-dir)
            SRC_OVERRIDE="$2"
            shift 2
            ;;
        --help)
            echo "Usage: $0 [--src-dir PATH]"
            echo "  --src-dir PATH  Specify source directory (default: ../src)"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            ;;
    esac
done

START_TIME=$(date +%s)

print_header

# ==================== DIRECTORIES ====================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

if [[ -n "$SRC_OVERRIDE" ]]; then
    SRC_DIR="$SRC_OVERRIDE"
    PROJECT_ROOT="$(cd "$SRC_DIR/.." && pwd)"
else
    SRC_DIR="$PROJECT_ROOT/src"
fi

LOG_DIR="$SRC_DIR/log"
GEN_DIR="$SRC_DIR/gen"
DOC_DIR="$PROJECT_ROOT/doc"

echo -e "${BOLD}Creating directories...${RESET}"
for dir in "$LOG_DIR" "$GEN_DIR" "$DOC_DIR"; do
    if [[ ! -d "$dir" ]]; then
        mkdir -p "$dir"
        if [[ -d "$dir" ]]; then
            echo -e "${TAB}${GREEN}Created: $dir${RESET}"
        else
            echo -e "${TAB}${RED}Failed to create: $dir${RESET}"
        fi
    else
        echo -e "${TAB}${CYAN}Exists: $dir${RESET}"
    fi
done
echo

# ==================== FILES ====================
H_FILE="$SRC_DIR/vector.h"
DECL_H_FILE="$GEN_DIR/vector_decl.h"
IMPL_C_FILE="$GEN_DIR/vector_impl.c"
ALIASES_H_FILE="$GEN_DIR/vector_aliases.h"
TYPES_TXT_FILE="$DOC_DIR/vector_types.txt"
DEBUG_TXT_FILE="$LOG_DIR/vector_gen.txt"

# Temporary files
TYPES_TEMP_FILE=$(mktemp)
PAIRS_TEMP_FILE=$(mktemp)

# ==================== VALIDATION ====================
if [[ ! -f "$H_FILE" ]]; then
    print_error "$H_FILE not found!"
fi

# ==================== FILE LIST ====================
echo -e "${BOLD}Files:${RESET}"
echo -e "${TAB}Input:  $H_FILE"
echo -e "${TAB}Output: $DECL_H_FILE"
echo -e "${TAB}        $IMPL_C_FILE"
echo -e "${TAB}        $ALIASES_H_FILE"
echo -e "${TAB}        $TYPES_TXT_FILE"
echo -e "${TAB}        $DEBUG_TXT_FILE"
echo

# ==================== STAGE 1: COLLECT TYPES ====================
echo -e "${BOLD}[1/4] Collecting types from $H_FILE...${RESET}"

COUNT=0
MAX_RAW_LEN=0

# Read vector.h line by line
while IFS= read -r line; do
    # Find VECTOR_INITIALIZE_* macros
    if [[ "$line" =~ VECTOR_INITIALIZE_[A-Z_]*\(([^)]*)\) ]]; then
        raw_type="${BASH_REMATCH[1]}"
        raw_type="$(echo "$raw_type" | xargs)"  # Trim
        
        # Check for duplicates
        if ! grep -q "^$raw_type$" "$TYPES_TEMP_FILE" 2>/dev/null; then
            echo "$raw_type" >> "$TYPES_TEMP_FILE"
            ((COUNT++))
            echo -e "${TAB}[$COUNT] Found type: $raw_type"
            
            # Update max length
            len=${#raw_type}
            [[ $len -gt $MAX_RAW_LEN ]] && MAX_RAW_LEN=$len
        fi
    fi
done < "$H_FILE"

if [[ $COUNT -eq 0 ]]; then
    print_error "No types found!"
fi

echo
echo -e "${TAB}Found unique types: $COUNT"
echo

# ==================== STAGE 2: GENERATE ALIASES ====================
echo -e "${BOLD}[2/4] Generating aliases...${RESET}"

PAIR_COUNT=0
MAX_ALIAS_LEN=0

# First pass - find maximum lengths
while IFS= read -r raw; do
    [[ -z "$raw" ]] && continue
    alias=$(generate_alias "$raw")
    alias_with_t="${alias}_t"
    
    len=${#raw}
    [[ $len -gt $MAX_RAW_LEN ]] && MAX_RAW_LEN=$len
    
    len=${#alias_with_t}
    [[ $len -gt $MAX_ALIAS_LEN ]] && MAX_ALIAS_LEN=$len
done < "$TYPES_TEMP_FILE"

# Second pass - display with alignment
while IFS= read -r raw; do
    [[ -z "$raw" ]] && continue
    alias=$(generate_alias "$raw")
    alias_with_t="${alias}_t"
    
    echo "$raw | $alias_with_t" >> "$PAIRS_TEMP_FILE"
    ((PAIR_COUNT++))
    
    # Pad raw string
    printf -v raw_padded "%-*s" $MAX_RAW_LEN "$raw"
    echo -e "${TAB}[$PAIR_COUNT/$COUNT] $raw_padded --> $alias_with_t"
done < "$TYPES_TEMP_FILE"

echo
echo -e "${TAB}Generated aliases: $PAIR_COUNT"
echo

# ==================== STAGE 3: GENERATE FILES ====================
echo -e "${BOLD}[3/4] Generating files...${RESET}"

# 3.1 Generate vector_aliases.h
echo -e "${TAB}[1/4] vector_aliases.h"

{
    cat << EOF
/**
 * @file        vector_aliases.h
 * @brief       Intermediate typedefs for vector types
 * @author      Generated by vector_gen.sh
 *
 * @date        $(date '+%Y-%m-%d %H:%M:%S')
 * @version     $VERSION
 *
 * @details     This file provides typedefs that map original C types
 *              to simplified type names used by the vector library.
 *              These typedefs are used as T parameters when including
 *              vector_template.h and vector_template.c.
 *
 * @note        This file is automatically generated.
 * @warning     DO NOT EDIT THIS FILE MANUALLY.
 *              Any manual changes will be lost on regeneration.
 *
 * @see         vector_template.h
 * @see         vector_decl.h
 * @see         vector_impl.c
 */

#ifndef __VECTOR_ALIASES_H__
#define __VECTOR_ALIASES_H__

EOF

    while IFS='|' read -r original alias; do
        echo "/* Type: $original */"
        echo "typedef $original $alias;"
        echo
    done < "$PAIRS_TEMP_FILE"

    echo "#endif // __VECTOR_ALIASES_H__"
} > "$ALIASES_H_FILE"

# 3.2 Generate vector_decl.h
echo -e "${TAB}[2/4] vector_decl.h"

{
    cat << EOF
/**
 * @file        vector_decl.h
 * @brief       Vector declaration
 *              This file contains the declarations of all vector operations
 *              for each type defined in the system.
 * @author      Generated by vector_gen.sh
 *
 * @date        $(date '+%Y-%m-%d %H:%M:%S')
 * @version     $VERSION
 *
 * @details     Declaration for $PAIR_COUNT vector types:
 *              - Memory management (create/destroy)
 *              - Element access (at, front, back)
 *              - Modifiers (push, pop, insert, erase)
 *              - Capacity operations (reserve, resize)
 *              - Iterators (begin, end)
 *
 * @note        This file is automatically generated
 * @warning     DO NOT EDIT THIS FILE MANUALLY.
 *              Any manual changes will be lost on regeneration.
 *
 * @see         vector_template.h
 * @see         vector_aliases.h
 */

#ifndef __VECTOR_DECL_H__
#define __VECTOR_DECL_H__

#include "vector_aliases.h"

#ifdef T
#define _OLD_T T
#undef T
#endif

EOF

    while IFS='|' read -r original alias; do
        echo "/* Type: $original -> $alias */"
        echo "#define T $alias"
        echo "#include \"vector_template.h\""
        echo "#undef T"
        echo
    done < "$PAIRS_TEMP_FILE"

    cat << EOF
#ifdef _OLD_T
#define T _OLD_T
#undef _OLD_T
#endif

#endif // __VECTOR_DECL_H__
EOF
} > "$DECL_H_FILE"

# 3.3 Generate vector_impl.c
echo -e "${TAB}[3/4] vector_impl.c"

{
    cat << EOF
/**
 * @file        vector_impl.c
 * @brief       Vector implementations
 *              This file contains the implementations of all vector operations
 *              for each type defined in the system.
 * @author      Generated by vector_gen.sh
 *
 * @date        $(date '+%Y-%m-%d %H:%M:%S')
 * @version     $VERSION
 *
 * @details     Implementations for $PAIR_COUNT vector types:
 *              - Memory management (create/destroy)
 *              - Element access (at, front, back)
 *              - Modifiers (push, pop, insert, erase)
 *              - Capacity operations (reserve, resize)
 *              - Iterators (begin, end)
 *
 * @note        This file is automatically generated
 * @warning     DO NOT EDIT THIS FILE MANUALLY.
 *              Any manual changes will be lost on regeneration.
 *
 * @see         vector_template.c
 * @see         vector_decl.h
 */

#include "vector_decl.h"

#ifdef T
#define _OLD_T T
#undef T
#endif

EOF

    while IFS='|' read -r original alias; do
        echo "/* Type: $original -> $alias */"
        echo "#define T $alias"
        echo "#include \"vector_template.c\""
        echo "#undef T"
        echo
    done < "$PAIRS_TEMP_FILE"

    echo "#ifdef _OLD_T"
    echo "#define T _OLD_T"
    echo "#undef _OLD_T"
    echo "#endif"
} > "$IMPL_C_FILE"

# 3.4 Generate documentation
echo -e "${TAB}[4/4] vector_types.txt"

{
    cat << EOF

$(repeat_char "=" 20)
\tVECTOR TYPES
$(repeat_char "=" 20)

Total types: $PAIR_COUNT

EOF

    # Calculate column widths
    MAX_ORIGINAL=0
    MAX_ALIAS=0
    while IFS='|' read -r original alias; do
        len=${#original}
        [[ $len -gt $MAX_ORIGINAL ]] && MAX_ORIGINAL=$len
        len=${#alias}
        [[ $len -gt $MAX_ALIAS ]] && MAX_ALIAS=$len
    done < "$PAIRS_TEMP_FILE"

    # Header
    printf "%-*s | %s\n" $((MAX_ORIGINAL + 2)) "Original type" "Alias"
    # Separator
    printf "%-*s-+-%s\n" $((MAX_ORIGINAL + 2)) "$(printf '%0.1s' "-"{1..$((MAX_ORIGINAL + 2))})" "$(printf '%0.1s' "-"{1..MAX_ALIAS})"
    
    # Data
    while IFS='|' read -r original alias; do
        printf "%-*s | %s\n" $((MAX_ORIGINAL + 2)) "$original" "$alias"
    done < "$PAIRS_TEMP_FILE"
} > "$TYPES_TXT_FILE"

# 3.5 Generate debug info
{
    echo "Vector Generator Debug Log"
    echo "Generated: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "Types found: $PAIR_COUNT"
    echo
    
    while IFS='|' read -r original alias; do
        echo "Original: $original"
        echo "  Alias: $alias"
        echo
    done < "$PAIRS_TEMP_FILE"
} > "$DEBUG_TXT_FILE"

# ==================== STAGE 4: CLEANUP ====================
echo -e "${BOLD}[4/4] Cleanup...${RESET}"

# Remove temporary files
rm -f "$TYPES_TEMP_FILE" "$PAIRS_TEMP_FILE"
echo -e "${TAB}${GREEN}Removed temporary files${RESET}"

# ==================== FINISH ====================
END_TIME=$(date +%s)
ELAPSED=$((END_TIME - START_TIME))

print_footer "$ELAPSED"

exit 0
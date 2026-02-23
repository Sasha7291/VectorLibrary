#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Vector Type Generator
Generates vector type declarations, implementations, and aliases
from vector.h macros.
"""

import os
import sys
import platform
import re
import time
from datetime import datetime
from pathlib import Path
import argparse
import shutil
from typing import List, Tuple, Set, Optional

# ==================== CONFIGURATION ====================
if platform.system() == "Windows":
    import ctypes
    kernel32 = ctypes.windll.kernel32
    kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)

VERSION = "1.0.1"

# ==================== TYPE PARSER ====================
class CTypeParser:
    """Parses C types and generates aliases"""
    
    def parse_type(self, type_str: str) -> dict:
        """
        Parse C type string and return components:
        - base_type: cleaned type name
        - pointers: number of pointer levels
        - qualifiers: dict of qualifiers (unsigned, signed, short, long, long long, struct)
        - alias: generated alias name
        """
        result = {
            'original': type_str.strip(),
            'pointers': 0,
            'qualifiers': {
                'unsigned': False,
                'signed': False,
                'short': False,
                'long': False,
                'long_long': False,
                'struct': False
            },
            'type_name': '',
            'alias': ''
        }
        
        working = result['original']
        
        while working.startswith('*'):
            result['pointers'] += 1
            working = working[1:].lstrip()
        
        if working.startswith('struct'):
            result['qualifiers']['struct'] = True
            working = working[6:].lstrip()
        
        if working.startswith('long long'):
            result['qualifiers']['long_long'] = True
            working = working[9:].lstrip()
        elif working.startswith('long'):
            result['qualifiers']['long'] = True
            working = working[4:].lstrip()
        
        if working.startswith('short'):
            result['qualifiers']['short'] = True
            working = working[5:].lstrip()
        
        if working.startswith('unsigned'):
            result['qualifiers']['unsigned'] = True
            working = working[8:].lstrip()
        
        if working.startswith('signed'):
            result['qualifiers']['signed'] = True
            working = working[6:].lstrip()
        
        working = working.rstrip()
        while working.endswith('*'):
            result['pointers'] += 1
            working = working[:-1].rstrip()
        
        result['type_name'] = working.strip()
        result['alias'] = self._generate_alias(result)
        
        return result
    
    def _generate_alias(self, parsed: dict) -> str:
        """Generate alias from parsed components"""
        alias_parts = []
        
        if parsed['qualifiers']['unsigned']:
            alias_parts.append('u')
        elif parsed['qualifiers']['signed']:
            alias_parts.append('s')
        if parsed['qualifiers']['short']:
            alias_parts.append('short_')
        elif parsed['qualifiers']['long']:
            alias_parts.append('l')
        elif parsed['qualifiers']['long_long']:
            alias_parts.append('ll')
        
        type_name = parsed['type_name']
        if type_name:
            type_name = type_name.replace('*', '').strip()
            if type_name:
                alias_parts.append(type_name)
        else:
            if parsed['qualifiers']['short']:
                alias_parts.append('short')
            elif parsed['qualifiers']['unsigned'] or parsed['qualifiers']['signed']:
                alias_parts.append('int')
            elif parsed['qualifiers']['struct']:
                alias_parts.append('struct')
        
        alias = ''.join(alias_parts)
        
        for _ in range(parsed['pointers']):
            alias += '_ptr'
        
        alias = alias.replace('__', '_')
        if alias.endswith('_'):
            alias = alias[:-1]
        
        return alias

# ==================== GENERATOR ====================
ESC = '\x1b'

class Colors:
    """ANSI color codes for terminal output"""
    CYAN = f'{ESC}[96m'
    RED = f'{ESC}[91m'
    END = f'{ESC}[0m'
    BOLD = f'{ESC}[1m'
        
class VectorGenerator:
    """Main generator class"""

    def __init__(self, src_dir: str = None):
        """Initialize generator"""
        self.parser = CTypeParser()
        self.types: List[Tuple[str, str]] = []
        self.start_time = time.time()
        
        self.script_dir = Path(__file__).parent.resolve()
        self.project_root = self.script_dir.parent.resolve()
        self.src_dir = (self.project_root / 'src').resolve()
        
        if src_dir:
            src_path = Path(src_dir)
            if src_path.is_absolute():
                self.src_dir = src_path
            else:
                self.src_dir = (self.project_root / src_path).resolve()
            
            if self.src_dir.name == 'src':
                self.project_root = self.src_dir.parent.resolve()
            else:
                self.project_root = self.src_dir.parent.parent.resolve()
        
        self.header_file = (self.src_dir / 'vector.h').resolve()
        self.gen_dir = (self.src_dir / 'gen').resolve()
        self.log_dir = (self.src_dir / 'log').resolve()
        self.doc_dir = (self.project_root / 'doc').resolve()
        
        self.gen_dir.mkdir(parents=True, exist_ok=True)
        self.log_dir.mkdir(parents=True, exist_ok=True)
        self.doc_dir.mkdir(parents=True, exist_ok=True)

    def print_header(self):
        """Print pretty header"""
        print(f"{Colors.CYAN}{'='*37}")
        print(f"\tVECTOR TYPE GENERATOR")
        print(f"{'='*37}{Colors.END}\n")
    
    def print_footer(self):
        """Print pretty footer"""
        elapsed = time.time() - self.start_time
        print()
        print(f"{Colors.CYAN}{'='*37}")
        print(f"\tDone!")
        print(f"\tSpent time: {elapsed:.2f}s")
        print(f"{'='*37}{Colors.END}\n")
    
    def extract_types(self) -> bool:
        """Extract types from vector.h using regex"""
        print(f"{Colors.BOLD}[1/3] Collecting types from {self.header_file}...{Colors.END}")
        
        if not self.header_file.exists():
            print(f"{Colors.RED}[ERROR] {self.header_file} not found!{Colors.END}")
            return False
        
        content = self.header_file.read_text(encoding='utf-8')
        
        pattern = r'VECTOR_INITIALIZE_[A-Z_]*\s*\(\s*([^)]+)\s*\)'
        matches = re.findall(pattern, content)
        
        seen = set()
        self.types.clear()
        
        for match in matches:
            original = match.strip()
            if original and original not in seen:
                seen.add(original)
                parsed = self.parser.parse_type(original)
                alias = parsed['alias'] + '_t'
                self.types.append((original, alias))
                
                print(f"\t[{len(self.types)}] Found type: {original}")
        
        if not self.types:
            print(f"{Colors.RED}[ERROR] No types found!{Colors.END}")
            return False
        
        print(f"\n\tFound unique types: {len(self.types)}\n")
        return True
    
    def display_aliases(self):
        """Display generated aliases in formatted table"""
        print(f"{Colors.BOLD}[2/3] Generating aliases...{Colors.END}")
        
        max_original = max(len(t[0]) for t in self.types)
        max_alias = max(len(t[1]) for t in self.types)
        
        for i, (original, alias) in enumerate(self.types, 1):
            padded_original = original.ljust(max_original)
            print(f"\t[{i}/{len(self.types)}] {padded_original} --> {alias}")
        
        print(f"\n\tGenerated aliases: {len(self.types)}\n")
    
    def generate_aliases_h(self, output_file: Path):
        """Generate vector_aliases.h"""
        print("\t[1/4] vector_aliases.h")
        
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(f"""/**
 * @file        vector_aliases.h
 * @brief       Intermediate typedefs for vector types
 * @author      Generated by vector_gen.py
 *
 * @date        {now}
 * @version     {VERSION}
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

""")
            
                for original, alias in self.types:
                    f.write(f"/* Type: {original} */\n")
                    f.write(f"typedef {original} {alias};\n\n")
                
                f.write("#endif // __VECTOR_ALIASES_H__\n")
            
        except Exception as e:
            print(f"\t\tError writing {output_file.name}: {e}")
            raise
    
    def generate_decl_h(self, output_file: Path):
        """Generate vector_decl.h"""
        print("\t[2/4] vector_decl.h")
        
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(f"""/**
 * @file        vector_decl.h
 * @brief       Vector declaration
 *              This file contains the declarations of all vector operations
 *              for each type defined in the system.
 * @author      Generated by vector_gen.py
 *
 * @date        {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}
 * @version     {VERSION}
 *
 * @details     Declaration for {len(self.types)} vector types:
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

""")
            
                for original, alias in self.types:
                    f.write(f"/* Type: {original} -> {alias} */\n")
                    f.write(f"#define T {alias}\n")
                    f.write(f'#include "vector_template.h"\n')
                    f.write(f"#undef T\n\n")
                
                f.write("""#ifdef _OLD_T
#define T _OLD_T
#undef _OLD_T
#endif

#endif // __VECTOR_DECL_H__
""")

        except Exception as e:
            print(f"\t\tError writing {output_file.name}: {e}")
            raise
    
    def generate_impl_c(self, output_file: Path):
        """Generate vector_impl.c"""
        print("\t[3/4] vector_impl.c")
        
        try: 
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(f"""/**
 * @file        vector_impl.c
 * @brief       Vector implementations
 *              This file contains the implementations of all vector operations
 *              for each type defined in the system.
 * @author      Generated by vector_gen.py
 *
 * @date        {datetime.now().strftime("%Y-%m-%d %H:%M:%S")}
 * @version     {VERSION}
 *
 * @details     Implementations for {len(self.types)} vector types:
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

""")
            
                for original, alias in self.types:
                    f.write(f"/* Type: {original} -> {alias} */\n")
                    f.write(f"#define T {alias}\n")
                    f.write(f'#include "vector_template.c"\n')
                    f.write(f"#undef T\n\n")
                
                f.write("""#ifdef _OLD_T
#define T _OLD_T
#undef _OLD_T
#endif
""")

        except Exception as e:
            print(f"\t\tError writing {output_file.name}: {e}")
            raise
    
    def generate_documentation(self, output_file: Path):
        """Generate vector_types.txt documentation"""
        print("\t[4/4] vector_types.txt")
        
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(f"""
{'='*20}
\tVECTOR TYPES
{'='*20}

Total types: {len(self.types)}

""")
            
                max_original = max(len(t[0]) for t in self.types)
                max_alias = max(len(t[1]) for t in self.types)
                
                header_original = "Original type".ljust(max_original + 2)
                header_alias = "Alias".ljust(max_alias)
                f.write(f"{header_original} | {header_alias}\n")
                
                f.write(f"{'-' * (max_original + 2)}-+-{'-' * max_alias}\n")
                
                for original, alias in self.types:
                    f.write(f"{original.ljust(max_original + 2)} | {alias}\n")
            
        except Exception as e:
            print(f"\t\tError writing {output_file.name}: {e}")
            raise
    
    def generate_debug(self, output_file: Path):
        """Generate debug information"""
        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                f.write(f"Vector Generator Debug Log\n")
                f.write(f"Generated: {datetime.now()}\n")
                f.write(f"Types found: {len(self.types)}\n\n")
                
                for original, alias in self.types:
                    parsed = self.parser.parse_type(original)
                    f.write(f"Original: {original}\n")
                    f.write(f"  Alias: {alias}\n")
                    f.write(f"  Pointers: {parsed['pointers']}\n")
                    f.write(f"  Qualifiers: {parsed['qualifiers']}\n")
                    f.write(f"  Type name: {parsed['type_name']}\n\n")

        except Exception as e:
            print(f"\t\tError writing {output_file.name}: {e}")
            raise
    
    def run(self):
        """Main generation routine"""
        self.print_header()
        
        print(f"Input files:")
        print(f"\tTypes initialization :: {self.header_file}\n")
        
        print(f"Output files:")
        print(f"\tDeclaration          :: {self.gen_dir / 'vector_decl.h'}")
        print(f"\tImplementation       :: {self.gen_dir / 'vector_impl.c'}")
        print(f"\tAliases              :: {self.gen_dir / 'vector_aliases.h'}")
        print(f"\tDocumentation        :: {self.doc_dir / 'vector_types.txt'}")
        print(f"\tDebug                :: {self.log_dir / 'vector_gen.txt'}\n")
        
        if not self.extract_types():
            return False
        
        self.display_aliases()
        
        print("{Colors.BOLD}[3/3] Generating files...{Colors.END}")
        
        self.generate_aliases_h(self.gen_dir / 'vector_aliases.h')
        self.generate_decl_h(self.gen_dir / 'vector_decl.h')
        self.generate_impl_c(self.gen_dir / 'vector_impl.c')
        self.generate_documentation(self.doc_dir / 'vector_types.txt')
        self.generate_debug(self.log_dir / 'vector_gen.txt')
        
        self.print_footer()
        
        return True

# ==================== MAIN ====================
def main():
    parser = argparse.ArgumentParser(description='Vector Type Generator')
    parser.add_argument('--src-dir', type=str, default='src',
                       help='Source directory (default: src)')
    
    args = parser.parse_args()
    src_dir = Path(args.src_dir)
    generator = VectorGenerator(src_dir)
    success = generator.run()
    
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
/* Copyright (c) 2021 i509VCB
   This file is part of the c-tiny-parser.

   c-tiny-parser is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   c-tiny-parser is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the c-tiny-parser; if not, see
   <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tiny.h"

/* Just a nice marker for api methods I am implementing */
#define __api_impl

/**
 * Counts the amount of *tab* indents in a string before the occurance of any other character.
 *
 * @param input the input
 * @return the amount of tab indents before any other character is reached
 */
unsigned int count_indents(char** input) {
    int indents = 0;
    size_t pos;

    for (pos = 0; input[pos]; pos++) {
        if (*input[pos] != '\t') {
            return indents;
        }

        indents++;
    }

    return indents;
}

__api_impl
struct TinyCursor {
    uint32_t major_version;
    uint32_t minor_version;
    char* content;
    size_t cursor;
    bool eof;
    char* last_line;
    char** namespaces;
};

/**
 * Reads a line in the file, moving the cursor.
 *
 * @param cursor the tiny cursor
 * @return the string of the line or NULL if EOF was reached
 */
char* read_line(TinyCursor* cursor) {
    size_t init_cursor = cursor->cursor;
    size_t current_cursor = init_cursor;

    while (true) {
        if (cursor->eof) {
            return NULL;
        }

        char c = cursor->content[current_cursor];

        if (!c) {
            cursor->eof = true;
            break;
        }

        if (c == '\n') {
            break;
        }

        current_cursor++;
    }

    size_t size = current_cursor - init_cursor;

    char* line = calloc(1, size + 1);

    if (!line) {
        printf("Failed to allocate memory to read line!\n");
        return NULL;
    }

    for (size_t idx = 0; idx < size; ++idx) {
        line[idx] = cursor->content[init_cursor + idx];
    }

    cursor->cursor = current_cursor + 1; // +1 to skip the \n to the next line

    return line;
}

char* read_to_tab(const char* input, size_t offset) {
    size_t init_offset = offset;

    while (true) {
        char c = input[offset];

        if (!c) {
            break;
        }

        if (c == '\t') {
            break;
        }

        offset++;
    }

    char* found = calloc(1, offset + 1);

    for (size_t idx = init_offset; idx < offset; ++idx) {
        found[idx - init_offset] = input[idx];
    }

    return found;
}

uint32_t char_to_uint(char c) {
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default: return -1;
    }
}

/**
 * Parse a tiny file header.
 *
 * @param cursor the cursor
 * @param line the header line
 * @return true if successful, false it there was any invalid syntax
 */
bool parse_header(TinyCursor* cursor, char* line) {
    size_t offset = 0;

    char* header_name = read_to_tab(line, 0);
    offset += strlen(header_name);

    bool atleast_v2 = false;

    int cmp = strcmp(header_name, "tiny");

    if (cmp == 0) {
        // At least tiny v2
        atleast_v2 = true;
        free(header_name);
    } else {
        // Is it v1?
        cmp = strcmp(header_name, "v1");

        if (cmp == 0) {
            // Definitely v1
            cursor->major_version = 1;
            cursor->minor_version = 0;
        } else {
            // No
            // TODO: Error setting - invalid syntax
            free(header_name);
            return false;
        }
    }

    // Complete v2 version
    if (atleast_v2) {
        offset += 1; // Skip past tab
        char* major_version = read_to_tab(line, offset);

        if (strlen(major_version) != 1) {
            free(major_version);
            return false; // TODO: Error setting - invalid syntax
        }

        cursor->major_version = char_to_uint(major_version[0]);
        offset += strlen(major_version);
        free(major_version);

        // Minor version
        offset += 1; // Skip past tab
        char* minor_version = read_to_tab(line, offset);

        if (strlen(minor_version) != 1) {
            free(minor_version);
            return false; // TODO: Error setting - invalid syntax
        }

        cursor->minor_version = char_to_uint(minor_version[0]);
        offset += strlen(minor_version);
        free(minor_version);
    }

    // Skip past tab
    offset += 1;

    // TODO: Read namespaces - same across v1 and v2

    uint32_t namespace_count = 0;
    char** namespaces = NULL;

    while (true) {
        char* namespace = read_to_tab(line, offset);

        offset += strlen(namespace);
        offset += 1; // Move past tab

        // TODO: Write namespace into namespaces array
        namespace_count++;

        // newline is end of namespaces
        if (!line[offset]) {
            break;
        }
    }

    offset += 1; // Move offset to next line
    cursor->cursor = offset;

    return true;
}

__api_impl
struct TinyCursor* create_cursor(char* content) {
    if (!content) {
        return NULL;
    }

    static TinyCursor cursor;
    cursor.content = content;
    cursor.cursor = 0;
    cursor.eof = false;

    // Read the header
    char* line = read_line(&cursor);

    if (!line) {
        // Failed to create cursor
        return NULL;
    }

    cursor.last_line = line;

    // Parse the header
    parse_header(&cursor, line);

    return &cursor;
}

__api_impl
uint32_t get_major_version(TinyCursor* cursor) {
    return cursor->major_version;
}

__api_impl
uint32_t get_minor_version(TinyCursor* cursor) {
    return cursor->minor_version;
}

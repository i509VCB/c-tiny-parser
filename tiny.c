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

#include "tiny.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Just a nice marker for api methods I am implementing */
#define __api_impl /* Nothing */

// TODO implement

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

enum ReadState {
    START,
    HEADER,
    CLASS,
    METHOD,
    METHOD_PARAMETER,
    FIELD
};

__api_impl
struct TinyCursor {
    enum ReadState read_state;
    unsigned int current_pos;
    unsigned int current_line;
    unsigned int major_version;
    unsigned int minor_version;
    char* content;
};

__api_impl
struct TinyCursor* create_cursor() {
    static struct TinyCursor cursor;
    cursor.read_state = START;

    return &cursor;
}

bool try_read_header(TinyCursor* cursor) {
    return false; // TODO
}

__api_impl
enum TinyResult begin_read(TinyCursor* cursor, char* input) {
    if (!cursor) {
        return NULL_CURSOR;
    }

    if (cursor->read_state != START) {
        return ALREADY_READING;
    }

    // Is the input empty?
    if (strlen(input) == 0) {
        return UNSUPPORTED_FILE;
    }

    cursor->read_state = START;
    cursor->current_pos = 0;
    cursor->current_line = 0;
    cursor->content = input;

    // See if we have a tiny file by looking at the header
    if (input[0] != 't') {
        return UNSUPPORTED_FILE;
    }

    // TODO Validate header format
    return OK;
}

__api_impl
unsigned int get_major_version(TinyCursor* cursor) {
    return cursor->major_version;
}

__api_impl
unsigned int get_minor_version(TinyCursor* cursor) {
    return cursor->minor_version;
}

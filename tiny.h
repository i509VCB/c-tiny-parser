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

#ifndef C_TINY_PARSER_TINY_H
#define C_TINY_PARSER_TINY_H

/**
 * Represents a cursor used to read a tiny file.
 */
typedef struct TinyCursor TinyCursor;

/**
 * Represents the result of an action related to reading a tiny file.
 */
enum TinyResult {
    /**
     * Represents a result where there were no issues while beginning to read a tiny file.
     */
    OK,
    /**
     * Represents an error where the cursor was a null pointer.
     */
    NULL_CURSOR,
    /**
     * Represents an error where the cursor is already reading a tiny file when the cursor is already reading a file.
     */
    ALREADY_READING,
    /**
     * Represents an error where the cursor is reading a file that is not a tiny file.
     */
    UNSUPPORTED_FILE,
    /**
     * Represents an error where the cursor is reading a malformed header in a tiny file.
     */
    INVALID_HEADER,
    /**
     * Represents an error where the cursor has encountered a version of the tiny file specification it cannot read.
     * The major and minor version of the tiny file may be read still if this error is thrown.
     */
    UNSUPPORTED_VERSION
};

/**
 * Represents a type of element which may be found in a tiny file.
 * The tiny elements in this enum are up to date version 2.0 tiny file.
 */
enum TinyElementType {
    /**
     * Represents a tiny element which this parser does not know how to parse.
     * The tiny specification requires that a parser ignore the element.
     *
     * You must call skip_element(TinyCursor) to move past this element.
     */
    UNSUPPORTED_ELEMENT,
    /**
     * Represents the definition of all namespaces the tiny file supports.
     * To read the namespaces, use read_namespaces.
     */
    NAMESPACES,
    /**
     * Represents a property definition in a tiny file.
     * To read the property definition, use read_property_def.
     */
    PROPERTY,
    /**
     * Represents a class definition in a tiny file.
     * To read the class definition, use read_class_def.
     */
    CLASS_DEF,
    /**
     * Represents a method definition in a tiny file.
     * To read the method definition, use read_method_def.
     */
    METHOD_DEF,
    /**
     * Represents a field definition in a tiny file.
     * To read the field definition, use read_field_def.
     */
    FIELD_DEF,
    /**
     * Represents a method parameter definition in a tiny file.
     * The method parameter belongs to the last method that was read by the cursor.
     * To read the method parameter, use read_method_parameter_def.
     */
    METHOD_PARAMETER_DEF,
    /**
     * Represents a local variable definition in a tiny file.
     * The local variable belongs to the last method that was read by the cursor.
     * To read the local variable, use read_local_variable_def.
     */
    LOCAL_VARIABLE_DEF,
    /**
     * Represents a comment in a tiny file.
     * A comment may belong to a class, method, field, method parameter and local variable.
     * The element this comment belongs to the last element that was read by the cursor.
     */
    COMMENT
};

/**
 * Creates a cursor used to read a tiny file
 * @return a new cursor
 */
struct TinyCursor* create_cursor();

/**
 * Starts reading a tiny file from the input.
 *
 * @param cursor a pointer representing the cursor used to track where the tiny file is being read from.
 * @param input the file to read from
 * @return a result detailing whether the file was successfully read or why the file could not be read.
 * If the cursor result is not OK then the
 */
enum TinyResult begin_read(TinyCursor* cursor, char* input);

/**
 * Peeks forward in the tiny file, reading the file without moving the cursor's internal position.
 *
 * @param cursor the cursor to read with
 * @return the type of element in the tiny file that was detected
 */
enum TinyElementType peek_forward(TinyCursor* cursor);

unsigned int get_major_version(TinyCursor* cursor);

unsigned int get_minor_version(TinyCursor* cursor);

#endif //C_TINY_PARSER_TINY_H

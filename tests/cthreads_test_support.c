/*
 * Library support functions test program
 *
 * Copyright (C) 2012-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_unused.h"

/* Tests the libcthreads_get_version function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libcthreads_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBCTHREADS_VERSION_STRING,
	          9 );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc CTHREADS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc CTHREADS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] CTHREADS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	CTHREADS_TEST_UNREFERENCED_PARAMETER( argc )
	CTHREADS_TEST_UNREFERENCED_PARAMETER( argv )

	CTHREADS_TEST_RUN(
	 "libcthreads_get_version",
	 cthreads_test_get_version );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


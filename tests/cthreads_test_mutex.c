/*
 * Library mutex type test program
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <errno.h>

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU
#endif

#include "cthreads_test_libcerror.h"
#include "cthreads_test_libcthreads.h"
#include "cthreads_test_macros.h"
#include "cthreads_test_memory.h"
#include "cthreads_test_unused.h"

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

static int (*cthreads_test_real_pthread_mutex_init)(pthread_mutex_t *, const pthread_mutexattr_t *) = NULL;
static int (*cthreads_test_real_pthread_mutex_destroy)(pthread_mutex_t *)                           = NULL;
static int (*cthreads_test_real_pthread_mutex_lock)(pthread_mutex_t *)                              = NULL;
static int (*cthreads_test_real_pthread_mutex_trylock)(pthread_mutex_t *)                           = NULL;
static int (*cthreads_test_real_pthread_mutex_unlock)(pthread_mutex_t *)                            = NULL;

int cthreads_test_pthread_mutex_init_attempts_before_fail                                           = -1;
int cthreads_test_pthread_mutex_destroy_attempts_before_fail                                        = -1;
int cthreads_test_pthread_mutex_lock_attempts_before_fail                                           = -1;
int cthreads_test_pthread_mutex_trylock_attempts_before_fail                                        = -1;
int cthreads_test_pthread_mutex_unlock_attempts_before_fail                                         = -1;

int cthreads_test_real_pthread_mutex_init_function_return_value                                     = EBUSY;
int cthreads_test_real_pthread_mutex_destroy_function_return_value                                  = EBUSY;
int cthreads_test_real_pthread_mutex_lock_function_return_value                                     = EBUSY;
int cthreads_test_real_pthread_mutex_trylock_function_return_value                                  = EBUSY;
int cthreads_test_real_pthread_mutex_unlock_function_return_value                                   = EBUSY;

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

libcthreads_mutex_t *cthreads_test_mutex = NULL;
int cthreads_test_mutexed_value          = 0;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

/* Custom pthread_mutex_init for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_init(
     pthread_mutex_t *mutex,
     const pthread_mutexattr_t *attr )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_init == NULL )
	{
		cthreads_test_real_pthread_mutex_init = dlsym(
		                                         RTLD_NEXT,
		                                         "pthread_mutex_init" );
	}
	if( cthreads_test_pthread_mutex_init_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_init_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_mutex_init_function_return_value );
	}
	else if( cthreads_test_pthread_mutex_init_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_init_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_init(
	          mutex,
	          attr );

	return( result );
}

/* Custom pthread_mutex_destroy for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_destroy(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_destroy == NULL )
	{
		cthreads_test_real_pthread_mutex_destroy = dlsym(
		                                            RTLD_NEXT,
		                                            "pthread_mutex_destroy" );
	}
	if( cthreads_test_pthread_mutex_destroy_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_destroy_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_mutex_destroy_function_return_value );
	}
	else if( cthreads_test_pthread_mutex_destroy_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_destroy_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_destroy(
	          mutex );

	return( result );
}

/* Custom pthread_mutex_lock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_lock(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_lock == NULL )
	{
		cthreads_test_real_pthread_mutex_lock = dlsym(
		                                         RTLD_NEXT,
		                                         "pthread_mutex_lock" );
	}
	if( cthreads_test_pthread_mutex_lock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_mutex_lock_function_return_value );
	}
	else if( cthreads_test_pthread_mutex_lock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_lock(
	          mutex );

	return( result );
}

/* Custom pthread_mutex_trylock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_trylock(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_trylock == NULL )
	{
		cthreads_test_real_pthread_mutex_trylock = dlsym(
		                                            RTLD_NEXT,
		                                            "pthread_mutex_trylock" );
	}
	if( cthreads_test_pthread_mutex_trylock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_mutex_trylock_function_return_value );
	}
	else if( cthreads_test_pthread_mutex_trylock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_trylock(
	          mutex );

	return( result );
}

/* Custom pthread_mutex_unlock for testing error cases
 * Returns 0 if successful or an error value otherwise
 */
int pthread_mutex_unlock(
     pthread_mutex_t *mutex )
{
	int result = 0;

	if( cthreads_test_real_pthread_mutex_unlock == NULL )
	{
		cthreads_test_real_pthread_mutex_unlock = dlsym(
		                                           RTLD_NEXT,
		                                           "pthread_mutex_unlock" );
	}
	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail == 0 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;

		return( cthreads_test_real_pthread_mutex_unlock_function_return_value );
	}
	else if( cthreads_test_pthread_mutex_unlock_attempts_before_fail > 0 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail--;
	}
	result = cthreads_test_real_pthread_mutex_unlock(
	          mutex );

	return( result );
}

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

/* The thread1 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_mutex_callback_function1(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_mutex_callback_function1";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab mutex.",
		 function );

		goto on_error;
	}
	cthreads_test_mutexed_value += 19;

	result = libcthreads_mutex_release(
		  cthreads_test_mutex,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release mutex.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	return( -1 );
}

/* The thread2 callback function
 * Returns 1 if successful or -1 on error
 */
int cthreads_test_mutex_callback_function2(
     void *arguments CTHREADS_TEST_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cthreads_test_mutex_callback_function2";
	int result               = 0;

	CTHREADS_TEST_UNREFERENCED_PARAMETER( arguments )

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab mutex.",
		 function );

		goto on_error;
	}
	cthreads_test_mutexed_value += 38;

	result = libcthreads_mutex_release(
		  cthreads_test_mutex,
		  &error );

	if( result != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release mutex.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	return( -1 );
}

/* Tests the libcthreads_mutex_initialize function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_mutex_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libcthreads_mutex_t *mutex      = NULL;
	int result                      = 0;

#if defined( HAVE_CTHREADS_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "mutex",
	 mutex );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "mutex",
	 mutex );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_mutex_initialize(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	mutex = (libcthreads_mutex_t *) 0x12345678UL;

	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	mutex = NULL;

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_CTHREADS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libcthreads_mutex_initialize with malloc failing
		 */
		cthreads_test_malloc_attempts_before_fail = test_number;

		result = libcthreads_mutex_initialize(
		          &mutex,
		          &error );

		if( cthreads_test_malloc_attempts_before_fail != -1 )
		{
			cthreads_test_malloc_attempts_before_fail = -1;

			if( mutex != NULL )
			{
				libcthreads_mutex_free(
				 &mutex,
				 NULL );
			}
		}
		else
		{
			CTHREADS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			CTHREADS_TEST_ASSERT_IS_NULL(
			 "mutex",
			 mutex );

			CTHREADS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libcthreads_mutex_initialize with memset failing
		 */
		cthreads_test_memset_attempts_before_fail = test_number;

		result = libcthreads_mutex_initialize(
		          &mutex,
		          &error );

		if( cthreads_test_memset_attempts_before_fail != -1 )
		{
			cthreads_test_memset_attempts_before_fail = -1;

			if( mutex != NULL )
			{
				libcthreads_mutex_free(
				 &mutex,
				 NULL );
			}
		}
		else
		{
			CTHREADS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			CTHREADS_TEST_ASSERT_IS_NULL(
			 "mutex",
			 mutex );

			CTHREADS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_CTHREADS_TEST_MEMORY ) */

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_mutex_initialize with pthread_mutex_init returning EAGAIN
	 */
	cthreads_test_pthread_mutex_init_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_init_function_return_value = EAGAIN;

	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	if( cthreads_test_pthread_mutex_init_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_init_attempts_before_fail = -1;

		if( mutex != NULL )
		{
			libcthreads_mutex_free(
			 &mutex,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "mutex",
		 mutex );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_initialize with pthread_mutex_init returning EBUSY
	 */
	cthreads_test_pthread_mutex_init_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_init_function_return_value = EBUSY;

	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	if( cthreads_test_pthread_mutex_init_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_init_attempts_before_fail = -1;

		if( mutex != NULL )
		{
			libcthreads_mutex_free(
			 &mutex,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "mutex",
		 mutex );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mutex != NULL )
	{
		libcthreads_mutex_free(
		 &mutex,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_mutex_free function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_mutex_free(
     void )
{
	libcerror_error_t *error   = NULL;
	int result                 = 0;

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
	libcthreads_mutex_t *mutex = NULL;
#endif

	/* Test error cases
	 */
	result = libcthreads_mutex_free(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_mutex_free with pthread_mutex_destroy returning EAGAIN
	 */
	cthreads_test_pthread_mutex_destroy_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_destroy_function_return_value = EAGAIN;

	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	if( cthreads_test_pthread_mutex_destroy_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_destroy_attempts_before_fail = -1;

		if( mutex != NULL )
		{
			libcthreads_mutex_free(
			 &mutex,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "mutex",
		 mutex );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_mutex_free with pthread_mutex_destroy returning EBUSY
	 */
	cthreads_test_pthread_mutex_destroy_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_destroy_function_return_value = EBUSY;

	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	if( cthreads_test_pthread_mutex_destroy_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_destroy_attempts_before_fail = -1;

		if( mutex != NULL )
		{
			libcthreads_mutex_free(
			 &mutex,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "mutex",
		 mutex );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libcthreads_mutex_free with pthread_mutex_destroy returning ENOMEM
	 */
	cthreads_test_pthread_mutex_destroy_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_destroy_function_return_value = ENOMEM;

	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	if( cthreads_test_pthread_mutex_destroy_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_destroy_attempts_before_fail = -1;

		if( mutex != NULL )
		{
			libcthreads_mutex_free(
			 &mutex,
			 NULL );
		}
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "mutex",
		 mutex );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )
	if( mutex != NULL )
	{
		libcthreads_mutex_free(
		 &mutex,
		 NULL );
	}
#endif
	return( 0 );
}

/* Tests the libcthreads_mutex_grab function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_mutex_grab(
     void )
{
	libcerror_error_t *error      = NULL;
	libcthreads_thread_t *thread1 = NULL;
	libcthreads_thread_t *thread2 = NULL;
	int result                    = 0;

	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &cthreads_test_mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libcthreads_thread_create(
	          &thread1,
	          NULL,
	          cthreads_test_mutex_callback_function1,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_create(
	          &thread2,
	          NULL,
	          cthreads_test_mutex_callback_function2,
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	cthreads_test_mutexed_value = 46;

	/* Test release
	 */
	result = libcthreads_mutex_release(
		  cthreads_test_mutex,
		  &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libcthreads_thread_join(
	          &thread2,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcthreads_thread_join(
	          &thread1,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "cthreads_test_mutexed_value",
	 cthreads_test_mutexed_value,
	 46 + 19 + 38 );

	/* Test error cases
	 */
	result = libcthreads_mutex_grab(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_mutex_grab with pthread_mutex_lock returning EAGAIN
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_lock_function_return_value = EAGAIN;

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( cthreads_test_pthread_mutex_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_grab with pthread_mutex_lock returning EDEADLK
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_lock_function_return_value = EDEADLK;

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( cthreads_test_pthread_mutex_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_grab with pthread_mutex_lock returning EBUSY
	 */
	cthreads_test_pthread_mutex_lock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_lock_function_return_value = EBUSY;

	result = libcthreads_mutex_grab(
	          cthreads_test_mutex,
	          &error );

	if( cthreads_test_pthread_mutex_lock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_lock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_mutex_free(
	          &cthreads_test_mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( thread2 != NULL )
	{
		libcthreads_thread_join(
		 &thread2,
		 NULL );
	}
	if( thread1 != NULL )
	{
		libcthreads_thread_join(
		 &thread1,
		 NULL );
	}
	if( cthreads_test_mutex != NULL )
	{
		libcthreads_mutex_release(
		 cthreads_test_mutex,
		 NULL );
		libcthreads_mutex_free(
		 &cthreads_test_mutex,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_mutex_try_grab function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_mutex_try_grab(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_mutex_t *mutex = NULL;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcthreads_mutex_try_grab(
	          mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_mutex_grab with pthread_mutex_trylock returing EBUSY
	 */
	cthreads_test_pthread_mutex_trylock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_trylock_function_return_value = EBUSY;

	result = libcthreads_mutex_try_grab(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_trylock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		CTHREADS_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test libcthreads_mutex_grab with pthread_mutex_trylock returing EAGAIN
	 */
	cthreads_test_pthread_mutex_trylock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_trylock_function_return_value = EAGAIN;

	result = libcthreads_mutex_try_grab(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_trylock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_grab with pthread_mutex_trylock returing EDEADLK
	 */
	cthreads_test_pthread_mutex_trylock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_trylock_function_return_value = EDEADLK;

	result = libcthreads_mutex_try_grab(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_trylock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_grab with pthread_mutex_trylock returing ENOMEM
	 */
	cthreads_test_pthread_mutex_trylock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_trylock_function_return_value = ENOMEM;

	result = libcthreads_mutex_try_grab(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_trylock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_trylock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Test error cases
	 */
	result = libcthreads_mutex_try_grab(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_mutex_grab with pthread_mutex_trylock failing
	 */

	/* TODO add tests */

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_mutex_release(
		  mutex,
		  &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_backtrace_fprint(
		 error,
		 stdout );

		libcerror_error_free(
		 &error );
	}
	if( mutex != NULL )
	{
		libcthreads_mutex_free(
		 &mutex,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcthreads_mutex_release function
 * Returns 1 if successful or 0 if not
 */
int cthreads_test_mutex_release(
     void )
{
	libcerror_error_t *error   = NULL;
	libcthreads_mutex_t *mutex = NULL;
	int result                 = 0;

	/* Initialize test
	 */
	result = libcthreads_mutex_initialize(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcthreads_mutex_release(
	          NULL,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CTHREADS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ )

	/* Test libcthreads_mutex_release with pthread_mutex_unlock returning EAGAIN
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_unlock_function_return_value = EAGAIN;

	result = libcthreads_mutex_release(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_release with pthread_mutex_unlock returning EDEADLK
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_unlock_function_return_value = EDEADLK;

	result = libcthreads_mutex_release(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcthreads_mutex_release with pthread_mutex_unlock returning EBUSY
	 */
	cthreads_test_pthread_mutex_unlock_attempts_before_fail       = 0;
	cthreads_test_real_pthread_mutex_unlock_function_return_value = EBUSY;

	result = libcthreads_mutex_release(
	          mutex,
	          &error );

	if( cthreads_test_pthread_mutex_unlock_attempts_before_fail != -1 )
	{
		cthreads_test_pthread_mutex_unlock_attempts_before_fail = -1;
	}
	else
	{
		CTHREADS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CTHREADS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __CYGWIN__ ) */

	/* Clean up
	 */
	result = libcthreads_mutex_free(
	          &mutex,
	          &error );

	CTHREADS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CTHREADS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( mutex != NULL )
	{
		libcthreads_mutex_free(
		 &mutex,
		 NULL );
	}
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
	 "libcthreads_mutex_initialize",
	 cthreads_test_mutex_initialize );

	CTHREADS_TEST_RUN(
	 "libcthreads_mutex_free",
	 cthreads_test_mutex_free );

	CTHREADS_TEST_RUN(
	 "libcthreads_mutex_grab",
	 cthreads_test_mutex_grab );

	CTHREADS_TEST_RUN(
	 "libcthreads_mutex_try_grab",
	 cthreads_test_mutex_try_grab );

	CTHREADS_TEST_RUN(
	 "libcthreads_mutex_release",
	 cthreads_test_mutex_release );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}


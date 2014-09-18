/********************************************************************************
 * Copyright (C) 2014 Daichi Teruya (@maruuusa83)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * This program is destributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *******************************************************************************/
#ifndef __LAMBDA_UTILS_H__
#define __LAMBDA_UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
	ID = 0,
	ABS,
	APP
} LambdaTermType;


/* structures */
struct _app;
struct _lambda_abstraction;
struct _lambda_term;

typedef struct _lambda_application {
	struct _lambda_term *to;
	struct _lambda_term *arg;
} LambdaApplication;

typedef struct _lambda_abstraction {
	char *param;
	struct _lambda_term *lambda_term;
} LambdaAbstraction;

typedef struct _lambda_term {
	LambdaTermType type;

	union {
        char *identifier;
		LambdaAbstraction *abs;
		LambdaApplication app;
	} data;
} LambdaTerm;

typedef struct _lambda_defined_term {
	struct _lambda_defined_term *next;
	char *name;
	LambdaTerm *term;
} LambdaDefinedTerm;


/* function prototype declaration */
char *lambda_create_identifier(char *str);
void lambda_free_identifier(char *id);

LambdaAbstraction *lambda_create_abstraction(char *param, LambdaTerm *term);
void lambda_free_abstraction(LambdaAbstraction *abs);

LambdaTerm *lambda_create_term(LambdaTermType type);
void lambda_free_term(LambdaTerm *term);

LambdaTerm *lambda_create_term_id(char *id);
LambdaTerm *lambda_create_term_abs(LambdaAbstraction *abs);
LambdaTerm *lambda_create_term_app(LambdaTerm *t1, LambdaTerm *t2);
LambdaTerm *lambda_create_term_clone(LambdaTerm *term);

int lambda_add_defined_term(char *name, LambdaTerm *term);
LambdaTerm *lambda_get_defined_term(char *name);

LambdaTerm *lambda_eval(LambdaTerm *term);
LambdaTerm *lambda_application(LambdaTerm *terget, char *target_id, LambdaTerm *arg);

void lambda_print_term(LambdaTerm *term);

#endif /* __LAMBDA_UTILS_H__ */


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
#include "lambda_utils.h"

/* global val */
LambdaDefinedTerm defined_term_list;

char *lambda_create_identifier(char *str)
{
	char *id = (char *)malloc((sizeof(char) * strlen(str)) + 1);
	strcpy(id, str);

	return (id);
}
void lambda_free_identifier(char *id)
{
    free(id);
}

LambdaAbstraction *lambda_create_abstraction(char *param, LambdaTerm *term)
{
    LambdaAbstraction *abs = (LambdaAbstraction *)malloc(sizeof(LambdaAbstraction));
	abs->param = (char *)malloc(strlen(param) + 1);

	strcpy(abs->param, param);
	abs->lambda_term = lambda_create_term_clone(term);

	return (abs);
}
void lambda_free_abstraction(LambdaAbstraction *abs)
{
    free(abs->param);
	lambda_free_term(abs->lambda_term);
	free(abs);
}

LambdaTerm *lambda_create_term_id(char *id)
{
    LambdaTerm *term = lambda_create_term(ID);
	(term->data).identifier = (char *)malloc(strlen(id) + 1);

	strcpy((term->data).identifier, id);

	return (term);
}

LambdaTerm *lambda_create_term_abs(LambdaAbstraction *abs)
{
	LambdaTerm *term = lambda_create_term(ABS);

	(term->data).abs = lambda_create_abstraction(abs->param, abs->lambda_term);

	return (term);
}

LambdaTerm *lambda_create_term_app(LambdaTerm *t1, LambdaTerm *t2)
{
	LambdaTerm *term = lambda_create_term(APP);
	
	(term->data).app.to  = lambda_create_term_clone(t1);
	(term->data).app.arg = lambda_create_term_clone(t2);

	return (term);
}

LambdaTerm *lambda_application(LambdaTerm *target, char *target_id, LambdaTerm *arg)
{
	LambdaTerm *result = NULL;

	LambdaTerm *t1, *t2;

#ifdef __LAMBDA_DEBUG__
	printf("\n");
	printf("app(%s) : ", target_id);
	lambda_print_term(target);
	printf("\n");
	lambda_print_term(arg);
	printf("\n");
#endif /* __LAMBDA_DEBUG__ */

    switch (target->type){
	  case ID:
		if (strcmp(target_id, (target->data).identifier) == 0){
			result = lambda_create_term_clone(arg);
		}
		else {
			result = lambda_create_term_clone(target);
		}
		break;

	  case ABS:
		if (strcmp(((target->data).abs)->param, target_id) != 0){
		    t1 = lambda_application(((target->data).abs)->lambda_term, target_id, arg);
			result = lambda_create_term_clone(target);
			lambda_free_term(((result->data).abs)->lambda_term);
			((result->data).abs)->lambda_term = t1;
		}
		else {
			result = lambda_create_term_clone(target);
		}
		break;

	  case APP:
		t1 = lambda_application(((target->data).app).to, target_id, arg);
		t2 = lambda_application(((target->data).app).arg, target_id, arg);
		result = lambda_create_term_app(t1, t2);
		break;
	}


	return (result);
}

LambdaTerm *lambda_create_term_clone(LambdaTerm *term)
{
	LambdaTerm *result = NULL;

	switch (term->type){
	  case ID:
		result = (LambdaTerm *)lambda_create_term_id((term->data).identifier);
		break;

	  case ABS:
		result = (LambdaTerm *)lambda_create_term_abs((term->data).abs);
		break;

	  case APP:
		result = (LambdaTerm *)lambda_create_term_app(((term->data).app).to, ((term->data).app).arg);
		break;
	}

	return (result);
}

LambdaTerm *lambda_create_term(LambdaTermType type)
{
	LambdaTerm *e_term = (LambdaTerm *)malloc(sizeof(LambdaTerm));

	e_term->type = type;

	return (e_term);
}

void lambda_free_term(LambdaTerm *term)
{
	switch (term->type){
	  case ID:
		free((term->data).identifier);
		break;

	  case ABS:
		lambda_free_term(((term->data).abs)->lambda_term);
		free(((term->data).abs)->param);

		free((term->data).abs);
		break;

	  case APP:
		lambda_free_term((term->data).app.to);
		lambda_free_term((term->data).app.arg);
		break;
	}

	free(term);
}

int lambda_add_defined_term(char *name, LambdaTerm *term)
{
	LambdaDefinedTerm *pos = &defined_term_list;
	LambdaDefinedTerm *new_defined_term;

	while (pos->next != NULL){
		pos = pos->next;

		if (strcmp(pos->name, name) == 0){
			return (-1);
		}
	}

	new_defined_term = (LambdaDefinedTerm *)malloc(sizeof(LambdaDefinedTerm));

	new_defined_term->next = NULL;
	new_defined_term->name = (char *)malloc(strlen(name) + 1);
	strcpy(new_defined_term->name, name);
	new_defined_term->term = lambda_create_term_clone(term);

	pos->next = new_defined_term;

	return (0);
}

LambdaTerm *lambda_get_defined_term(char *name)
{
	LambdaDefinedTerm *pos = &defined_term_list;

	while (pos->next != NULL){
		pos = pos->next;

		if (strcmp(pos->name, name) == 0){
			return (lambda_create_term_clone(pos->term));
		}
	}

	return (NULL);
}

LambdaTerm *lambda_eval(LambdaTerm *term)
{
	LambdaTerm *result = NULL;
	LambdaTerm *evaled1, *evaled2;
	LambdaTerm *temp;

#ifdef __LAMBDA_DEBUG__
	printf("eval : ");
	lambda_print_term(term);
	printf("\n");
#endif /* __LAMBDA_DEBUG__ */

    switch (term->type){
	  case ID:
	  case ABS:
		result = lambda_create_term_clone(term);
		break;
	
	  case APP:
		evaled1 = lambda_eval(((term->data).app).to);
		//evaled2 = lambda_eval(((term->data).app).arg);
		evaled2 = ((term->data).app).arg;

		switch (evaled1->type){
		  case ID:
		  case APP:
			temp = lambda_create_term_app(evaled1, evaled2);
			break;

		  case ABS:
			temp = lambda_application(((evaled1->data).abs)->lambda_term, ((evaled1->data).abs)->param, evaled2);
			break;
		}
		result = lambda_eval(temp);
		break;
	}

	return (result);
}

void lambda_print_term(LambdaTerm *term)
{
    printf("(");

    switch (term->type){
	  case ID:
		printf("%s", (term->data).identifier);
		break;

	  case ABS:
		printf("lambda %s.", ((term->data).abs)->param);
		lambda_print_term(((term->data).abs)->lambda_term);
		break;

	  case APP:
		lambda_print_term(((term->data).app).to);
		lambda_print_term(((term->data).app).arg);
		break;
	}

	printf(")");
}


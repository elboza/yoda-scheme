#include<stdio.h>
#include<string.h>
#include "object.h"
#include "stream.h"
#include "env.h"
#include "eval.h"
#include "print.h"

char is_self_evaluating(object_t *exp) {
	return is_boolean(exp)   ||
	is_fixnum(exp)    ||
	is_float(exp)     ||
	is_bottom(exp)    ||
	is_character(exp) ||
	is_string(exp);
}

char is_variable(object_t *expression) {
	return is_symbol(expression);
}

char is_tagged_list(object_t *expression, object_t *tag) {
	object_t *the_car;
	
	if (is_pair(expression)) {
		the_car = car(expression);
		return is_symbol(the_car) && (the_car == tag);
	}
	return 0;
}

char is_quoted(object_t *expression) {
	return is_tagged_list(expression, quote_symbol);
}

char is_quasiquoted(object_t *exp){
	return is_tagged_list(exp,quasiquote_symbol);
}

char is_unquoted(object_t *exp){
	return is_tagged_list(exp,unquote_symbol);
}
char is_unquoted_splicing(object_t *exp){
	return is_tagged_list(exp,unquote_splicing_symbol);
}

object_t *text_of_quotation(object_t *exp) {
	return cadr(exp);
}

char is_assignment(object_t *exp) {
	return is_tagged_list(exp, set_symbol);
}

object_t *assignment_variable(object_t *exp) {
	return car(cdr(exp));
}

object_t *assignment_value(object_t *exp) {
	return car(cdr(cdr(exp)));
}

char is_definition(object_t *exp) {
	return is_tagged_list(exp, define_symbol);
}

object_t *definition_variable(object_t *exp) {
	if (is_symbol(cadr(exp))) {
		return cadr(exp);
	}
	else {
		return caadr(exp);
	}
}

object_t *definition_value(object_t *exp) {
	if (is_symbol(cadr(exp))) {
		return caddr(exp);
	}
	else {
		return make_lambda(cdadr(exp), cddr(exp));
	}
}

object_t *make_if(object_t *predicate, object_t *consequent,object_t *alternative) {
	return cons(if_symbol,
				cons(predicate,
					 cons(consequent,
						  cons(alternative, the_empty_list))));
				}

char is_if(object_t *expression) {
    return is_tagged_list(expression, if_symbol);
}

object_t *if_predicate(object_t *exp) {
    return cadr(exp);
}

object_t *if_consequent(object_t *exp) {
    return caddr(exp);
}

object_t *if_alternative(object_t *exp) {
    if (is_the_empty_list(cdddr(exp))) {
        return false;
    }
    else {
        return cadddr(exp);
    }
}

object_t *make_lambda(object_t *parameters, object_t *body) {
    return cons(lambda_symbol, cons(parameters, body));
}

char is_lambda(object_t *exp) {
    return is_tagged_list(exp, lambda_symbol);
}

object_t *lambda_parameters(object_t *exp) {
    return cadr(exp);
}

object_t *lambda_body(object_t *exp) {
    return cddr(exp);
}

object_t *make_begin(object_t *seq) {
    return cons(begin_symbol, seq);
}

char is_begin(object_t *exp) {
    return is_tagged_list(exp, begin_symbol);
}

object_t *begin_actions(object_t *exp) {
    return cdr(exp);
}

char is_last_exp(object_t *seq) {
    return is_the_empty_list(cdr(seq));
}

object_t *first_exp(object_t *seq) {
    return car(seq);
}

object_t *rest_exps(object_t *seq) {
    return cdr(seq);
}

char is_cond(object_t *exp) {
    return is_tagged_list(exp, cond_symbol);
}

object_t *cond_clauses(object_t *exp) {
    return cdr(exp);
}

object_t *cond_predicate(object_t *clause) {
    return car(clause);
}

object_t *cond_actions(object_t *clause) {
    return cdr(clause);
}

char is_cond_else_clause(object_t *clause) {
    return cond_predicate(clause) == else_symbol;
}

object_t *sequence_to_exp(object_t *seq) {
    if (is_the_empty_list(seq)) {
        return seq;
    }
    else if (is_last_exp(seq)) {
        return first_exp(seq);
    }
    else {
        return make_begin(seq);
    }
}

object_t *expand_clauses(object_t *clauses) {
    object_t *first;
    object_t *rest;
    
    if (is_the_empty_list(clauses)) {
        return false;
    }
    else {
        first = car(clauses);
        rest  = cdr(clauses);
        if (is_cond_else_clause(first)) {
            if (is_the_empty_list(rest)) {
                return sequence_to_exp(cond_actions(first));
            }
            else {
                fprintf(stderr, "else clause isn't last cond->if");
                //exit(1);
				return bottom;
            }
        }
        else {
            return make_if(cond_predicate(first),
                           sequence_to_exp(cond_actions(first)),
                           expand_clauses(rest));
        }
    }
}

object_t *cond_to_if(object_t *exp) {
    return expand_clauses(cond_clauses(exp));
}

object_t *make_application(object_t *operator, object_t *operands) {
    return cons(operator, operands);
}

char is_application(object_t *exp) {
    return is_pair(exp);
}

object_t *operator(object_t *exp) {
    return car(exp);
}

object_t *operands(object_t *exp) {
    return cdr(exp);
}

char is_no_operands(object_t *ops) {
    return is_the_empty_list(ops);
}

object_t *first_operand(object_t *ops) {
    return car(ops);
}

object_t *rest_operands(object_t *ops) {
    return cdr(ops);
}

char is_letstar(object_t *exp) {
	return is_tagged_list(exp, letstar_symbol);
}

char is_let(object_t *exp) {
    return is_tagged_list(exp, let_symbol);
}

object_t *let_bindings(object_t *exp) {
    return cadr(exp);
}

object_t *let_body(object_t *exp) {
    return cddr(exp);
}

object_t *binding_parameter(object_t *binding) {
    return car(binding);
}

object_t *binding_argument(object_t *binding) {
    return cadr(binding);
}

object_t *bindings_parameters(object_t *bindings) {
    return is_the_empty_list(bindings) ?
               the_empty_list :
               cons(binding_parameter(car(bindings)),
                    bindings_parameters(cdr(bindings)));
}

object_t *bindings_arguments(object_t *bindings) {
    return is_the_empty_list(bindings) ?
               the_empty_list :
               cons(binding_argument(car(bindings)),
                    bindings_arguments(cdr(bindings)));
}

object_t *let_parameters(object_t *exp) {
    return bindings_parameters(let_bindings(exp));
}

object_t *let_arguments(object_t *exp) {
    return bindings_arguments(let_bindings(exp));
}

object_t *let_to_application(object_t *exp) {
    return make_application(
               make_lambda(let_parameters(exp),
                           let_body(exp)),
               let_arguments(exp));
}

object_t *letstar_expand(object_t *exp){
	object_t *e;
	if(is_the_empty_list(let_bindings(exp)))
	{
		return cons(let_symbol,cons(the_empty_list,let_body(exp)));
	}
	else{
		e=cons(let_symbol,
			 cons(cons(car(let_bindings(exp)),the_empty_list),cons(
				  letstar_expand(cons(letstar_symbol,cons(cdr(let_bindings(exp)),let_body(exp)))),the_empty_list)));
		return e;
	}
	return bottom;
}

char is_and(object_t *exp) {
    return is_tagged_list(exp, and_symbol);
}

object_t *and_tests(object_t *exp) {
    return cdr(exp);
}

char is_or(object_t *exp) {
    return is_tagged_list(exp, or_symbol);
}

object_t *or_tests(object_t *exp) {
    return cdr(exp);
}

object_t *apply_operator(object_t *arguments) {
    return car(arguments);
}

object_t *prepare_apply_operands(object_t *arguments) {
    if (is_the_empty_list(cdr(arguments))) {
        return car(arguments);
    }
    else {
        return cons(car(arguments),
                    prepare_apply_operands(cdr(arguments)));
    }
}

object_t *apply_operands(object_t *arguments) {
    return prepare_apply_operands(cdr(arguments));
}

object_t *eval_expression(object_t *arguments) {
    return car(arguments);
}

object_t *eval_environment(object_t *arguments) {
	if(!arguments) return the_global_environment;
	if(!cadr(arguments)) return the_global_environment;
    return cadr(arguments);
}

object_t *list_of_values(object_t *exps, object_t *env) {
    if (is_no_operands(exps)) {
        return the_empty_list;
    }
    else {
        return cons(eval(first_operand(exps), env),
                    list_of_values(rest_operands(exps), env));
    }
}

object_t *eval_assignment(object_t *exp, object_t *env) {
    set_variable_value(assignment_variable(exp), 
                       eval(assignment_value(exp), env),
                       env);
    return ok_symbol;
}

object_t *eval_definition(object_t *exp, object_t *env) {
    define_variable(definition_variable(exp), 
                    eval(definition_value(exp), env),
                    env);
    return ok_symbol;
}
object_t *append_lists(object_t *list1,object_t *list2){
	if(is_the_empty_list(list1)){
		return list2;
	}
// 	if(!is_pair(list1)){
// 		fprintf(stderr,"arg1 is not a list!\n");
// 		return bottom;
// 	}
// 	if(!is_pair(list2)){
// 		fprintf(stderr,"arg2 is not a list!\n");
// 		return bottom;
// 	}
	return cons(car(list1),append_lists(cdr(list1),list2));
}
object_t *qq_expand(object_t *exp,object_t *env){
	if(is_unquoted(exp)){
		return eval(text_of_quotation(exp),env);
	}
	else if(is_unquoted_splicing(exp)){
		fprintf(stderr,"invalid context for unquote-slicing.\n");
		return bottom;
	}
	else if(is_quasiquoted(exp)){
		//return cons(quote_symbol,cons(exp,the_empty_list));
		return exp;
	}
	else if(is_pair(exp)){
		return append_lists(qq_expand_list(car(exp),env),qq_expand(cdr(exp),env));
	}
	else{
		return exp;
	}
	//never reach this code
	return exp;
}

object_t *qq_expand_list(object_t *exp,object_t *env){
	if(is_unquoted(exp)){
		return cons(eval(text_of_quotation(exp),env),the_empty_list);
	}
	else if(is_unquoted_splicing(exp)){
		return cons(qq_expand(car(text_of_quotation(exp)),env),qq_expand(cdr(text_of_quotation(exp)),env));
	}
	else if(is_quasiquoted(exp)){
		//return cons(quote_symbol,cons(exp,the_empty_list));
		return cons(exp,the_empty_list);
	}
	else if(is_pair(exp)){
		//return cons(qq_expand_list(car(exp),env),qq_expand(cdr(exp),env));
		return cons(append_lists(qq_expand_list(car(exp),env),qq_expand(cdr(exp),env)),the_empty_list);
	}
	else{
		return cons(exp,the_empty_list);
	}
	//never reach this code
	return cons(exp,the_empty_list);
}

object_t *eval(object_t *exp, object_t *env) {
    object_t *procedure;
    object_t *arguments;
    object_t *result;

tailcall:
    if (is_self_evaluating(exp)) {
        return exp;
    }
    else if (is_variable(exp)) {
        return lookup_variable_value(exp, env);
    }
    else if (is_quoted(exp)) {
        return text_of_quotation(exp);
    }
    else if (is_quasiquoted(exp)) {
		exp=qq_expand(text_of_quotation(exp),env);
		//goto tailcall;
		return exp;
	}
//	else if (is_unquoted(exp)) {
//		exp=text_of_quotation(exp);
//		goto tailcall;
//		//return text_of_quotation(exp);
//	}
//	else if (is_unquoted_splicing(exp)) {
//		return text_of_quotation(exp);
//	}
    else if (is_assignment(exp)) {
        return eval_assignment(exp, env);
    }
    else if (is_definition(exp)) {
        return eval_definition(exp, env);
    }
    else if (is_if(exp)) {
        exp = is_true(eval(if_predicate(exp), env)) ?
                  if_consequent(exp) :
                  if_alternative(exp);
        goto tailcall;
    }
    else if (is_lambda(exp)) {
        return make_compound_proc(lambda_parameters(exp),
                                  lambda_body(exp),
                                  env);
    }
    else if (is_begin(exp)) {
        exp = begin_actions(exp);
        while (!is_last_exp(exp)) {
            eval(first_exp(exp), env);
            exp = rest_exps(exp);
        }
        exp = first_exp(exp);
        goto tailcall;
    }
    else if (is_cond(exp)) {
        exp = cond_to_if(exp);
        goto tailcall;
    }
    else if (is_let(exp)) {
        exp = let_to_application(exp);
        goto tailcall;
    }
    else if (is_letstar(exp)) {
		exp=letstar_expand(exp);
		goto tailcall;
	}
    else if (is_and(exp)) {
        exp = and_tests(exp);
        if (is_the_empty_list(exp)) {
            return true;
        }
        while (!is_last_exp(exp)) {
            result = eval(first_exp(exp), env);
            if (is_false(result)) {
                return result;
            }
            exp = rest_exps(exp);
        }
        exp = first_exp(exp);
        goto tailcall;
    }
    else if (is_or(exp)) {
        exp = or_tests(exp);
        if (is_the_empty_list(exp)) {
            return false;
        }
        while (!is_last_exp(exp)) {
            result = eval(first_exp(exp), env);
            if (is_true(result)) {
                return result;
            }
            exp = rest_exps(exp);
        }
        exp = first_exp(exp);
        goto tailcall;
    }
    else if (is_application(exp)) {
        procedure = eval(operator(exp), env);
        arguments = list_of_values(operands(exp), env);

        /* handle eval specially for tail call requirement */
        if (is_primitive_proc(procedure) && 
            procedure->data.primitive_proc.fn == eval_proc) {
            exp = eval_expression(arguments);
            env = eval_environment(arguments);
            goto tailcall;
        }

        /* handle apply specially for tail call requirement */
        if (is_primitive_proc(procedure) && 
            procedure->data.primitive_proc.fn == apply_proc) {
            procedure = apply_operator(arguments);
            arguments = apply_operands(arguments);
		}

        if (is_primitive_proc(procedure)) {
            return (procedure->data.primitive_proc.fn)(arguments);
        }
        else if (is_compound_proc(procedure)) {
            env = extend_environment( 
                       procedure->data.compound_proc.parameters,
                       arguments,
                       procedure->data.compound_proc.env);
            exp = make_begin(procedure->data.compound_proc.body);
            goto tailcall;
        }
		
        else {
            fprintf(stderr, "unknown procedure type\n");
            //exit(1);
			return bottom;
        }
    }
    else {
        fprintf(stderr, "cannot eval unknown expression type\n");
        //exit(1);
		return bottom;
    }
    fprintf(stderr, "eval illegal state\n");
    //exit(1);
	return bottom;
}

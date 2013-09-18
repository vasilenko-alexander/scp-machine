/*
-----------------------------------------------------------------------------
This source file is part of OSTIS (Open Semantic Technology for Intelligent Systems)
For the latest info, see http://www.ostis.net

Copyright (c) 2010-2013 OSTIS

OSTIS is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OSTIS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with OSTIS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#include "sc_memory_headers.h"
#include "scp_keynodes.h"
#include "scp_procedure_preprocessor.h"
#include "scp_interpreter_utils.h"
#include <glib.h>

#include <stdio.h>

sc_event *event_procedure_preprocessing;

void append_to_hash(GHashTable *table, scp_operand *elem)
{
    g_hash_table_add(table, MAKE_PHASH(elem));
}

void append_to_set(gpointer key, gpointer value, gpointer set)
{
    sc_addr elem;
    elem.offset = SC_ADDR_LOCAL_OFFSET_FROM_INT(GPOINTER_TO_INT(key));
    elem.seg = SC_ADDR_LOCAL_SEG_FROM_INT(GPOINTER_TO_INT(key));
    sc_memory_arc_new(sc_type_arc_pos_const_perm, ((scp_operand *)set)->addr, elem);
}

void gen_set_from_hash(GHashTable *table, scp_operand *set)
{
    g_hash_table_foreach(table, append_to_set, set);
}

void append_all_relation_elements_to_hash_with_modifiers(GHashTable *table, scp_operand *set, scp_operand *parameter_set, scp_operand *const_set, scp_operand *vars_set)
{
    scp_operand arc1, arc2, curr_operand, operand_arc, modifier, modifier_arc,
                operand_element, operand_element_arc, operand_element_modifier, operand_element_modifier_arc;
    scp_iterator3 *it1, *it2, *it3;
    MAKE_DEFAULT_ARC_ASSIGN(arc1);
    MAKE_DEFAULT_ARC_ASSIGN(arc2);
    MAKE_DEFAULT_ARC_ASSIGN(operand_arc);
    MAKE_DEFAULT_ARC_ASSIGN(modifier_arc);
    MAKE_DEFAULT_ARC_ASSIGN(operand_element_arc);
    MAKE_DEFAULT_ARC_ASSIGN(operand_element_modifier_arc);
    modifier_arc.erase = SCP_TRUE;
    operand_element_modifier_arc.erase = SCP_TRUE;
    MAKE_DEFAULT_OPERAND_ASSIGN(curr_operand);
    MAKE_DEFAULT_OPERAND_ASSIGN(operand_element);
    MAKE_DEFAULT_OPERAND_ASSIGN(operand_element_modifier);
    MAKE_DEFAULT_OPERAND_ASSIGN(modifier);
    // Operands loop
    it1 = scp_iterator3_new(set, &arc1, &curr_operand);
    while (SCP_RESULT_TRUE == scp_iterator3_next(it1, set, &operand_arc, &curr_operand))
    {
        curr_operand.param_type = SCP_FIXED;
        operand_arc.param_type = SCP_FIXED;

        append_to_hash(table, &curr_operand);
        append_to_hash(table, &operand_arc);

        // Operand modifiers loop
        it2 = scp_iterator3_new(&modifier, &arc1, &operand_arc);
        while (SCP_RESULT_TRUE == scp_iterator3_next(it2, &modifier, &modifier_arc, &operand_arc))
        {
            modifier.param_type = SCP_FIXED;
            modifier_arc.param_type = SCP_FIXED;

            append_to_hash(table, &modifier_arc);

            modifier_arc.param_type = SCP_ASSIGN;
            modifier.param_type = SCP_ASSIGN;
        }
        scp_iterator3_free(it2);

        // Operand elements loop
        it2 = scp_iterator3_new(&curr_operand, &operand_element_arc, &operand_element);
        while (SCP_RESULT_TRUE == scp_iterator3_next(it2, &curr_operand, &operand_element_arc, &operand_element))
        {
            operand_element.param_type = SCP_FIXED;
            operand_element_arc.param_type = SCP_FIXED;

            append_to_hash(table, &operand_element);
            append_to_hash(table, &operand_element_arc);

            // Operand element modifiers loop
            it3 = scp_iterator3_new(&operand_element_modifier, &operand_element_modifier_arc, &operand_element_arc);
            while (SCP_RESULT_TRUE == scp_iterator3_next(it3, &operand_element_modifier, &operand_element_modifier_arc, &operand_element_arc))
            {
                operand_element_modifier.param_type = SCP_FIXED;
                operand_element_modifier_arc.param_type = SCP_FIXED;

                // Variable case
                if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_var, &operand_element_modifier))
                {
                    if (SCP_RESULT_TRUE == searchElStr5(parameter_set, &arc1, &operand_element, &arc2, &rrel_in))
                    {
                        eraseEl(&operand_element_modifier_arc);
                        genElStr3(&rrel_scp_const, &arc1, &operand_element_arc);

                        arc1.param_type = SCP_FIXED;
                        append_to_hash(table, &arc1);
                        arc1.param_type = SCP_ASSIGN;

                        genElStr3(const_set, &arc1, &operand_element);
                    }
                    else
                    {
                        append_to_hash(table, &operand_element_modifier_arc);
                        genElStr3(vars_set, &arc1, &operand_element);
                    }
                }
                else
                {
                    append_to_hash(table, &operand_element_modifier_arc);
                    // Constant case
                    if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_const, &operand_element_modifier))
                    {
                        genElStr3(const_set, &arc1, &operand_element);
                    }
                }

                operand_element_modifier_arc.param_type = SCP_ASSIGN;
                operand_element_modifier.param_type = SCP_ASSIGN;
            }
            scp_iterator3_free(it3);

            operand_element_arc.param_type = SCP_ASSIGN;
            operand_element.param_type = SCP_ASSIGN;
        }
        scp_iterator3_free(it2);

        operand_arc.param_type = SCP_ASSIGN;
        curr_operand.param_type = SCP_ASSIGN;
    }
    scp_iterator3_free(it1);
}

void append_all_set_elements_to_hash_with_modifiers(GHashTable *table, scp_operand *set, scp_operand *parameter_set, scp_operand *const_set, scp_operand *vars_set)
{
    scp_operand arc1, arc2, curr_operand, operand_arc, modifier, modifier_arc;
    scp_iterator3 *it1, *it2;
    MAKE_DEFAULT_ARC_ASSIGN(arc1);
    MAKE_DEFAULT_ARC_ASSIGN(arc2);
    MAKE_DEFAULT_ARC_ASSIGN(operand_arc);
    MAKE_DEFAULT_ARC_ASSIGN(modifier_arc);
    modifier_arc.erase = SCP_TRUE;
    MAKE_DEFAULT_OPERAND_ASSIGN(curr_operand);
    MAKE_DEFAULT_OPERAND_ASSIGN(modifier);
    // Elements loop
    it1 = scp_iterator3_new(set, &operand_arc, &curr_operand);
    while (SCP_RESULT_TRUE == scp_iterator3_next(it1, set, &operand_arc, &curr_operand))
    {
        curr_operand.param_type = SCP_FIXED;
        operand_arc.param_type = SCP_FIXED;

        append_to_hash(table, &curr_operand);
        append_to_hash(table, &operand_arc);

        // Operand modifiers loop
        it2 = scp_iterator3_new(&modifier, &modifier_arc, &operand_arc);
        while (SCP_RESULT_TRUE == scp_iterator3_next(it2, &modifier, &modifier_arc, &operand_arc))
        {
            modifier.param_type = SCP_FIXED;
            modifier_arc.param_type = SCP_FIXED;

            // Variable case
            if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_var, &modifier))
            {
                if (SCP_RESULT_TRUE == searchElStr5(parameter_set, &arc1, &curr_operand, &arc2, &rrel_in))
                {
                    eraseEl(&modifier_arc);
                    genElStr3(&rrel_scp_const, &arc1, &operand_arc);

                    arc1.param_type = SCP_FIXED;
                    append_to_hash(table, &arc1);
                    arc1.param_type = SCP_ASSIGN;

                    genElStr3(const_set, &arc1, &curr_operand);
                }
                else
                {
                    append_to_hash(table, &modifier_arc);
                    genElStr3(vars_set, &arc1, &curr_operand);
                }
            }
            else
            {
                append_to_hash(table, &modifier_arc);
                // Constant case
                if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_const, &modifier))
                {
                    genElStr3(const_set, &arc1, &curr_operand);
                }
            }

            modifier_arc.param_type = SCP_ASSIGN;
            modifier.param_type = SCP_ASSIGN;
        }
        scp_iterator3_free(it2);

        operand_arc.param_type = SCP_ASSIGN;
        curr_operand.param_type = SCP_ASSIGN;
    }
    scp_iterator3_free(it1);
}

scp_result gen_system_structures(scp_operand *operator_set, scp_operand *parameter_set, scp_operand *vars_set, scp_operand *const_set, scp_operand *operators_copying_pattern)
{
    scp_operand arc1, arc2, curr_operator, operator_arc, operator_type, curr_operand, operand_arc, modifier, modifier_arc;
    scp_iterator3 *it, *it1, *it2;
    GHashTable *table = g_hash_table_new(NULL, NULL);

    MAKE_DEFAULT_ARC_ASSIGN(arc1);
    MAKE_DEFAULT_ARC_ASSIGN(arc2);
    MAKE_DEFAULT_ARC_ASSIGN(operator_arc);
    MAKE_DEFAULT_ARC_ASSIGN(operand_arc);
    MAKE_DEFAULT_ARC_ASSIGN(modifier_arc);
    modifier_arc.erase = SCP_TRUE;
    MAKE_DEFAULT_OPERAND_ASSIGN(curr_operator);
    MAKE_DEFAULT_OPERAND_ASSIGN(curr_operand);
    MAKE_DEFAULT_OPERAND_ASSIGN(operator_arc);
    MAKE_DEFAULT_OPERAND_ASSIGN(operator_type);
    MAKE_DEFAULT_OPERAND_ASSIGN(modifier);
    append_to_hash(table, operator_set);
    it = scp_iterator3_new(operator_set, &operator_arc, &curr_operator);
    while (SCP_RESULT_TRUE == scp_iterator3_next(it, operator_set, &operator_arc, &curr_operator))
    {
        curr_operator.param_type = SCP_FIXED;
        operator_arc.param_type = SCP_FIXED;

        if (SCP_RESULT_TRUE == searchElStr3(&rrel_init, &arc1, &operator_arc))
        {
            append_to_hash(table, &arc1);
        }

        append_to_hash(table, &curr_operator);
        append_to_hash(table, &operator_arc);

        // Operator type loop
        operator_type.param_type = SCP_ASSIGN;
        it1 = scp_iterator3_new(&operator_type, &arc1, &curr_operator);
        while (SCP_RESULT_TRUE == scp_iterator3_next(it1, &operator_type, &arc1, &curr_operator))
        {
            operator_type.param_type = SCP_FIXED;

            if (SCP_RESULT_TRUE == searchElStr3(&scp_operator, &arc2, &operator_type))
            {
                append_to_hash(table, &arc1);
                operator_type.param_type = SCP_ASSIGN;
                break;
            }

            operator_type.param_type = SCP_ASSIGN;
        }
        scp_iterator3_free(it1);
        operator_type.param_type = SCP_FIXED;

        // Operands loop
        it1 = scp_iterator3_new(&curr_operator, &operand_arc, &curr_operand);
        while (SCP_RESULT_TRUE == scp_iterator3_next(it1, &curr_operator, &operand_arc, &curr_operand))
        {
            curr_operand.param_type = SCP_FIXED;
            operand_arc.param_type = SCP_FIXED;

            append_to_hash(table, &curr_operand);
            append_to_hash(table, &operand_arc);

            // Operand modifiers loop
            it2 = scp_iterator3_new(&modifier, &modifier_arc, &operand_arc);
            while (SCP_RESULT_TRUE == scp_iterator3_next(it2, &modifier, &modifier_arc, &operand_arc))
            {
                modifier.param_type = SCP_FIXED;
                modifier_arc.param_type = SCP_FIXED;

                if (SCP_RESULT_TRUE == ifCoin(&operator_type, &op_call)
                    && SCP_RESULT_TRUE == ifCoin(&modifier, ordinal_rrels + 2))
                {
                    append_all_set_elements_to_hash_with_modifiers(table, &curr_operand, parameter_set, const_set, vars_set);
                }

                if (((SCP_RESULT_TRUE == ifCoin(&operator_type, &op_sys_gen)) || (SCP_RESULT_TRUE == ifCoin(&operator_type, &op_sys_search))) &&
                    ((SCP_RESULT_TRUE == ifCoin(&modifier, ordinal_rrels + 2)) || (SCP_RESULT_TRUE == ifCoin(&modifier, ordinal_rrels + 3))))
                {
                    append_all_relation_elements_to_hash_with_modifiers(table, &curr_operand, parameter_set, const_set, vars_set);
                }

                // Variable case
                if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_var, &modifier))
                {
                    if (SCP_RESULT_TRUE == searchElStr5(parameter_set, &arc1, &curr_operand, &arc2, &rrel_in))
                    {
                        eraseEl(&modifier_arc);
                        genElStr3(&rrel_scp_const, &arc1, &operand_arc);

                        arc1.param_type = SCP_FIXED;
                        append_to_hash(table, &arc1);
                        arc1.param_type = SCP_ASSIGN;

                        genElStr3(const_set, &arc1, &curr_operand);
                    }
                    else
                    {
                        append_to_hash(table, &modifier_arc);
                        genElStr3(vars_set, &arc1, &curr_operand);
                    }
                }
                else
                {
                    append_to_hash(table, &modifier_arc);
                    // Constant case
                    if (SCP_RESULT_TRUE == ifCoin(&rrel_scp_const, &modifier))
                    {
                        genElStr3(const_set, &arc1, &curr_operand);
                    }
                }

                modifier_arc.param_type = SCP_ASSIGN;
                modifier.param_type = SCP_ASSIGN;
            }
            scp_iterator3_free(it2);

            operand_arc.param_type = SCP_ASSIGN;
            curr_operand.param_type = SCP_ASSIGN;
        }
        scp_iterator3_free(it1);

        operator_arc.param_type = SCP_ASSIGN;
        curr_operator.param_type = SCP_ASSIGN;
    }
    scp_iterator3_free(it);

    //cantorize_set(const_set);
    //cantorize_set(vars_set);
    //printf("SIZE: %d\n", g_hash_table_size(table));
    //printEl(const_set);
    //printEl(vars_set);
    gen_set_from_hash(table, operators_copying_pattern);
    g_hash_table_destroy(table);
    return SCP_RESULT_TRUE;
}

sc_result preprocess_scp_procedure(sc_event *event, sc_addr arg)
{
    scp_operand arc1, arc2, scp_procedure_node, node1, question_node, scp_procedure_params,
                scp_procedure_consts, scp_procedure_vars, scp_procedure_operators, scp_procedure_operators_copying_pattern;
    MAKE_DEFAULT_OPERAND_FIXED(arc1);
    MAKE_DEFAULT_ARC_ASSIGN(arc2);

    arc1.addr = arg;

    MAKE_DEFAULT_NODE_ASSIGN(node1);
    MAKE_DEFAULT_NODE_ASSIGN(question_node);
    if (SCP_RESULT_TRUE != ifVarAssign(&arc1))
    {
        return SC_RESULT_ERROR;
    }
    searchElStr3(&node1, &arc1, &question_node);

    question_node.param_type = SCP_FIXED;
    if (SCP_RESULT_TRUE != searchElStr3(&question_scp_procedure_preprocessing_request, &arc2, &question_node))
    {
        return SC_RESULT_ERROR;
    }
    arc1.erase = SCP_TRUE;
    eraseEl(&arc1);
    printf("PREPROCESSING...\n");
    MAKE_DEFAULT_ARC_ASSIGN(arc1);
    MAKE_DEFAULT_OPERAND_ASSIGN(scp_procedure_node);
    if (SCP_RESULT_TRUE != searchElStr3(&question_node, &arc1, &scp_procedure_node))
    {
        print_error("scp-procedure preprocessing", "Can't find question parameter");
        return SC_RESULT_ERROR;
    }
    scp_procedure_node.param_type = SCP_FIXED;

    MAKE_DEFAULT_OPERAND_ASSIGN(scp_procedure_operators);
    searchElStr5(&scp_procedure_node, &arc1, &scp_procedure_operators, &arc2, &rrel_operators);
    scp_procedure_operators.param_type = SCP_FIXED;
    MAKE_DEFAULT_OPERAND_ASSIGN(scp_procedure_params);
    searchElStr5(&scp_procedure_node, &arc1, &scp_procedure_params, &arc2, &rrel_params);
    scp_procedure_params.param_type = SCP_FIXED;

    MAKE_DEFAULT_NODE_ASSIGN(scp_procedure_consts);
    genElStr5(&scp_procedure_node, &arc1, &scp_procedure_consts, &arc2, &rrel_consts);
    scp_procedure_consts.param_type = SCP_FIXED;
    MAKE_DEFAULT_NODE_ASSIGN(scp_procedure_vars);
    genElStr5(&scp_procedure_node, &arc1, &scp_procedure_vars, &arc2, &rrel_vars);
    scp_procedure_vars.param_type = SCP_FIXED;
    MAKE_DEFAULT_NODE_ASSIGN(scp_procedure_operators_copying_pattern);
    genElStr5(&scp_procedure_node, &arc1, &scp_procedure_operators_copying_pattern, &arc2, &rrel_operators_copying_pattern);
    scp_procedure_operators_copying_pattern.param_type = SCP_FIXED;
    if (SCP_RESULT_TRUE == gen_system_structures(&scp_procedure_operators, &scp_procedure_params, &scp_procedure_vars, &scp_procedure_consts, &scp_procedure_operators_copying_pattern))
    {
        finish_question_successfully(&question_node);
        printf("PREPROCESSING FINISHED\n");
        return SC_RESULT_ERROR;
    }
    else
    {
        finish_question_unsuccessfully(&question_node);
        return SC_RESULT_OK;
    }
}

scp_result scp_procedure_preprocessor_init()
{
    event_procedure_preprocessing = sc_event_new(question_initiated.addr, SC_EVENT_ADD_OUTPUT_ARC, 0, preprocess_scp_procedure, 0);
    if (event_procedure_preprocessing == nullptr)
        return SCP_RESULT_ERROR;
    return SCP_RESULT_TRUE;
}

scp_result scp_procedure_preprocessor_shutdown()
{
    sc_event_destroy(event_procedure_preprocessing);
    return SCP_RESULT_TRUE;
}

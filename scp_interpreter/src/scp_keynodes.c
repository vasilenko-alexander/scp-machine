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

#include "scp_interpreter_utils.h"

#include "scp_keynodes.h"
#include "string.h"
#include "stdio.h"

scp_operand scp_program;
scp_operand agent_scp_program;
scp_operand platform_independent_abstract_sc_agent;
scp_operand abstract_sc_agent;
scp_operand nrel_sc_agent_program;
scp_operand nrel_primary_initiation_condition;

scp_operand question_initiated;
scp_operand question_finished_successfully;
scp_operand question_finished_unsuccessfully;

scp_operand question_scp_interpretation_request;
scp_operand scp_process;
scp_operand abstract_scp_machine;
scp_operand useless_scp_process;
scp_operand nrel_scp_process;
scp_operand nrel_value;
scp_operand nrel_authors;
scp_operand nrel_system_identifier;

scp_operand active_scp_operator;
scp_operand active_sc_agent;

//scp-program sets
scp_operand formed_scp_program;
scp_operand correct_scp_program;
scp_operand incorrect_scp_program;
scp_operand prepared_scp_program;

// rrels

scp_operand nrel_then;
scp_operand nrel_else;
scp_operand nrel_goto;
scp_operand nrel_error;

scp_operand nrel_scp_program_var;
scp_operand nrel_scp_program_const;
scp_operand nrel_scp_program_copied_const;
scp_operand nrel_template_of_scp_process_creation;
scp_operand nrel_inclusion;
scp_operand rrel_params;
scp_operand rrel_operators;
scp_operand rrel_init;

scp_operand rrel_in;
scp_operand rrel_out;

scp_operand rrel_fixed;
scp_operand rrel_assign;
scp_operand rrel_set;
scp_operand rrel_erase;

scp_operand rrel_const;
scp_operand rrel_var;
scp_operand rrel_scp_const;
scp_operand rrel_scp_var;
scp_operand rrel_node;
scp_operand rrel_arc;
scp_operand rrel_edge;
scp_operand rrel_link;
scp_operand rrel_pos;
scp_operand rrel_neg;
scp_operand rrel_fuz;
scp_operand rrel_temp;
scp_operand rrel_perm;
scp_operand rrel_access;
scp_operand rrel_common;
scp_operand rrel_pos_const_perm;

// Events
scp_operand sc_event_elem;
scp_operand sc_event_add_output_arc;
scp_operand sc_event_add_input_arc;
scp_operand sc_event_remove_output_arc;
scp_operand sc_event_remove_input_arc;
scp_operand sc_event_change_link_content;

// Ordinals
scp_operand order_role_relation;
scp_operand ordinal_set_rrel;
scp_operand ordinal_rrels[ORDINAL_RRELS_COUNT + 1]; // 0 element reserved
scp_operand ordinal_set_rrels[ORDINAL_RRELS_COUNT + 1]; // 0 element reserved

#define resolve_keynode(keynode, keynode_str) \
    if (sc_helper_resolve_system_identifier(keynode_str, &(keynode)) == SC_FALSE) \
    {\
        g_warning("Can't find element with system identifier: %s", keynode_str); \
        keynode = sc_memory_node_new(0); \
        if (sc_helper_set_system_identifier(keynode, keynode_str, strlen(keynode_str)) != SC_RESULT_OK) \
            return SCP_RESULT_ERROR; \
        g_message("Created element with system identifier: %s", keynode_str); \
    }

scp_result scp_keynodes_init()
{
    scp_uint32 i = 0;
    char name[12];
    MAKE_DEFAULT_OPERAND_FIXED(scp_program);
    MAKE_DEFAULT_OPERAND_FIXED(agent_scp_program);

    MAKE_DEFAULT_OPERAND_FIXED(platform_independent_abstract_sc_agent);
    MAKE_DEFAULT_OPERAND_FIXED(abstract_sc_agent);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_sc_agent_program);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_primary_initiation_condition);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_inclusion);

    MAKE_DEFAULT_OPERAND_FIXED(question_scp_interpretation_request);
    MAKE_DEFAULT_OPERAND_FIXED(question_initiated);
    MAKE_DEFAULT_OPERAND_FIXED(question_finished_successfully);
    MAKE_DEFAULT_OPERAND_FIXED(question_finished_unsuccessfully);
    MAKE_DEFAULT_OPERAND_FIXED(useless_scp_process);
    MAKE_DEFAULT_OPERAND_FIXED(scp_process);
    MAKE_DEFAULT_OPERAND_FIXED(abstract_scp_machine);
    MAKE_DEFAULT_OPERAND_FIXED(formed_scp_program);
    MAKE_DEFAULT_OPERAND_FIXED(correct_scp_program);
    MAKE_DEFAULT_OPERAND_FIXED(incorrect_scp_program);
    MAKE_DEFAULT_OPERAND_FIXED(prepared_scp_program);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_scp_process);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_system_identifier);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_value);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_authors);
    MAKE_DEFAULT_OPERAND_FIXED(active_scp_operator);
    MAKE_DEFAULT_OPERAND_FIXED(active_sc_agent);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_then);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_else);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_goto);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_error);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_scp_program_var);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_scp_program_const);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_scp_program_copied_const);
    MAKE_DEFAULT_OPERAND_FIXED(nrel_template_of_scp_process_creation);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_params);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_operators);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_init);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_in);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_out);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_fixed);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_assign);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_set);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_erase);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_const);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_var);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_scp_const);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_scp_var);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_node);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_link);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_arc);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_edge);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_pos);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_neg);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_fuz);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_temp);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_perm);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_access);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_common);
    MAKE_DEFAULT_OPERAND_FIXED(rrel_pos_const_perm);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_elem);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_add_input_arc);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_add_output_arc);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_change_link_content);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_remove_input_arc);
    MAKE_DEFAULT_OPERAND_FIXED(sc_event_remove_output_arc);
    MAKE_DEFAULT_OPERAND_FIXED(order_role_relation);
    MAKE_DEFAULT_OPERAND_FIXED(ordinal_set_rrel);
    for (i = 1; i <= ORDINAL_RRELS_COUNT; i++)
    {
        MAKE_DEFAULT_OPERAND_FIXED(ordinal_rrels[i]);
    }
    for (i = 1; i <= ORDINAL_SET_RRELS_COUNT; i++)
    {
        MAKE_DEFAULT_OPERAND_FIXED(ordinal_set_rrels[i]);
    }

    resolve_keynode(scp_program.addr,"scp_program");
    resolve_keynode(abstract_scp_machine.addr,"abstract_scp_machine");
    resolve_keynode(formed_scp_program.addr,"formed_scp_program");
    resolve_keynode(correct_scp_program.addr,"correct_scp_program");
    resolve_keynode(incorrect_scp_program.addr,"incorrect_scp_program");
    resolve_keynode(prepared_scp_program.addr,"prepared_scp_program");
    resolve_keynode(agent_scp_program.addr,"agent_scp_program");
    resolve_keynode(platform_independent_abstract_sc_agent.addr,"platform_independent_abstract_sc_agent");
    resolve_keynode(abstract_sc_agent.addr,"abstract_sc_agent");
    resolve_keynode(nrel_sc_agent_program.addr,"nrel_sc_agent_program");
    resolve_keynode(nrel_primary_initiation_condition.addr,"nrel_primary_initiation_condition");
    resolve_keynode(nrel_inclusion.addr,"nrel_inclusion");
    resolve_keynode(question_scp_interpretation_request.addr,"question_scp_interpretation_request");
    resolve_keynode(question_initiated.addr,"question_initiated");
    resolve_keynode(question_finished_successfully.addr,"question_finished_successfully");
    resolve_keynode(question_finished_unsuccessfully.addr,"question_finished_unsuccessfully");
    resolve_keynode(useless_scp_process.addr,"useless_scp_process");
    resolve_keynode(scp_process.addr,"scp_process");
    resolve_keynode(nrel_scp_process.addr,"nrel_scp_process");
    resolve_keynode(nrel_authors.addr,"nrel_authors");
    resolve_keynode(nrel_system_identifier.addr,"nrel_system_identifier");
    resolve_keynode(active_scp_operator.addr,"active_scp_operator");
    resolve_keynode(active_sc_agent.addr,"active_sc_agent");
    resolve_keynode(nrel_then.addr,"nrel_then");
    resolve_keynode(nrel_else.addr,"nrel_else");
    resolve_keynode(nrel_goto.addr,"nrel_goto");
    resolve_keynode(nrel_error.addr,"nrel_error");
    resolve_keynode(nrel_scp_program_var.addr,"nrel_scp_program_var");
    resolve_keynode(nrel_scp_program_const.addr,"nrel_scp_program_const");
    resolve_keynode(nrel_scp_program_copied_const.addr,"nrel_scp_program_copied_const");
    resolve_keynode(nrel_template_of_scp_process_creation.addr,"nrel_template_of_scp_process_creation");
    resolve_keynode(rrel_params.addr,"rrel_params");
    resolve_keynode(rrel_operators.addr,"rrel_operators");
    resolve_keynode(nrel_value.addr,"nrel_scp_var_value");
    resolve_keynode(rrel_init.addr,"rrel_init");
    resolve_keynode(rrel_in.addr,"rrel_in");
    resolve_keynode(rrel_out.addr,"rrel_out");
    resolve_keynode(rrel_fixed.addr,"rrel_fixed");
    resolve_keynode(rrel_assign.addr,"rrel_assign");
    resolve_keynode(rrel_set.addr,"rrel_set");
    resolve_keynode(rrel_erase.addr,"rrel_erase");
    resolve_keynode(rrel_const.addr,"rrel_const");
    resolve_keynode(rrel_var.addr,"rrel_var");
    resolve_keynode(rrel_scp_const.addr,"rrel_scp_const");
    resolve_keynode(rrel_scp_var.addr,"rrel_scp_var");
    resolve_keynode(rrel_node.addr,"rrel_node");
    resolve_keynode(rrel_arc.addr,"rrel_arc");
    resolve_keynode(rrel_edge.addr,"rrel_edge");
    resolve_keynode(rrel_link.addr,"rrel_link");
    resolve_keynode(rrel_pos.addr,"rrel_pos");
    resolve_keynode(rrel_neg.addr,"rrel_neg");
    resolve_keynode(rrel_fuz.addr,"rrel_fuz");
    resolve_keynode(rrel_temp.addr,"rrel_temp");
    resolve_keynode(rrel_perm.addr,"rrel_perm");
    resolve_keynode(rrel_access.addr,"rrel_access");
    resolve_keynode(rrel_common.addr,"rrel_common");
    resolve_keynode(rrel_pos_const_perm.addr,"rrel_pos_const_perm");
    resolve_keynode(sc_event_elem.addr,"sc_event");
    resolve_keynode(sc_event_add_input_arc.addr,"sc_event_add_input_arc");
    resolve_keynode(sc_event_add_output_arc.addr,"sc_event_add_output_arc");
    resolve_keynode(sc_event_remove_input_arc.addr,"sc_event_remove_input_arc");
    resolve_keynode(sc_event_remove_output_arc.addr,"sc_event_remove_output_arc");
    resolve_keynode(sc_event_change_link_content.addr,"sc_event_change_link_content");
    resolve_keynode(order_role_relation.addr,"order_role_relation");

    for (i = 1; i <= ORDINAL_RRELS_COUNT; i++)
    {
        snprintf(name, 8, "rrel_%d", i);
        resolve_keynode(ordinal_rrels[i].addr,name);
    }
    for (i = 1; i <= ORDINAL_SET_RRELS_COUNT; i++)
    {
        snprintf(name, 12, "rrel_set_%d", i);
        resolve_keynode(ordinal_set_rrels[i].addr,name);
    }

    return init_operator_keynodes();
}

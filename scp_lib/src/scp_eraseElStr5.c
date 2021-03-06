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
but WITHOUT ANY WARRANTY{} without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with OSTIS.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#include "sc_memory_headers.h"

#include "scp_eraseElStr5.h"
#include "scp_utils.h"

scp_result eraseElStr5F(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    if (param1->erase == SCP_TRUE)
    {
        sc_memory_element_free(param1->addr);
    }
    if (param3->erase == SCP_TRUE)
    {
        sc_memory_element_free(param3->addr);
    }
    if (param5->erase == SCP_TRUE)
    {
        sc_memory_element_free(param5->addr);
    }
    if (SC_TRUE == sc_memory_is_element(param2->addr) && param2->erase == SCP_TRUE)
    {
        sc_memory_element_free(param2->addr);
    }
    if (SC_TRUE == sc_memory_is_element(param4->addr) && param4->erase == SCP_TRUE)
    {
        sc_memory_element_free(param4->addr);
    }
    return SCP_RESULT_TRUE;
}

scp_result eraseElStr5_f_a_a_a_a(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_f_a_a_a_a_new(param1->addr, param2->element_type, param3->element_type, param4->element_type, param5->element_type);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param2->addr = sc_iterator5_value(it, 1);
        param3->addr = sc_iterator5_value(it, 2);
        param4->addr = sc_iterator5_value(it, 3);
        param5->addr = sc_iterator5_value(it, 4);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param1->erase == SCP_TRUE)
    {
        sc_memory_element_free(param1->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_a_a_f_a_a(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_a_a_f_a_a_new(param1->element_type, param2->element_type, param3->addr, param4->element_type, param5->element_type);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param1->addr = sc_iterator5_value(it, 0);
        param2->addr = sc_iterator5_value(it, 1);
        param4->addr = sc_iterator5_value(it, 3);
        param5->addr = sc_iterator5_value(it, 4);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param3->erase == SCP_TRUE)
    {
        sc_memory_element_free(param3->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_a_a_a_a_f(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator3 *it = sc_iterator3_f_a_a_new(param5->addr, param4->element_type, param2->element_type);
    if (SC_TRUE == sc_iterator3_next(it))
    {
        param4->addr = sc_iterator3_value(it, 1);
        param2->addr = sc_iterator3_value(it, 2);
        sc_memory_get_arc_begin(param2->addr, &(param1->addr));
        sc_memory_get_arc_end(param2->addr, &(param3->addr));
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator3_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param5->erase == SCP_TRUE)
    {
        sc_memory_element_free(param5->addr);
    }
    sc_iterator3_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_f_a_f_a_a(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_f_a_f_a_a_new(param1->addr, param2->element_type, param3->addr, param4->element_type, param5->element_type);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param2->addr = sc_iterator5_value(it, 1);
        param4->addr = sc_iterator5_value(it, 3);
        param5->addr = sc_iterator5_value(it, 4);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param1->erase == SCP_TRUE)
    {
        sc_memory_element_free(param1->addr);
    }
    if (param3->erase == SCP_TRUE)
    {
        sc_memory_element_free(param3->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_f_a_a_a_f(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_f_a_a_a_f_new(param1->addr, param2->element_type, param3->element_type, param4->element_type, param5->addr);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param2->addr = sc_iterator5_value(it, 1);
        param3->addr = sc_iterator5_value(it, 2);
        param4->addr = sc_iterator5_value(it, 3);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param1->erase == SCP_TRUE)
    {
        sc_memory_element_free(param1->addr);
    }
    if (param5->erase == SCP_TRUE)
    {
        sc_memory_element_free(param5->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_a_a_f_a_f(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_a_a_f_a_f_new(param1->element_type, param2->element_type, param3->addr, param4->element_type, param5->addr);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param1->addr = sc_iterator5_value(it, 0);
        param2->addr = sc_iterator5_value(it, 1);
        param4->addr = sc_iterator5_value(it, 3);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param3->erase == SCP_TRUE)
    {
        sc_memory_element_free(param3->addr);
    }
    if (param5->erase == SCP_TRUE)
    {
        sc_memory_element_free(param5->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}
scp_result eraseElStr5_f_a_f_a_f(scp_operand *param1, scp_operand *param2, scp_operand *param3, scp_operand *param4, scp_operand *param5)
{
    sc_iterator5 *it = sc_iterator5_f_a_f_a_f_new(param1->addr, param2->element_type, param3->addr, param4->element_type, param5->addr);
    if (SC_TRUE == sc_iterator5_next(it))
    {
        param2->addr = sc_iterator5_value(it, 1);
        param4->addr = sc_iterator5_value(it, 3);
        eraseElStr5F(param1, param2, param3, param4, param5);
        sc_iterator5_free(it);
        return SCP_RESULT_TRUE;
    }
    if (param1->erase == SCP_TRUE)
    {
        sc_memory_element_free(param1->addr);
    }
    if (param3->erase == SCP_TRUE)
    {
        sc_memory_element_free(param3->addr);
    }
    if (param5->erase == SCP_TRUE)
    {
        sc_memory_element_free(param5->addr);
    }
    sc_iterator5_free(it);
    return SCP_RESULT_FALSE;
}

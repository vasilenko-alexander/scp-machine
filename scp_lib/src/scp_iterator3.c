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

#include "scp_iterator3.h"
#include "scp_utils.h"

scp_iterator3 *scp_iterator3_new(scp_operand *param1, scp_operand *param2, scp_operand *param3)
{
    sc_uint32 fixed1 = 0;
    sc_uint32 fixed3 = 0;
    sc_uint32 fixed = 0;
    if (param2->param_type == SCP_FIXED)
    {
        print_error("SCP ITERATOR 3 NEW", "Parameter 2 must have ASSIGN modifier");
        return nullptr;
    }
    if (param1->param_type == SCP_FIXED)
    {
        if (SC_FALSE == sc_memory_is_element(param1->addr))
        {
            print_error("SCP ITERATOR 3 NEW", "Parameter 1 has modifier FIXED, but has not value");
            return nullptr;
        }
        fixed1 = 0x1;
    }
    if (param3->param_type == SCP_FIXED)
    {
        if (SC_FALSE == sc_memory_is_element(param3->addr))
        {
            print_error("SCP ITERATOR 3 NEW", "Parameter 3 has modifier FIXED, but has not value");
            return nullptr;
        }
        fixed3 = 0x100;
    }
    fixed = (fixed1 | fixed3);
    switch (fixed)
    {
        case 0x001:
            return (scp_iterator3 *)sc_iterator3_f_a_a_new(param1->addr, param2->element_type, param3->element_type);
        case 0x100:
            return (scp_iterator3 *)sc_iterator3_a_a_f_new(param1->element_type, param2->element_type, param3->addr);
        case 0x101:
            return (scp_iterator3 *)sc_iterator3_f_a_f_new(param1->addr, param2->element_type, param3->addr);
        default:
            print_error("SCP ITERATOR 3 NEW", "Unsupported parameter type combination");
            return nullptr;
    }
    return nullptr;
}

scp_result scp_iterator3_next(scp_iterator3 *iter, scp_operand *param1, scp_operand *param2, scp_operand *param3)
{
    sc_uint32 fixed1 = 0;
    sc_uint32 fixed2 = 0;
    sc_uint32 fixed3 = 0;
    sc_uint32 fixed = 0;
    if (param2->param_type == SCP_FIXED)
    {
        return print_error("SCP ITERATOR 3 NEXT", "Parameter 2 must have ASSIGN modifier");
    }
    if (param1->param_type == SCP_FIXED)
    {
        if (SC_FALSE == sc_memory_is_element(param1->addr))
        {
            return print_error("SCP ITERATOR 3 NEXT", "Parameter 1 has modifier FIXED, but has not value");
        }
        fixed1 = 0x1;
    }
    if (param3->param_type == SCP_FIXED)
    {
        if (SC_FALSE == sc_memory_is_element(param3->addr))
        {
            return print_error("SCP ITERATOR 3 NEXT", "Parameter 3 has modifier FIXED, but has not value");
        }
        fixed3 = 0x100;
    }
    fixed = (fixed1 | fixed2 | fixed3);
    switch (fixed)
    {
        case 0x001:
            if (iter->type != sc_iterator3_f_a_a)
            {
                return print_error("SCP ITERATOR 3 NEXT", "Iterator type and parameter type combination doesn't match");
            }
            else
            {
                if (SC_TRUE == sc_iterator3_next(iter))
                {
                    param2->addr = sc_iterator3_value(iter, 1);
                    param3->addr = sc_iterator3_value(iter, 2);
                    return SCP_RESULT_TRUE;
                }
                else
                {
                    return SCP_RESULT_FALSE;
                }
            }
        case 0x100:
            if (iter->type != sc_iterator3_a_a_f)
            {
                return print_error("SCP ITERATOR 3 NEXT", "Iterator type and parameter type combination doesn't match");
            }
            else
            {
                if (SC_TRUE == sc_iterator3_next(iter))
                {
                    param1->addr = sc_iterator3_value(iter, 0);
                    param2->addr = sc_iterator3_value(iter, 1);
                    return SCP_RESULT_TRUE;
                }
                else
                {
                    return SCP_RESULT_FALSE;
                }
            }
        case 0x101:
            if (iter->type != sc_iterator3_f_a_f)
            {
                return print_error("SCP ITERATOR 3 NEXT", "Iterator type and parameter type combination doesn't match");
            }
            else
            {
                if (SC_TRUE == sc_iterator3_next(iter))
                {
                    param2->addr = sc_iterator3_value(iter, 1);
                    return SCP_RESULT_TRUE;
                }
                else
                {
                    return SCP_RESULT_FALSE;
                }
            }
        default:
            return print_error("SCP ITERATOR 3 NEXT", "Unsupported parameter type combination");
    }
    return SCP_RESULT_ERROR;
}

void scp_iterator3_free(scp_iterator3 *iter)
{
    sc_iterator3_free(iter);
}

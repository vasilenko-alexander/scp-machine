
#include "scp_lib.h"
#include "sc_helper.h"
//#include "sc_memory_headers.h"

#include <stdio.h>

#include <glib.h>

/*void merge_test()
{
    sc_addr quest, init, elem2, attr;
    scp_operand op;

    sc_helper_resolve_system_identifier("test_question", &quest);
    sc_helper_resolve_system_identifier("question_initiated", &init);
    sc_helper_resolve_system_identifier("elem2", &elem2);
    sc_helper_resolve_system_identifier("attr2", &attr);

    op.addr = elem2;
    printEl(&op);
    op.addr = attr;
    printEl(&op);

    sc_memory_arc_new(sc_type_arc_pos_const_perm, init, quest);

    op.addr = elem2;
    printEl(&op);
    op.addr = attr;
    printEl(&op);
}

void temp()
{
    sc_addr quest, init, elem2, attr;
    scp_operand op;

    sc_helper_resolve_system_identifier("sc_agent_of_set_cantorization_result", &quest);

    op.addr = quest;
    printEl(&op);

}*/

void create_process_test()
{
    sc_addr quest, init, nrel_answer, active_sc_agent, agent;
    scp_operand op;

    sc_helper_resolve_system_identifier("quest1", &quest);
    sc_helper_resolve_system_identifier("question_initiated", &init);
    sc_helper_resolve_system_identifier("nrel_answer", &nrel_answer);

    sc_helper_resolve_system_identifier("active_sc_agent", &active_sc_agent);
    sc_helper_resolve_system_identifier("sc_agent_of_search_of_all_output_arcs_agent_scp", &agent);

    sc_memory_arc_new(sc_type_arc_pos_const_perm, init, quest);

    sc_addr arc = sc_memory_arc_new(sc_type_arc_pos_const_perm, active_sc_agent, agent);

    sc_memory_arc_new(sc_type_arc_pos_const_perm, init, quest);

    /*sc_memory_element_free(arc);

    sc_memory_arc_new(sc_type_arc_pos_const_perm, init, quest);*/
}

void test_scp_process_creating(int value)
{
    int i = 0;
    for (i = 0; i < value; i++)
    {
        create_process_test();
    }
}

int main(void)
{
    sc_memory_initialize((sc_char *)"repo", (sc_char *)"sctp_config.ini", SC_FALSE);
    sc_helper_init();
    sc_memory_initialize_ext("extensions");

    GTimer *timer = 0;
    timer = g_timer_new();
    g_timer_start(timer);

    test_scp_process_creating(1);
    //merge_test();
    //temp();

    g_timer_stop(timer);
    printf((sc_char *)"Time: %f s\n", g_timer_elapsed(timer, 0));
    g_timer_destroy(timer);

    return 0;
}

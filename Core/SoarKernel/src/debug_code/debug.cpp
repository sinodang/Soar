/*************************************************************************
 * PLEASE SEE THE FILE "COPYING" (INCLUDED WITH THIS SOFTWARE PACKAGE)
 * FOR LICENSE AND COPYRIGHT INFORMATION.
 *************************************************************************/

/*------------------------------------------------------------------
                       debug.cpp

   @brief debug.cpp provides some utility functions for inspecting and
          manipulating the data structures of the Soar kernel at run
          time.

------------------------------------------------------------------ */

#include "debug.h"

#include "agent.h"
#include "condition.h"
#include "ebc.h"
#include "episodic_memory.h"
#include "lexer.h"
#include "print.h"
#include "rhs.h"
#include "soar_module.h"
#include "soar_instance.h"
#include "test.h"
#include "output_manager.h"
#include "working_memory.h"

#include <string>
#include <iostream>
#include <sstream>
#include "dprint.h"

/* -- For stack trace printing (works on Mac.  Not sure about other platforms) --*/
#ifdef DEBUG_MAC_STACKTRACE
#include <execinfo.h>
#include <cxxabi.h>
#include "debug_stacktrace.h"
#endif

using namespace soar_module;

bool symbol_matches_string(Symbol* sym, const char* match)
{
    if (sym->to_string() == match)
        return true;
    return false;
}
bool wme_matches_string(wme *w, const char* match_id, const char* match_attr, const char* match_value)
{
    if(symbol_matches_string(w->id, match_id) && symbol_matches_string(w->attr, match_attr) && symbol_matches_string(w->value, match_value))
        return true;
    return false;
}
bool id_matches(uint64_t lID, uint64_t lID_to_match)
{
    if (lID == lID_to_match)
        return true;
    return false;
}

void debug_set_mode_info(trace_mode_info mode_info[num_trace_modes], bool pEnabled)
{
    for (int i=0; i < num_trace_modes; i++)
    {
        mode_info[i].enabled = false;
    }
    if (pEnabled)
    {
        mode_info[No_Mode].enabled =                        true;
        mode_info[DT_DEBUG].enabled =                       true;

    }
}

void initialize_debug_trace(trace_mode_info mode_info[num_trace_modes])
{
    for (int i=0; i < num_trace_modes; i++)
    {
        mode_info[i].prefix = NULL;
    }

    mode_info[No_Mode].prefix =                         strdup("        | ");
    mode_info[DT_DEBUG].prefix =                        strdup("Debug   | ");

    mode_info[DT_MILESTONES].prefix =                   strdup("Milestne| ");
    mode_info[DT_PRINT_INSTANTIATIONS].prefix =         strdup("PrntInst| ");

    mode_info[DT_ADD_EXPLANATION_TRACE].prefix =        strdup("AddAddtn| ");
    mode_info[DT_IDENTITY_GENERATION].prefix =          strdup("ID Prop | ");

    mode_info[DT_VARIABLIZATION_MANAGER].prefix =       strdup("VrblzMgr| ");
    mode_info[DT_EXTRA_RESULTS].prefix =                strdup("ExtraRes| ");
    mode_info[DT_BACKTRACE].prefix =                    strdup("BackTrce| ");
    mode_info[DT_ADD_IDENTITY_SET_MAPPING].prefix =     strdup("Unify   | ");
    mode_info[DT_UNIFY_SINGLETONS].prefix =             strdup("Unify_S | ");
    mode_info[DT_BUILD_CHUNK_CONDS].prefix =            strdup("BChnkCnd| ");
    mode_info[DT_LHS_VARIABLIZATION].prefix =           strdup("VrblzLHS| ");
    mode_info[DT_RHS_VARIABLIZATION].prefix =           strdup("VrblzRHS| ");
    mode_info[DT_RHS_FUN_VARIABLIZATION].prefix =       strdup("RHS Func| ");
    mode_info[DT_NCC_VARIABLIZATION].prefix =           strdup("VrblzNCC| ");
    mode_info[DT_RL_VARIABLIZATION].prefix =            strdup("Vrblz RL| ");
    mode_info[DT_CONSTRAINTS].prefix =                  strdup("Cnstrnts| ");
    mode_info[DT_MERGE].prefix =                        strdup("Merge Cs| ");
    mode_info[DT_REORDERER].prefix =                    strdup("Reorder | ");
    mode_info[DT_REPAIR].prefix =                       strdup("Repair  | ");
    mode_info[DT_REINSTANTIATE].prefix =                strdup("ReInst  | ");
    mode_info[DT_EBC_CLEANUP].prefix =                  strdup("CleanUp | ");
    mode_info[DT_CLONES].prefix =                       strdup("Clones  | ");

    mode_info[DT_EXPLAIN].prefix =                      strdup("Explain | ");
    mode_info[DT_EXPLAIN_PATHS].prefix =                strdup("EIDPaths| ");
    mode_info[DT_EXPLAIN_ADD_INST].prefix =             strdup("EAddInst| ");
    mode_info[DT_EXPLAIN_CONNECT].prefix =              strdup("EConnect| ");
    mode_info[DT_EXPLAIN_UPDATE].prefix =               strdup("EUpdate | ");
    mode_info[DT_EXPLAIN_CONDS].prefix =                strdup("EConds  | ");
    mode_info[DT_EXPLAIN_IDENTITIES].prefix =           strdup("EIdent  | ");
    mode_info[DT_EXPLAIN_CACHE].prefix =                strdup("ExpCache| ");

    mode_info[DT_EPMEM_CMD].prefix =                    strdup("EpMemCmd| ");
    mode_info[DT_GDS].prefix =                          strdup("GDS     | ");
    mode_info[DT_GDS_HIGH].prefix =                     strdup("GDS High| ");
    mode_info[DT_SMEM_INSTANCE].prefix =                strdup("SMemInst| ");
    mode_info[DT_PARSER].prefix =                       strdup("Parser  | ");
    mode_info[DT_SOAR_INSTANCE].prefix =                strdup("SoarInst| ");
    mode_info[DT_WME_CHANGES].prefix =                  strdup("WMEChngs| ");

    mode_info[DT_ALLOCATE_RHS_VALUE].prefix =           strdup("MakeRHSv| ");
    mode_info[DT_ID_LEAKING].prefix =                   strdup("ID Leak | ");
    mode_info[DT_DEALLOCATE_INST].prefix =              strdup("Del Inst| ");
    mode_info[DT_DEALLOCATE_PREF].prefix =              strdup("Del Pref| ");
    mode_info[DT_DEALLOCATE_PROD].prefix =              strdup("Del Prod| ");
    mode_info[DT_DEALLOCATE_RHS_VALUE].prefix =         strdup("Del RHSv| ");
    mode_info[DT_DEALLOCATE_SLOT].prefix =              strdup("Del Slot| ");
    mode_info[DT_DEALLOCATE_SYMBOL].prefix =            strdup("Del Sym | ");
    mode_info[DT_DEALLOCATE_TEST].prefix =              strdup("Del Test| ");
    mode_info[DT_REFCOUNT_ADDS].prefix =                strdup("RefCnt  | ");
    mode_info[DT_REFCOUNT_REMS].prefix =                strdup("RefCnt  | ");

    mode_info[DT_LINKS].prefix =                        strdup("Links   | ");
    mode_info[DT_UNKNOWN_LEVEL].prefix =                strdup("No Level| ");
    mode_info[DT_PREFS].prefix =                        strdup("Prefs   | ");
    mode_info[DT_RETE_PNODE_ADD].prefix =               strdup("ReteNode| ");
    mode_info[DT_WATERFALL].prefix =                    strdup("Waterfal| ");
    mode_info[DT_DEEP_COPY].prefix =                    strdup("DeepCopy| ");
    mode_info[DT_RHS_LTI_LINKING].prefix =              strdup("RHS LTI | ");

    /* In case we forget to add a trace prefix */
    for (int i=0; i < num_trace_modes; i++)
    {
        if (mode_info[i].prefix == NULL)
        {
            mode_info[i].prefix =              strdup("???     | ");
        }
    }

#ifndef SOAR_RELEASE_VERSION
    debug_set_mode_info(mode_info, true);
#else
    debug_set_mode_info(mode_info, false);
#endif
}

debug_param_container::debug_param_container(agent* new_agent): soar_module::param_container(new_agent)
{
}

#ifdef DEBUG_REFCOUNT_DB

#include "output_manager.h"

void debug_store_refcount(Symbol* sym, bool isAdd)
{
    std::string caller_string = get_stacktrace(isAdd ? "add_ref" : "remove_ref");
    debug_agent->outputManager->store_refcount(sym, caller_string.c_str() , isAdd);
}

#endif

#ifdef DEBUG_MAC_STACKTRACE
std::string get_stacktrace(const char* prefix)
{
    // storage array for stack trace data
    // you can change the size of the array to increase the depth of
    // the stack summarized in the string returned
    void* addrlist[12];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0)
    {
        return std::string("<empty, possibly corrupt>");
    }

    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);
    std::string return_string;
    if (prefix)
    {
        return_string += prefix;
        return_string +=  " | ";
    }
    // iterate over the returned symbol lines. skip the first two
    for (int i = 2; i < addrlen; i++)
    {
        return_string += tracey::demangle(std::string(symbollist[i]));
        if (i < (addrlen - 1))
        {
            return_string += " | ";
        }
    }
    free(funcname);
    free(symbollist);
    return return_string;
}
#endif

void debug_trace_off()
{
    if (Soar_Instance::Get_Soar_Instance().was_run_from_unit_test()) return;
    Output_Manager::Get_OM().clear_output_modes();

    agent* thisAgent = Output_Manager::Get_OM().get_default_agent();
    if (thisAgent)
    {
        thisAgent->outputManager->printa(thisAgent, "Debug trace messages disabled.\n");
    }
}

void debug_trace_on()
{
    if (Soar_Instance::Get_Soar_Instance().was_run_from_unit_test()) return;
    Output_Manager::Get_OM().restore_output_modes();
    Output_Manager::Get_OM().set_output_params_global(true);

    agent* thisAgent = Output_Manager::Get_OM().get_default_agent();
    if (thisAgent)
    {
        thisAgent->output_settings->set_output_params_agent(true);
        thisAgent->outputManager->printa(thisAgent, "Debug trace settings restored.\n");
    }
}

void debug_trace_set(int dt_num, bool pEnable)
{
    if (Soar_Instance::Get_Soar_Instance().was_run_from_unit_test()) return;
    if (dt_num < num_trace_modes)
    {
        if (dt_num == 0)
        {

             if (pEnable)
             {
                 debug_trace_on();
                 dprint(DT_DEBUG, "...testing debug trace output.\n");
             } else {
                 debug_trace_off();
             }
             return;
        } else {
            Output_Manager::Get_OM().set_output_mode(dt_num, pEnable);
        }
    }
}

void debug_print_rhs(rhs_value rv)
{
    agent* thisAgent = Output_Manager::Get_OM().get_default_agent();
    if (!thisAgent)
    {
        return;
    }
    dprint(DT_DEBUG, "%r\n", rv);
    Output_Manager::Get_OM().set_output_params_global(true);
    thisAgent->output_settings->set_output_params_agent(true);
}

void debug_test(int type)
{
    agent* thisAgent = Output_Manager::Get_OM().get_default_agent();
    if (!thisAgent)
    {
        return;
    }

    switch (type)
    {
        case 1:
        {
            Symbol *sym = thisAgent->symbolManager->find_identifier('L', 1);
            if (sym)
            {
                dprint(DT_DEBUG, "%y found with refcount of %u.\n", sym, sym->reference_count);
//                condition* dummy = make_condition(thisAgent);
//                thisAgent->ebChunker->generate_conditions_to_ground_lti(&dummy, sym);
            } else {
                dprint(DT_DEBUG, "Could not find symbol.\n");
            }
            break;
        }
        case 2:
//            rhs_value rv;
//            rv = reteloc_to_rhs_value(0,0);
//            dprint(DT_DEBUG, "NULL reteloc: %s", rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rv = reteloc_to_rhs_value(1,0);
//            dprint(DT_DEBUG, "Non-NULL reteloc: %s", !rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rv = funcall_list_to_rhs_value(NULL);
//            dprint(DT_DEBUG, "NULL funcall: %s", rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            cons* ls;
//            allocate_cons(thisAgent, &ls);
//            rv = funcall_list_to_rhs_value(ls);
//            dprint(DT_DEBUG, "Non-NULL funcall: %s", !rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            free_cons(thisAgent, ls);
//            rv = unboundvar_to_rhs_value(0);
//            dprint(DT_DEBUG, "NULL unbound: %s", rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rv = unboundvar_to_rhs_value(1);
//            dprint(DT_DEBUG, "Non-NULL unbound: %s", !rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rv = rhs_symbol_to_rhs_value(NULL);
//            dprint(DT_DEBUG, "NULL symbol: %s", rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rhs_symbol rs;
//            rs->referent = NULL;
//            rv = rhs_symbol_to_rhs_value(rs);
//            dprint(DT_DEBUG, "Semi-NULL symbol: %s", rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
//            rs->referent = thisAgent->symbolManager->find_identifier('S', 1);
//            rv = rhs_symbol_to_rhs_value(rs);
//            dprint(DT_DEBUG, "Non-NULL symbol: %s", !rhs_value_is_null(rv) ? "PASS\n" : "FAIL\n");
            break;
        case 3:
        {
            /* -- Print all wme's -- */
            dprint(DT_DEBUG, "%8");
            break;
        }
        case 4:
        {
            Symbol *sym = thisAgent->symbolManager->find_identifier('G', 1);
            if (sym)
            {
                dprint(DT_DEBUG, "G1 found.  level = %d, promoted level = %d.\n", static_cast<int64_t>(sym->id->level), static_cast<int64_t>(sym->id->promotion_level));
            } else {
                dprint(DT_DEBUG, "Could not find G1.\n");
            }
        }
            break;

        case 5:
        {
//            print_internal_symbols(thisAgent);
            dprint_identifiers(DT_DEBUG);
            break;
        }
        case 6:
        {
            dprint_variablization_table(DT_DEBUG);
            dprint_o_id_tables(DT_DEBUG);
            dprint_attachment_points(DT_DEBUG);
            dprint_constraints(DT_DEBUG);
            dprint_merge_map(DT_DEBUG);
            dprint_ovar_to_o_id_map(DT_DEBUG);
            dprint_unification_map(DT_DEBUG);
            dprint_identity_debug_map(DT_DEBUG);
            dprint_tables(DT_DEBUG);
            break;
        }
        case 7:
            /* -- Print all instantiations -- */
            dprint_all_inst(DT_DEBUG);
            break;
        case 8:
        {
            break;
        }
        case 9:
            break;
    }
}

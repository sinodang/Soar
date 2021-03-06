/*------------------------------------------------------------------
                       debug.h

   @brief debug.h provides some utility functions for inspecting and
          manipulating the data structures of the Soar kernel at run
          time.

          (Not much here now.  Will move some other utility stuff from
           experimental chunking and memory consolidation branches
           later.)

------------------------------------------------------------------ */

#ifndef SOARDEBUG_H
#define SOARDEBUG_H

#include "kernel.h"
#include "soar_module.h"

#ifndef SOAR_RELEASE_VERSION
    /* This can be used to turn off dprints except for a decision cycle range */
//    #define DEBUG_ONLY_AFTER_DC 184
//    #define DEBUG_ONLY_BEFORE_DC 185

    /* This can be used to turn off dprints except for a particular chunk */
//    #define DEBUG_ONLY_CHUNK_ID 23611
//    #define DEBUG_ONLY_CHUNK_ID_LAST 23613
#endif


extern void debug_set_mode_info(trace_mode_info mode_info[num_trace_modes], bool pEnabled);
extern void initialize_debug_trace(trace_mode_info mode_info[num_trace_modes]);
extern void debug_trace_set(int dt_num, bool pEnable);
extern void debug_trace_on();
extern void debug_trace_off();

bool symbol_matches_string(Symbol* sym, const char* match);
bool wme_matches_string(wme *w, const char* match_id, const char* match_attr, const char* match_value);
bool id_matches(uint64_t lID, uint64_t lID_to_match);

extern std::string get_stacktrace(const char* prefix = NULL);

extern void debug_test(int type = 1);

extern void debug_init_db(agent* thisAgent);
extern void debug_store_refcount(Symbol* sym, bool isAdd);
extern void debug_destroy_for_refcount(agent* delete_agent);

/**
 * @brief Contains the parameters for the debug command
 */
class debug_param_container: public soar_module::param_container
{
    public:

        debug_param_container(agent* new_agent);

};

#endif


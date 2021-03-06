/*
 * output_settings.h
 *
 *  Created on: Jul 14, 2016
 *      Author: mazzin
 */

#ifndef CORE_SOARKERNEL_SRC_OUTPUT_MANAGER_OUTPUT_SETTINGS_H_
#define CORE_SOARKERNEL_SRC_OUTPUT_MANAGER_OUTPUT_SETTINGS_H_

#include "kernel.h"
#include "soar_module.h"
#include "soar_db.h"
#include "sml_KernelSML.h"

class OM_Parameters: public soar_module::param_container
{
    public:

        OM_Parameters(agent* new_agent, uint64_t pOutput_sysparams[]);

        // storage
        soar_module::constant_param<soar_module::db_choices>* database;
        soar_module::string_param* path;
        soar_module::boolean_param* lazy_commit;
        soar_module::boolean_param* append_db;

        // performance
        soar_module::constant_param<soar_module::page_choices>* page_size;
        soar_module::integer_param* cache_size;
        soar_module::constant_param<soar_module::opt_choices>* opt;

        soar_module::integer_param* print_depth;
        soar_module::boolean_param* warnings;
        soar_module::boolean_param* echo_commands;

        soar_module::boolean_param* enabled;
        soar_module::boolean_param* callback_enabled;
        soar_module::boolean_param* stdout_enabled;

        soar_module::boolean_param* ctf;
        soar_module::boolean_param* clog;
        soar_module::boolean_param* help_cmd;
        soar_module::boolean_param* qhelp_cmd;

        void print_output_settings(agent* thisAgent);
        void print_output_summary(agent* thisAgent);

        void update_bool_setting(agent* thisAgent, soar_module::boolean_param* pChangedParam, sml::KernelSML* pKernelSML);
        void update_int_setting(agent* thisAgent, soar_module::integer_param* pChangedParam);

};


#endif /* CORE_SOARKERNEL_SRC_OUTPUT_MANAGER_OUTPUT_SETTINGS_H_ */

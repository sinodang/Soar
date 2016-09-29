/***********************************************************************
 *
 * file filters/set_filters.h
 *
 * node_set_test_any_filter
 *   Generic base filter used when you want to test a single node against a set
 *       and return a true/false value based on pairwise tests
 *   The 'any' set filter returns true if any test between node 'a' 
 *       and a node in the set returned true
 *
 *   Functions:
 *       set_a_first(bool flag)
 *           Sets the value of the 'a_first' flag
 *           If this is true, for a node 'b' in the input set this will calculate test(a, b)
 *           If this is false, for a node 'b' in the input set this will calculate test(b, a)
 *   
 * node_set_test_all_filter
 *   Generic base filter used when you want to test a single node against a set
 *       and return a true/false value based on pairwise tests
 *   The 'all' set filter returns true if all tests between node 'a' 
 *       and every node in the set returned true
 *
 *   Functions:
 *       set_a_first(bool flag)
 *           Sets the value of the 'a_first' flag
 *           If this is true, for a node 'b' in the input set this will calculate test(a, b)
 *           If this is false, for a node 'b' in the input set this will calculate test(b, a)
 *
 *
 ******************************************************************************/
#ifndef __SET_FILTERS_H__
#define __SET_FILTERS_H__

#include "filter.h"
#include "filters/base_node_filters.h"

class node_set_test_any_filter : public set_computation_filter<bool>
{
    public:
        node_set_test_any_filter(Symbol* root, soar_interface* si, filter_input* input, node_test* test)
            : set_computation_filter<bool>(root, si, input), test(test), a_first(true)
        {}

        bool input_added(const filter_params* p);
        bool input_changed(const filter_params* p);
        bool input_removed(const filter_params* p);

        bool compute(const std::set<const filter_params*>& input_set, bool& out, bool& show_out);

        void set_a_first(bool flag)
        {
            a_first = flag;
        }

    private:
        bool a_first;
        node_test* test;

        std::map<const sgnode*, bool> node_cache;
};

class node_set_test_all_filter : public set_computation_filter<bool>
{
    public:
        node_set_test_all_filter(Symbol* root, soar_interface* si, filter_input* input, node_test* test)
            : set_computation_filter<bool>(root, si, input), test(test), a_first(true)
        {}

        bool input_added(const filter_params* p);
        bool input_changed(const filter_params* p);
        bool input_removed(const filter_params* p);

        bool compute(const std::set<const filter_params*>& input_set, bool& out, bool& show_out);

        void set_a_first(bool flag)
        {
            a_first = flag;
        }

    private:
        bool a_first;
        node_test* test;

        std::map<const sgnode*, bool> node_cache;
};

#endif

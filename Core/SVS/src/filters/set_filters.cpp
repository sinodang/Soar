
#include "filters/set_filters.h"

///////// node_set_test_any_filter /////////

bool node_set_test_any_filter::input_added(const filter_params* p)
{
    // Same behavior as input_changed
    return input_changed(p);
}

bool node_set_test_any_filter::input_changed(const filter_params* p)
{
    sgnode* a = NULL;
    sgnode* set_node = NULL;
    
    if (!get_filter_param(this, p, "a", a)
            || !get_filter_param(this, p, "set", set_node))
    {
        set_status("Need nodes a and set as input");
        return false;
    }

    get_filter_param(this, p, "a_first", a_first);

    if (a_first)
    {
        node_cache[p] = test(a, set_node, p);
    }
    else
    {
        node_cache[p] = test(set_node, a, p);
    }

    return true;
}

bool node_set_test_any_filter::input_removed(const filter_params* p){
    node_cache.erase(p);
    return true;
}

bool node_set_test_any_filter::compute(const std::set<const filter_params*>& input_set, bool& out, bool& show_out){
    if (input_set.size() == 0)
    {
        show_out = false;
    }
    show_out = true;
    out = false;
    // Check all cached values, if any is true then the output is true (OR operation)
    for(std::map<const filter_params*, bool>::const_iterator i = node_cache.begin(); i != node_cache.end(); i++)
    {
        if (i->second == true)
        {
            out = true;
            return true;
        }
    }
    return true;
}


///////// node_set_test_all_filter /////////

bool node_set_test_all_filter::input_added(const filter_params* p)
{
    // Same behavior as input_changed
    return input_changed(p);
}

bool node_set_test_all_filter::input_changed(const filter_params* p)
{
    sgnode* a = NULL;
    sgnode* set_node = NULL;
    
    if (!get_filter_param(this, p, "a", a)
            || !get_filter_param(this, p, "set", set_node))
    {
        set_status("Need nodes a and set as input");
        return false;
    }

    get_filter_param(this, p, "a_first", a_first);

    if (a_first)
    {
        node_cache[p] = test(a, set_node, p);
    }
    else
    {
        node_cache[p] = test(set_node, a, p);
    }

    return true;
}

bool node_set_test_all_filter::input_removed(const filter_params* p){
    node_cache.erase(p);
    return true;
}

bool node_set_test_all_filter::compute(const std::set<const filter_params*>& input_set, bool& out, bool& show_out){
    if (input_set.size() == 0)
    {
        show_out = false;
    }
    show_out = true;
    out = true;
    // Check all cached values, if any is false then the output is false (AND operation)
    for(std::map<const filter_params*, bool>::const_iterator i = node_cache.begin(); i != node_cache.end(); i++)
    {
        if (i->second == false)
        {
            out = false;
            return true;
        }
    }
    return true;
}

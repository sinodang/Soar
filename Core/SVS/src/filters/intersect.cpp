/***************************************************
 *
 * File: filters/intersect.cpp
 *
 * Intersect Filters
 *  bool intersect_test(sgnode* a, sgnode* b, fp* p)
 *    Returns true if a intersects b
 *    intersect_type << bbox hull >>
 *      what kind of intersection to test for, defaults to bbox
 *
 *  Filter intersect : node_test_filter
 *    Parameters:
 *      sgnode a
 *      sgnode b
 *    Returns
 *      bool - true if a intersects b
 *
 *  Filter intersect_select : node_test_select_filter
 *    Parameters:
 *      sgnode a
 *      sgnode b
 *    Returns:
 *      sgnode b - if a intersects b
 *
 *********************************************************/
#include "sgnode_algs.h"
#include "filters/base_node_filters.h"
#include "filters/set_filters.h"
#include "scene.h"
#include "filter_table.h"

#include <string>

using namespace std;

bool intersect_test(sgnode* a, sgnode* b, const filter_params* p)
{
    if (a == b)
    {
        return true;
    }
    string int_type = "bbox";
    get_filter_param(0, p, "intersect_type", int_type);
    if (int_type == "hull")
    {
        return convex_intersects(a, b);
    }
    else
    {
        return bbox_intersects(a, b);
    }
}

////// filter intersect //////
filter* make_intersect_filter(Symbol* root, soar_interface* si, scene* scn, filter_input* input)
{
    return new node_test_filter(root, si, input, &intersect_test);
}

filter_table_entry* intersect_filter_entry()
{
    filter_table_entry* e = new filter_table_entry();
    e->name = "intersect";
    e->description = "Returns true if a intersects b";
    e->parameters["a"] = "Sgnode a";
    e->parameters["b"] = "Sgnode b";
		e->parameters["intersect_type"] = "Either bbox or hull";
    e->create = &make_intersect_filter;
    return e;
}

////// filter intersect_select //////
filter* make_intersect_select_filter(Symbol* root, soar_interface* si, scene* scn, filter_input* input)
{
    return new node_test_select_filter(root, si, input, &intersect_test);
}

filter_table_entry* intersect_select_filter_entry()
{
    filter_table_entry* e = new filter_table_entry();
    e->name = "intersect_select";
    e->description = "Selects b if a intersects b";
    e->parameters["a"] = "Sgnode a";
    e->parameters["b"] = "Sgnode b";
		e->parameters["intersect_type"] = "Either bbox or hull";
    e->create = &make_intersect_select_filter;
    return e;
}

/////// filter intersect_any //////
filter* make_intersect_any_filter(Symbol* root, soar_interface* si, scene* scn, filter_input* input)
{
    return new node_set_test_any_filter(root, si, input, &intersect_test);
}

filter_table_entry* intersect_any_filter_entry()
{
    filter_table_entry* e = new filter_table_entry();
    e->name = "intersect_any";
    e->description = "returns a boolean, true if node a intersects any in the given set";
    e->parameters["a"] = "A single sgnode to test against the given set";
    e->parameters["set"] = "Set of sgnodes to test against";
    e->parameters["intersect_type"] = "Either bbox or hull";
    e->parameters["a_first"] = "If true computes test(a, b), if false computes test(b, a), for each node b in set";
    e->create = &make_intersect_any_filter;
    return e;
}

/////// filter intersect_all //////
filter* make_intersect_all_filter(Symbol* root, soar_interface* si, scene* scn, filter_input* input)
{
    return new node_set_test_all_filter(root, si, input, &intersect_test);
}

filter_table_entry* intersect_all_filter_entry()
{
    filter_table_entry* e = new filter_table_entry();
    e->name = "intersect_all";
    e->description = "returns a boolean, true if node a intersects all nodes in the given set";
    e->parameters["a"] = "A single sgnode to test against the given set";
    e->parameters["set"] = "Set of sgnodes to test against";
    e->parameters["intersect_type"] = "Either bbox or hull";
    e->parameters["a_first"] = "If true computes test(a, b), if false computes test(b, a), for each node b in set";
    e->create = &make_intersect_all_filter;
    return e;
}


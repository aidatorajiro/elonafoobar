#pragma once

#include <unordered_map>
#include "cat.hpp"


namespace elona
{


struct trait_data
{
    enum class Type
    {
        feat,
        mutation,
        nature,
        ether_disease,
    };

    int id;
    Type type;
    int min;
    int max;
};



class TraitDB;


namespace cat
{

template <>
struct cat_db_traits<TraitDB>
{
    using id_type = int;
    using data_type = trait_data;
    static constexpr const char* filename = u8"trait.lua";
    static constexpr const char* table_name = u8"trait";
};

} // namespace cat



class TraitDB : public cat::CatDB<TraitDB>
{
public:
    TraitDB() = default;

    void define(lua_State* L);
};



extern TraitDB the_trait_db;


int trait_get_info(int traitmode, int tid);
void trait_load_desc();



} // namespace elona

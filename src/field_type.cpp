#include "field_type.h"

#include <set>

#include "debug.h"
#include "enums.h"
#include "generic_factory.h"
#include "json.h"
#include "int_id.h"

namespace
{

generic_factory<field_type> all_field_types( "field types" );

} // namespace

/** @relates int_id */
template<>
bool int_id<field_type>::is_valid() const
{
    return all_field_types.is_valid( *this );
}

/** @relates int_id */
template<>
const field_type &int_id<field_type>::obj() const
{
    return all_field_types.obj( *this );
}

/** @relates int_id */
template<>
const string_id<field_type> &int_id<field_type>::id() const
{
    return all_field_types.convert( *this );
}

/** @relates string_id */
template<>
bool string_id<field_type>::is_valid() const
{
    return all_field_types.is_valid( *this );
}

/** @relates string_id */
template<>
const field_type &string_id<field_type>::obj() const
{
    return all_field_types.obj( *this );
}

/** @relates string_id */
template<>
int_id<field_type> string_id<field_type>::id() const
{
    return all_field_types.convert( *this, fd_null );
}

/** @relates int_id */
template<>
int_id<field_type>::int_id( const string_id<field_type> &id ) : _id( id.id() )
{
}

void field_type::load( JsonObject &jo, const std::string & )
{
    optional( jo, was_loaded, "legacy_enum_id", legacy_enum_id, -1 );
    JsonArray ja = jo.get_array( "intensity_levels" );
    for( size_t i = 0; i < ja.size(); ++i ) {
        field_intensity_level intensity_level;
        field_intensity_level fallback_intensity_level = i > 0 ? intensity_levels[i - 1] : intensity_level;
        JsonObject jao = ja.get_object( i );
        optional( jao, was_loaded, "name", intensity_level.name, fallback_intensity_level.name );
        optional( jao, was_loaded, "sym", intensity_level.symbol, unicode_codepoint_from_symbol_reader,
                  fallback_intensity_level.symbol );
        intensity_level.color = jao.has_member( "color" ) ? color_from_string( jao.get_string( "color" ) ) :
                                fallback_intensity_level.color;
        optional( jao, was_loaded, "transparent", intensity_level.transparent,
                  fallback_intensity_level.transparent );
        optional( jao, was_loaded, "dangerous", intensity_level.dangerous,
                  fallback_intensity_level.dangerous );
        optional( jao, was_loaded, "move_cost", intensity_level.move_cost,
                  fallback_intensity_level.move_cost );
        intensity_levels.emplace_back( intensity_level );
    }
    optional( jo, was_loaded, "underwater_age_speedup", underwater_age_speedup, 0_turns );
    optional( jo, was_loaded, "priority", priority, 0 );
    optional( jo, was_loaded, "half_life", half_life, 0_turns );
    if( jo.has_member( "phase" ) ) {
        phase = jo.get_enum_value<phase_id>( "phase", PNULL );
    }
    optional( jo, was_loaded, "accelerated_decay", accelerated_decay, false );
    optional( jo, was_loaded, "display_items", display_items, true );
    optional( jo, was_loaded, "display_field", display_field, false );
}

void field_type::check() const
{
    if( intensity_levels.empty() ) {
        debugmsg( "No intensity levels defined for field type \"%s\".", id.c_str() );
    }
    int i = 0;
    for( auto &intensity_level : intensity_levels ) {
        i++;
        if( intensity_level.name.empty() ) {
            debugmsg( "Intensity level %d defined for field type \"%s\" has empty name.", i, id.c_str() );
        }
    }
}

size_t field_type::count()
{
    return all_field_types.size();
}

void field_types::load( JsonObject &jo, const std::string &src )
{
    all_field_types.load( jo, src );
}

void field_types::finalize_all()
{
    set_field_type_ids();
}

void field_types::check_consistency()
{
    all_field_types.check();
}

void field_types::reset()
{
    all_field_types.reset();
}

const std::vector<field_type> &field_types::get_all()
{
    return all_field_types.get_all();
}

field_type_id fd_null,
              fd_blood,
              fd_bile,
              fd_gibs_flesh,
              fd_gibs_veggy,
              fd_web,
              fd_slime,
              fd_acid,
              fd_sap,
              fd_sludge,
              fd_fire,
              fd_rubble,
              fd_smoke,
              fd_toxic_gas,
              fd_tear_gas,
              fd_nuke_gas,
              fd_gas_vent,
              fd_fire_vent,
              fd_flame_burst,
              fd_electricity,
              fd_fatigue,
              fd_push_items,
              fd_shock_vent,
              fd_acid_vent,
              fd_plasma,
              fd_laser,
              fd_spotlight,
              fd_dazzling,
              fd_blood_veggy,
              fd_blood_insect,
              fd_blood_invertebrate,
              fd_gibs_insect,
              fd_gibs_invertebrate,
              fd_cigsmoke,
              fd_weedsmoke,
              fd_cracksmoke,
              fd_methsmoke,
              fd_bees,
              fd_incendiary,
              fd_relax_gas,
              fd_fungal_haze,
              fd_cold_air1,
              fd_cold_air2,
              fd_cold_air3,
              fd_cold_air4,
              fd_hot_air1,
              fd_hot_air2,
              fd_hot_air3,
              fd_hot_air4,
              fd_fungicidal_gas,
              fd_smoke_vent
              ;

void field_types::set_field_type_ids()
{
    fd_null = field_type_id( "fd_null" );
    fd_blood = field_type_id( "fd_blood" );
    fd_bile = field_type_id( "fd_bile" );
    fd_gibs_flesh = field_type_id( "fd_gibs_flesh" );
    fd_gibs_veggy = field_type_id( "fd_gibs_veggy" );
    fd_web = field_type_id( "fd_web" );
    fd_slime = field_type_id( "fd_slime" );
    fd_acid = field_type_id( "fd_acid" );
    fd_sap = field_type_id( "fd_sap" );
    fd_sludge = field_type_id( "fd_sludge" );
    fd_fire = field_type_id( "fd_fire" );
    fd_rubble = field_type_id( "fd_rubble" );
    fd_smoke = field_type_id( "fd_smoke" );
    fd_toxic_gas = field_type_id( "fd_toxic_gas" );
    fd_tear_gas = field_type_id( "fd_tear_gas" );
    fd_nuke_gas = field_type_id( "fd_nuke_gas" );
    fd_gas_vent = field_type_id( "fd_gas_vent" );
    fd_fire_vent = field_type_id( "fd_fire_vent" );
    fd_flame_burst = field_type_id( "fd_flame_burst" );
    fd_electricity = field_type_id( "fd_electricity" );
    fd_fatigue = field_type_id( "fd_fatigue" );
    fd_push_items = field_type_id( "fd_push_items" );
    fd_shock_vent = field_type_id( "fd_shock_vent" );
    fd_acid_vent = field_type_id( "fd_acid_vent" );
    fd_plasma = field_type_id( "fd_plasma" );
    fd_laser = field_type_id( "fd_laser" );
    fd_spotlight = field_type_id( "fd_spotlight" );
    fd_dazzling = field_type_id( "fd_dazzling" );
    fd_blood_veggy = field_type_id( "fd_blood_veggy" );
    fd_blood_insect = field_type_id( "fd_blood_insect" );
    fd_blood_invertebrate = field_type_id( "fd_blood_invertebrate" );
    fd_gibs_insect = field_type_id( "fd_gibs_insect" );
    fd_gibs_invertebrate = field_type_id( "fd_gibs_invertebrate" );
    fd_cigsmoke = field_type_id( "fd_cigsmoke" );
    fd_weedsmoke = field_type_id( "fd_weedsmoke" );
    fd_cracksmoke = field_type_id( "fd_cracksmoke" );
    fd_methsmoke = field_type_id( "fd_methsmoke" );
    fd_bees = field_type_id( "fd_bees" );
    fd_incendiary = field_type_id( "fd_incendiary" );
    fd_relax_gas = field_type_id( "fd_relax_gas" );
    fd_fungal_haze = field_type_id( "fd_fungal_haze" );
    fd_cold_air1 = field_type_id( "fd_cold_air1" );
    fd_cold_air2 = field_type_id( "fd_cold_air2" );
    fd_cold_air3 = field_type_id( "fd_cold_air3" );
    fd_cold_air4 = field_type_id( "fd_cold_air4" );
    fd_hot_air1 = field_type_id( "fd_hot_air1" );
    fd_hot_air2 = field_type_id( "fd_hot_air2" );
    fd_hot_air3 = field_type_id( "fd_hot_air3" );
    fd_hot_air4 = field_type_id( "fd_hot_air4" );
    fd_fungicidal_gas = field_type_id( "fd_fungicidal_gas" );
    fd_smoke_vent = field_type_id( "fd_smoke_vent" );
}

field_type field_types::get_field_type_by_legacy_enum( int legacy_enum_id )
{
    for( const auto &ft : all_field_types.get_all() ) {
        if( legacy_enum_id == ft.legacy_enum_id ) {
            return ft;
        }
    }
    debugmsg( "Cannot find field_type for legacy enum: %d.", legacy_enum_id );
    return field_type();
}

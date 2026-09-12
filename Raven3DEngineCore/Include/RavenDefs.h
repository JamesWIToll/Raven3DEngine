
#ifndef RAVEN_DEFS
#define RAVEN_DEFS

#define BITSHIFT_TO_POS(x) (1 << x)

#define RAVEN_BYTE std::uint8_t
#define RAVEN_SHORT std::int16_t
#define RAVEN_INT std::int32_t
#define RAVEN_LONG std::int64_t
#define RAVEN_FLOAT std::float_t
#define RAVEN_DOUBLE std::float64_t
#define RAVEN_U_SHORT std::uint16_t
#define RAVEN_U_INT std::uint32_t
#define RAVEN_U_LONG std::uint64_t

#define RAVEN_U_INT_MAX 0xFFFFFFFF

#define RAVEN_ENTITY_TYPE RAVEN_U_INT
#define RAVEN_ENTITY_NULL RAVEN_U_INT_MAX

#ifndef RAVEN_RESOURCE_PATH
#define RAVEN_RESOURCE_PATH "../Resources/"
#endif //RAVEN_RESOURCE_PATH

#include <pch.h>


namespace Raven3DEngineCore::Scene {
    enum class Entity_T : RAVEN_ENTITY_TYPE {};
} 


#endif //RAVEN_DEFS

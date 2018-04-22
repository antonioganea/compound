#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#include <cstdint>

#define S_REQUEST_SERVER_EVENTS static_cast<std::uint16_t>(1)
#define C_SEND_SERVER_EVENTS static_cast<std::uint16_t>(1)
#define S_REGISTER_CLIENT_EVENT static_cast<std::uint16_t>(2)
#define C_CLIENT_EVENT_ACKNOWLEDGEMENT static_cast<std::uint16_t>(2)

#define C_RUN_CLIENT_SCRIPTS static_cast<std::uint16_t>(6)

#define S_REQUEST_REGISTER_OBJECT static_cast<std::uint16_t>(7)
#define C_REGISTRATION_CODE static_cast<std::uint16_t>(7)

#define SHARED_POSITION static_cast<std::uint16_t>(8)
#define SHARED_VELOCITY static_cast<std::uint16_t>(9)
#define SHARED_FRICTION static_cast<std::uint16_t>(10)
#define SHARED_ROTATION static_cast<std::uint16_t>(11)
#define SHARED_TEXTUREID static_cast<std::uint16_t>(12)
#define SHARED_KILL static_cast<std::uint16_t>(13)

#define C_SINGLE_OBJECT static_cast<std::uint16_t>(14)

#endif // EVENTS_H_INCLUDED

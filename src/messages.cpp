/*
 *  Copyright (C) 2012 Josh Bialkowski (jbialk@mit.edu)
 *
 *  This file is part of openbook.
 *
 *  openbook is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  openbook is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with openbook.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 *  @file   src/messages.cpp
 *
 *  @date   Feb 11, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */

#include "messages.h"
#include "ExceptionStream.h"

namespace   openbook {
namespace filesystem {

MessageId parseMessageId( char byte )
{
    if( 0 < byte && byte < NUM_MSG )
        return (MessageId)(byte);
    else
    {
        ex()() << "Invalid message type: " << (int)byte;
        return MSG_INVALID;
    }
}

const char* g_msgIdStr[] =
{
    "QUIT",
    "PING",
    "PONG",
    "SET_DISPLAY_NAME",
    "SET_DATA_DIR",
    "SET_LOCAL_SOCKET",
    "SET_REMOTE_SOCKET",
    "SET_CLIENT_SOCKET",
    "SET_MAX_CONN",
    "LOAD_CONFIG",
    "SAVE_CONFIG",
    "ATTEMPT_CONNECT",
    "LEADER_ELECT",
    "DH_PARAMS",
    "KEY_EXCHANGE",
    "CEK",
    "AUTH_REQ",
    "AUTH_CHALLENGE",
    "AUTH_SOLN",
    "AUTH_RESULT",
    "SUBSCRIBE",
    "UNSUBSCRIBE",
    "NEW_VERSION",
    "REQUEST_FILE",
    "FILE_INFO",
    "FILE_CHUNK",
    "DIR_INFO",
    "DIR_CHUNK",
    "INVALID",
};



const char* messageIdToString( MessageId id )
{
    if( 0 < id && id < NUM_MSG )
        return g_msgIdStr[id];
    else
        return g_msgIdStr[NUM_MSG];
}



} // namespace filesystem
} // namespace openbook


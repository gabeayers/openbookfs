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
 *  @file   msg_gen/MessageMap.h
 *
 *  @date   Apr 16, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  Generated by message_map.pl
 */

#ifndef OPENBOOK_FS_MSG_GEN_MESSAGEMAP_H
#define OPENBOOK_FS_MSG_GEN_MESSAGEMAP_H

#include "msg_gen/MessageId.h"
#include "messages.pb.h"

namespace   openbook {
namespace filesystem {
    
/// maps MessageId to the message type
template < MessageId ID > struct MessageType;

/// maps message type to MessageId
template < typename T > struct MessageTypeToId;

/// @cond MessageTypeTemplateInstantiations
#define MAP_MSG_TYPE(MID,TYPE)                      \
template <> struct MessageType<MSG_##MID>           \
    { typedef messages::TYPE type; };               \
template <> struct MessageTypeToId<messages::TYPE>  \
    { static const MessageId ID = MSG_##MID; };     \

MAP_MSG_TYPE(              QUIT, Quit)
MAP_MSG_TYPE(              PING, Ping)
MAP_MSG_TYPE(              PONG, Pong)
MAP_MSG_TYPE(  SET_DISPLAY_NAME, SetDisplayName)
MAP_MSG_TYPE(      SET_DATA_DIR, SetDataDir)
MAP_MSG_TYPE(  SET_LOCAL_SOCKET, SetLocalSocket)
MAP_MSG_TYPE( SET_REMOTE_SOCKET, SetRemoteSocket)
MAP_MSG_TYPE( SET_CLIENT_SOCKET, SetClientSocket)
MAP_MSG_TYPE(      SET_MAX_CONN, SetMaxConnections)
MAP_MSG_TYPE(       LOAD_CONFIG, LoadConfig)
MAP_MSG_TYPE(       SAVE_CONFIG, SaveConfig)
MAP_MSG_TYPE(   ATTEMPT_CONNECT, AttemptConnection)
MAP_MSG_TYPE(   ADD_MOUNT_POINT, AddMountPoint)
MAP_MSG_TYPE(REMOVE_MOUNT_POINT, RemoveMountPoint)
MAP_MSG_TYPE(          UI_REPLY, UserInterfaceReply)
MAP_MSG_TYPE(      LEADER_ELECT, LeaderElect)
MAP_MSG_TYPE(         DH_PARAMS, DiffieHellmanParams)
MAP_MSG_TYPE(      KEY_EXCHANGE, KeyExchange)
MAP_MSG_TYPE(               CEK, ContentKey)
MAP_MSG_TYPE(          AUTH_REQ, AuthRequest)
MAP_MSG_TYPE(    AUTH_CHALLENGE, AuthChallenge)
MAP_MSG_TYPE(         AUTH_SOLN, AuthSolution)
MAP_MSG_TYPE(       AUTH_RESULT, AuthResult)
MAP_MSG_TYPE(         SUBSCRIBE, Subscribe)
MAP_MSG_TYPE(       UNSUBSCRIBE, Unsubscribe)
MAP_MSG_TYPE(       NEW_VERSION, NewVersion)
MAP_MSG_TYPE(      REQUEST_FILE, RequestFile)
MAP_MSG_TYPE(         FILE_INFO, FileInfo)
MAP_MSG_TYPE(        FILE_CHUNK, FileChunk)
MAP_MSG_TYPE(          DIR_INFO, DirInfo)
MAP_MSG_TYPE(         DIR_CHUNK, DirChunk)
MAP_MSG_TYPE(           INVALID, Invalid)



/// @endcond MessageTypeTemplateInstantiations
    
} //< namespace filesystem 
} //< namespace openbook

#endif //< OPENBOOK_FS_MSG_GEN_MESSAGEMAP_H
        

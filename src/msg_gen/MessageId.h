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
 *  @file   msg_gen/MessageId.h
 *
 *  @date   May 12, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  Generated by message_map.pl
 */

#ifndef OPENBOOK_FS_MSG_GEN_MESSAGEID_H
#define OPENBOOK_FS_MSG_GEN_MESSAGEID_H

namespace   openbook {
namespace filesystem {
	
enum MessageId
{
    MSG_QUIT,
    MSG_PING,
    MSG_PONG,
    MSG_SET_DISPLAY_NAME,
    MSG_SET_DATA_DIR,
    MSG_SET_LOCAL_SOCKET,
    MSG_SET_REMOTE_SOCKET,
    MSG_SET_CLIENT_SOCKET,
    MSG_SET_MAX_CONN,
    MSG_LOAD_CONFIG,
    MSG_SAVE_CONFIG,
    MSG_ATTEMPT_CONNECT,
    MSG_ADD_MOUNT_POINT,
    MSG_REMOVE_MOUNT_POINT,
    MSG_UI_REPLY,
    MSG_GET_BACKEND_INFO,
    MSG_PEER_LIST,
    MSG_MOUNT_LIST,
    MSG_START_SYNC,
    MSG_LEADER_ELECT,
    MSG_DH_PARAMS,
    MSG_KEY_EXCHANGE,
    MSG_CEK,
    MSG_AUTH_REQ,
    MSG_AUTH_CHALLENGE,
    MSG_AUTH_SOLN,
    MSG_AUTH_RESULT,
    MSG_SUBSCRIBE,
    MSG_UNSUBSCRIBE,
    MSG_ID_MAP,
    MSG_NODE_INFO,
    MSG_SEND_TREE,
    MSG_SEND_FILE,
    MSG_NEW_VERSION,
    MSG_REQUEST_FILE,
    MSG_FILE_CHUNK,
    MSG_DIR_CHUNK,
    MSG_INVALID,
    NUM_MSG = MSG_INVALID,
};


	
} //< namespace filesystem 
} //< namespace openbook

#endif //< OPENBOOK_FS_MSG_GEN_MESSAGEID_H
        

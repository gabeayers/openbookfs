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
 *  @file   src/NotifyPipe.h
 *
 *  @date   Feb 13, 2013
 *  @author Josh Bialkowski (jbialk@mit.edu)
 *  @brief  
 */

#ifndef OPENBOOK_NOTIFYPIPE_H_
#define OPENBOOK_NOTIFYPIPE_H_

#include <cstdlib>
#include <unistd.h>

namespace   openbook {
namespace filesystem {

/// manages a pipe which can be used as a condition variable
class NotifyPipe
{
    private:
        int m_fd[2];

    public:
        /// opens pipe
        NotifyPipe();

        /// closes pipe
        ~NotifyPipe();

        /// return the file descriptor for the write end of the pipe
        int writeFd();

        /// return the file descriptor for the read end of the pipe
        int readFd();

        /// write to the pipe to signal all listeners
        void notify();

        /// read from the pipe until it's empty
        void clear();
};



} // namespace filesystem
} // namespace openbook




#endif // NOTIFYPIPE_H_

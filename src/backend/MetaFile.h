/**
 *  @file   /openbook_fs/[Source directory]/src/backend/MetaFile.h
 *
 *  @date   Apr 19, 2013
 *  @author Josh Bialkowski <jbialk@mit.edu>
 *  @brief  
 */

#ifndef METAFILE_H_
#define METAFILE_H_

#include <sys/types.h>
#include <boost/filesystem.hpp>
#include <soci/soci.h>

#include "fuse_include.h"
#include "messages.pb.h"
#include "VersionVector.h"

namespace   openbook {
namespace filesystem {

/// wraps access to the sqlite database for a file and provides methods
/// which implement SQL queries
class MetaFile
{
    public:
        typedef boost::filesystem::path Path_t;
        typedef soci::session           Session_t;

    private:
        Session_t   m_sql;
        std::string m_subpath;

    public:
        /// open a soci session to the sqlite meta file for the specified
        /// path
        MetaFile( const Path_t& path );

        /// closes the sqlite metadata file
        ~MetaFile();

        /// initialize the metadata by creating appropriate tables
        void init();

        /// add an entry to the file list for
        void mknod( const std::string& path );//, mode_t type, mode_t mode );

        /// remove an entry from the file list
        void unlink( const std::string& path );

        /// read directory entries into a fuse buffer
        void readdir( void *buf, fuse_fill_dir_t filler, off_t offset );

        /// read directory entries into a message
        void readdir( messages::DirChunk* msg );

        /// merge entries from another peer
        void merge( messages::DirChunk* msg );

        /// increase the version vector for entry 0 (this)
        void incrementVersion();

        /// increase the version for a child of the directory
        void incrementVersion( const std::string& path );

        /// get the version for a path
        void getVersion( const std::string& path, VersionVector& v );

        /// get the version for a path
        void setVersion( const std::string& path, const VersionVector& v );

        /// assimilate keys for the specified path
        void assimilateKeys( const std::string& path, const VersionVector& v);


};


} //< namespace filesystem
} //< namespace openbook




#endif /* METAFILE_H_ */

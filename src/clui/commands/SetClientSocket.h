#ifndef OPENBOOK_FS_CLUI_SETCLIENTSOCKET_H_
#define OPENBOOK_FS_CLUI_SETCLIENTSOCKET_H_

#include "Options.h"

namespace   openbook {
namespace filesystem {
namespace       clui {

class SetClientSocket:
	public Options
{
	TCLAP::UnlabeledValueArg<int> addressFamily;
	TCLAP::UnlabeledValueArg<std::string> node;
	
	public:
        static const std::string COMMAND;
        static const std::string DESCRIPTION;

        SetClientSocket(TCLAP::CmdLine& cmd);
        void go();
};


}
}
}

#endif

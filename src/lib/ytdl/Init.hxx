#ifndef MPD_LIB_YTDL_INIT_HXX
#define MPD_LIB_YTDL_INIT_HXX

#include "input/InputStream.hxx"
#include "util/Domain.hxx"
#include <string>

class EventLoop;
class ConfigBlock;

extern const class Domain ytdl_domain;

namespace Ytdl {

class YtdlParams {
	std::string cmd_name;
	std::string config_file;

	public:
	YtdlParams() = default;
	YtdlParams(std::string _cmd_name, std::string _config_file)
		: cmd_name(_cmd_name), config_file(_config_file) {}

	void ReadConfigBlock(const ConfigBlock &block);

	const char *GetCommandName() const {
		return cmd_name.c_str();
	}

	const char *GetConfigFile() const {
		return config_file.empty() ? nullptr : config_file.c_str();
	}
};

void Init(const ConfigBlock &block);

const char *UriSupported(const char *uri);
bool WhitelistMatch(const char *uri);
const YtdlParams &GetParams();

} // namespace Ytdl

#endif

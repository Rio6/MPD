#include "config.h"
#include "Init.hxx"
#include "config/Block.hxx"
#include "util/StringView.hxx"
#include "util/StringCompare.hxx"
#include "util/IterableSplitString.hxx"
#include <forward_list>

const Domain ytdl_domain("youtube-dl");

static const char* DEFAULT_WHITELIST =
	"youtu.be "
	"music.youtube.com "
	"www.youtube.com";

static bool initialized = false;
static std::forward_list<std::string> domain_whitelist;
static Ytdl::YtdlParams ytdl_params;

namespace Ytdl {

void
YtdlParams::ReadConfigBlock(const ConfigBlock &block) {
	cmd_name = block.GetBlockValue("ytdl_command", "youtube-dl");
	config_file = block.GetBlockValue("config_file", "");
}

const char *
UriSupported(const char *uri)
{
	assert(initialized);

	const char* p;

	if ((p = StringAfterPrefix(uri, "ytdl://"))) {
		return p;
	} else if (WhitelistMatch(uri)) {
		return uri;
	} else {
		return nullptr;
	}
}

bool
WhitelistMatch(const char *uri)
{
	assert(initialized);

	const char* p;
	if (!(p = StringAfterPrefix(uri, "http://")) &&
		!(p = StringAfterPrefix(uri, "https://"))) {
		return false;
	}

	StringView domain(p);
	for (const auto &whitelist : domain_whitelist) {
		if (domain.StartsWith(whitelist.c_str())) {
			return true;
		}
	}

	return false;
}

void
Init(const ConfigBlock &block)
{
	const char* domains = block.GetBlockValue("domain_whitelist", DEFAULT_WHITELIST);

	for (const auto domain : IterableSplitString(domains, ' ')) {
		if (!domain.empty()) {
			domain_whitelist.emplace_front(domain.ToString());
		}
	}

	ytdl_params.ReadConfigBlock(block);

	initialized = true;
}

const YtdlParams
&GetParams() {
	return ytdl_params;
}

} // namespace Ytdl

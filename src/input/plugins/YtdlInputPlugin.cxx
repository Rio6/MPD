#include "config.h"
#include "lib/ytdl/Parser.hxx"
#include "lib/ytdl/TagHandler.hxx"
#include "lib/ytdl/Init.hxx"
#include "util/StringAPI.hxx"
#include "util/UriUtil.hxx"
#include "tag/Tag.hxx"
#include "config/Block.hxx"
#include "YtdlInputPlugin.hxx"
#include "YtdlInputStream.hxx"
#include "YtdlTagScanner.hxx"
#include "CurlInputPlugin.hxx"
#include "../InputStream.hxx"
#include "../RemoteTagScanner.hxx"
#include "tag/Tag.hxx"
#include <vector>

static EventLoop *event_loop;

static void
input_ytdl_init(EventLoop &_event_loop, const ConfigBlock &block)
{
	event_loop = &_event_loop;
	Ytdl::Init(block);
}

static bool
input_ytdl_supports_uri(const char *uri) noexcept
{
	return Ytdl::UriSupported(uri) != nullptr;
}

static InputStreamPtr
input_ytdl_open(const char *uri, Mutex &mutex)
{
	uri = Ytdl::UriSupported(uri);
	if (uri) {
		return std::make_unique<YtdlInputStream>(uri, mutex, *event_loop);
	}

	return nullptr;
}

static std::set<std::string> input_ytdl_protocols() noexcept
{
	std::set<std::string> protocols;
	protocols.emplace("ytdl");
	return protocols;
}

static std::unique_ptr<RemoteTagScanner>
input_ytdl_scan_tags(const char* uri, RemoteTagHandler &handler)
{
	uri = Ytdl::UriSupported(uri);
	if (uri) {
		return std::make_unique<YtdlTagScanner>(*event_loop, uri, handler);
	}

	return nullptr;
}

const struct InputPlugin input_plugin_ytdl = {
	"youtube-dl",
	nullptr,
	input_ytdl_init,
	nullptr,
	input_ytdl_open,
	input_ytdl_protocols,
	input_ytdl_scan_tags,
	input_ytdl_supports_uri,
};

#include "http_header.h"

#include <fstream>

using namespace Illuminati;

http_header& http_header::operator<<(boost::asio::streambuf& buf) {
  std::istream is(&buf);
  std::string str;
  auto end = key_words.end();

  LOG("Request");

  while (is >> str) {
    auto it = key_words.find(str);
    if (it != end) {
      if ((*it).second(is)) break;
    }
  }
  LOG(std::endl);
  return *this;
}

http_header& http_header::operator>>(boost::asio::streambuf& buf) {
  std::ostream ssOut(&buf);
  std::ifstream file;
  static auto checkGetUrl = [](const auto& url) {
    if (url.size() > 3) {
      if (url[0] == '/' && url[1] == 'g' && url[2] == 'e' && url[3] == 't') {
        return true;
      }
    }
    return false;
  };

  static auto getLengthOfFile = [](auto& file_stream) {
    auto pos = file_stream.tellg();
    file_stream.seekg(0, std::ios_base::end);
    auto end = file_stream.tellg();
    file_stream.seekg(pos);
    return end;
  };

  static auto getFileNameFromUrl = [](auto url) {
    auto pos_sl = url.find_last_of('/');

    if (pos_sl != std::string::npos) {
      url.erase(url.begin(), url.begin() + pos_sl + 1);
    }
    return std::move(url);
  };

  static auto getFilePuthFromUrl = [](auto url, unsigned int count) {
    url.erase(0, count - 1);

    return std::move(url);
  };

  auto createResponse = [&ssOut, &url = url ](
      auto&& http_status, auto&& cntent_type, auto&& content_length,
      auto&& data, auto&& extra_options) {
    LOG("\nResponse");
    LOG("Url: " << url);
    LOG("HTTP/1.1 " << http_status);

    ssOut << "HTTP/1.1 " << http_status << std::endl;
    ssOut << "content-type: " << cntent_type << std::endl;
    ssOut << "content-length: " << content_length << std::endl;
    ssOut << extra_options;
    ssOut << std::endl;
    ssOut << data;
  };

  auto createResponse404 = [&createResponse]() {
    std::string html = "<html><body><h1>Error 404</h1></body></html>";
    createResponse("404 Not Found", "text/html", html.length(), html, "");
  };

  auto fileResponse = [&file, &url = url, &createResponse404, &createResponse ](
      auto&& file_puth, auto&& content_type) {
    file.open(file_puth, std::ios_base::in | std::ios_base::binary);
    if (file.is_open()) {
      auto content_length = getLengthOfFile(file);
      auto filename = getFileNameFromUrl(url);

      std::stringstream str;
      str << "Content-Disposition: attachment; filename=\"" << filename
          << "\"\n";
      createResponse("200 OK", content_type /*"application/octet-stream"*/,
                     content_length, file.rdbuf(), str.str());
      LOG("File name: " << filename);
      LOG("File puth: " << file_puth);
      file.close();
    } else {
      createResponse404();
    }
  };

  if (url == "/") {
    std::stringstream html;
    html << "<html><body><h1>Asynchronous server</h1><p>Based on the library "
            "boost :: asio.</p><p>Create request: "
         << host << "/get/file_name</p></body></html>";
    createResponse("200 OK", "text/html", html.str().length(), html.str(), "");

  } else if (url == "/favicon.ico") {
    fileResponse(getFilePuthFromUrl(url, sizeof("/")),
                 "image/vnd.microsoft.icon");
  } else {
    if (checkGetUrl(url)) {
      fileResponse(getFilePuthFromUrl(url, sizeof("/get/")),
                   "application/octet-stream");
    } else
      createResponse404();
  }

  LOG(std::endl);

  return *this;
}

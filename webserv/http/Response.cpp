#include "Response.hpp"


const std::string Response::_httpVersion = "1.1";

Response::Response(void)
    : _statusCode(0), _statusMsg("") /*, _contentLength(0)*/
{
}

Response::~Response()
{
}

void Response::setLocation(const std::string& str) {
  _location = str;
}

std::string Response::_makeLocation() {
  std::string ret = "Location: ";
  ret += _location + "\n";
  return ret;
}

std::string Response::makeResponse(const std::string& body) {
  std::string ret = "";
  ret += (_makeStatusLine() + "\n");
  ret += (_makeHeader() + "\n");
  ret += body;
  return ret;
}

void Response::setStatusCode(const int& n) {
  _statusCode = n;
}

void Response::setStatusMsg(const std::string& str) {
  _statusMsg = str;
}

void Response::setServerName(const std::string& str) {
  _serverName = str;
}

void Response::setContentType(std::string type)
{
	_contentType = type;
}

std::string Response::_makeServerInfo(void) {
  std::string ret = "Server: ";
  ret += _serverName + "\n";
  return ret;
}

std::string Response::_makeStatusLine() {
  std::stringstream ss;
  ss << "HTTP/" << _httpVersion + " " << _statusCode << " " << _statusMsg << "\n";
  std::string ret;
  std::getline(ss, ret);
  ret + "\n";
  return ret;
}

std::string Response::_makeHeader() {
  std::string ret;
	// ret += "Content-Type: image/jpeg\n";
  ret += ("Date: " + Formatter::getDate());
  ret += _makeServerInfo();
  ret += _makeLocation();
  return ret;
}

const std::string	Response::generateAutoindexPage(std::string const &path) const//./Youpi// 경로가 dir일 뗴만 실행.
{
	DIR				*dir = opendir(path.c_str());
	struct dirent	*dirInfo;
	struct stat		statBuf;
	std::string		addPrefix, fileName, lastModTime, fileSize, content;
	/* begin html */
	content = "<html>\n";
	content += "<head><title>autoindex</title></head>\n";
	content += "<body>\n";
	content += "<h1>Index of ";
	content += path.substr(13);
	content += "</h1><hr/>\n";

	/* create table */
	content += "<table width=\"100%\" border=\"0\">\n";
	content += "<tr>\n";
	content += "<th align=\"left\">Name</th>\n";
	content += "<th align=\"left\">Last modified</th>\n";
	content += "<th align=\"left\">size</th>\n";
	content += "</tr>\n";

	if (dir == NULL) {
		std::cout << "opendir failed" << std::endl;
		return std::string("");
	}

	/* create table content */
	while ((dirInfo = readdir(dir)) != NULL) {
		fileName = dirInfo->d_name;
		if (fileName == ".") {
			fileName.clear();
			continue;
		}

		/* get absolut path */
		addPrefix = path;
		addPrefix += "/";
		if (fileName != "..")
			addPrefix += fileName;
		else
			addPrefix = fileName;

		/* get file status */
		stat(addPrefix.c_str(), &statBuf);

		/* get file modify time */
		lastModTime = ctime(&statBuf.st_mtime);
		lastModTime.erase(lastModTime.end() - 1);

		/* get file size */
		fileSize = std::to_string(statBuf.st_size);

		/* begin of content */
		content += "<tr>\n";

		/* element 1: path access */
		content += "<td><a href=\"";
		content += fileName;
		if (S_ISDIR(statBuf.st_mode))
			content += "/";
		content += "\">";
		content += fileName;
		if (S_ISDIR(statBuf.st_mode))
			content += "/";		
		content += "</a></td>\n";

		/* element 2: modify time */
		content += "<td>";
		content += lastModTime;
		content += "</td>";

		/* element 3: file size */
		content += "<td>";
		if (S_ISDIR(statBuf.st_mode))
			content += "_";
		else
			content += fileSize;
		content += "</td>";

		/* end of content */
		content += "</tr>\n";

		addPrefix.clear();
		fileName.clear();
		lastModTime.clear();
		fileSize.clear();
	}

	closedir(dir);

	/* end of html */
	content += "</table>\n";
	content += "</body>\n";
	content += "</html>\n";

	// printf("%s\n", content.c_str());
	return content;
}

#include "SearchEngine.h"
#include "util.h"

#include <curl/curl.h>
#include <json/json.h>


namespace fastgo {



string SearchEngine::last_key_words;
vector<string> SearchEngine::last_path_ret;

static size_t ReceiveData(void *contents, size_t size, size_t nmemb, void *stream)
{
    std::string *str = (std::string *)stream;
    (*str).append((char *)contents, size * nmemb);
    return size * nmemb;
}

static int http_post(const string& url, const string& req_body, string& resp) {
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);  //设置问非0表示本次操作为post

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0); // 不显示接收头信息
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReceiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        elog << curl_easy_strerror(res);
        return 777;
    }
    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return http_code;
}

static int http_delete(const string& url, const string& req, string& resp) {
    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type:application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0); // 不显示接收头信息
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ReceiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&resp);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        elog << curl_easy_strerror(res);
        return 777;
    }
    long http_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return http_code;
}

static string get_url_prefix() {
    return "http://localhost:7700/indexes/" + util::get_cur_username() + "/";
}

bool SearchEngine::deleteAllPath() {
    string resp;
    int http_code = http_delete(get_url_prefix() + "documents", "", resp);
    ilog << "http_code:" << http_code << ",resp:" << resp;
    return true;
}


bool SearchEngine::setPathSortableAttributes() {
    Json::Value root(Json::arrayValue);
    root.append("length");
    string data = Json::FastWriter().write(root);
    ilog << data;

    string resp;
    int http_code = http_post(get_url_prefix() + "settings/sortable-attributes", data, resp);
    ilog << "http_code:" << http_code << ",resp:" << resp;

    return true;
}

bool SearchEngine::updatePath(const vector<string>& paths) {
    Json::Value root(Json::arrayValue);
    for (const string& path : paths) {
        Json::Value val;
        val["id"] = std::to_string(std::hash<std::string>{}(path));
        val["content"] = path;
        val["length"] = (Json::UInt64)path.size();
        root.append(val);
    }

    string data = Json::FastWriter().write(root);
    //ilog << data;

    string resp;
    int http_code = http_post(get_url_prefix() + "documents", data, resp);

    ilog << "http_code:" << http_code << ",resp:" << resp;

    return true;
}


vector<string> SearchEngine::searchPath(const string& key_words) {
    if (key_words == last_key_words) {
        return last_path_ret;
    }
    Json::Value root;
    root["q"] = key_words;

    Json::Value sort_array(Json::arrayValue);
    sort_array.append("length:asc");
    root["sort"] = sort_array;

    string data = Json::FastWriter().write(root);
    ilog << data;

    string resp;
    int http_code = http_post(get_url_prefix() + "search", data, resp);
    ilog << "http_code:" << http_code << ",resp:" << resp;

    root.clear();
    Json::Reader reader;
    if (!reader.parse(resp, root)) {
        elog << "parse resp fail";
        throw runtime_error("parse resp fail ");
    }
    Json::Value hits_array = root["hits"];
    vector<string> ret;
    if (!hits_array.isNull() && hits_array.isArray()) {
        int i = 0;
        for (auto iter = hits_array.begin(); iter != hits_array.end() && i < 3; ++iter) {
            ++i;
            ret.push_back((*iter)["content"].asString());
        }
    }
    for (string s : ret) {
        ilog << s;
    }
    last_key_words = key_words;
    last_path_ret = ret;
    return ret;
}



} // namespace fastgo end
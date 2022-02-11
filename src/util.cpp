#include "util.h"
#include <sys/stat.h>
#include <unistd.h>

namespace fastgo {
namespace util {


uint64_t get_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


string get_fmt_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm* tm = localtime(&tv.tv_sec);

    char fmt_time[128];
    sprintf(fmt_time, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 1900 + tm->tm_year,
    tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, (int)(tv.tv_usec / 1000));
    return fmt_time;
}

void set_thread_name(string name) {
    assert(name.size() < 16);
    pthread_setname_np(pthread_self(), name.c_str());
}

string get_thread_name() {
    char name[16] = {0};
    prctl(PR_GET_NAME, (unsigned long)name);
    return name;
}

string get_filename_from_path(const string& path) {
    size_t found = path.rfind('/');
    return found == string::npos ? path : path.substr(found + 1);
}


bool is_dir(const char* path) {
    struct stat s;
    int ret = stat(path, &s);
    if (ret == EACCES) {
        throw runtime_error("Permission denied for " + string(path));
    }
    if (ret == 0 && s.st_mode & S_IFDIR) {
        return true;
    }
    return false;
}

#define MAX_LEN 512
int dp[MAX_LEN][MAX_LEN];
int levenshtein_distance(const string& str1, const string& str2) {
    if (str1.size() >= MAX_LEN || str2.size() >= MAX_LEN) {
        throw runtime_error("the string is too long for calculate levenshtein distance");
    }
    int len1 = str1.size();
    int len2 = str2.size();
    for (int i = 0; i <= len1; ++i) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= len1; i++) {
		for (int j = 1; j <= len2; j++) {
			dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1);
			dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (str1[i - 1] != str2[j - 1]));
			//删除，插入，替换
		}
	}
    return dp[len1][len2];
}




} // namespace util end
} // namespace fastgo end

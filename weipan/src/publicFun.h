#ifndef PUBLICFUN_H
#define PUBLICFUN_H
#include <QString>
#include <QCryptographicHash>
#include <QUrl>
#include <QTime>
#include <time.h>

const int CORRECT_RESULT            =4000;//正确的返回结果

enum EnumTmpTokenIndex
{
    TMP_TOKEN_SECRET_INDEX=3,//返回的临时tokenSecret在reply中的游标
    TMP_TOKEN_INDEX=7       //返回的临时token在reply中的游标
};

enum EnumAuTokenIndex   //authToken
{
    AUTHOR_TOKEN_SECRET_INDEX=3,
    AUTHOR_TOKEM_INDEX=7,
    AUTHOR_USR_ID=10,
    AUTHOR_USR_DIR=13
};

enum EnumUsrInfoIndex   //用户信息
{
    USR_ID=3,
    USR_TOTAL_SIZE_INDEX=13,
    USR_USED_SIZE_INDEX=17,
    USR_NAME_INDEX=27,
    USR_GENDER=43,
};


#define CLIENT_KEY                "3131777236"                           //client_key
#define CLIENT_SECRET             "dcf396d4b024a28fede10e432ba50f80"      //client_secret

#define AUTHORISE_SRC_URL         "https://auth.sina.com.cn/oauth2/authorize"
#define ACCESS_TOKEN_SRC_URL      "https://auth.sina.com.cn/oauth2/access_token"
#define GET_USR_INFO_URL          "https://api.weipan.cn/2/account/info"

#define GET_META_DATA_URL         "https://api.weipan.cn/2/metadata/sandbox/"
#define CREATE_FOLDER             "https://api.weipan.cn/2/fileops/create_folder"
#define UPLOAD_URL                "http://upload-vdisk.sina.com.cn/2/files_put/sandbox/"
#define DOWNLOAD_FILE_URL         "https://api.weipan.cn/2/files/sandbox/"

#define CLIENT_ID                 "client_id="
#define REDIRECT_URI              "redirect_uri=https://api.weibo.com/oauth2/default.html"
#define RESPONSE_TYPE             "response_type="
#define GRANT_TYPE                "grant_type="
#define ACCESS_TOKEN              "access_token="

#define DATA_FILE_LIMIT           "file_limit="
#define DATA_FILTER_EXT           "filter_ext="
#define DATA_LIST                 "list="
#define DATA_PAGE                 "page="
#define DATA_PAGE_SIZE            "page_size="
#define DATA_SORT_BY              "sort_by="

#define FOLDER_NAME               "path="
#define FOLDER_ROOT               "root="

#define TO_RECYLE                 "to_recycle="
#define DATA_FROM_PATH            "from_path="
#define DATA_TO_PATH              "to_path="

#define UPLOAD_OVER_WRITE         "overwrite="

#define BYTE_TO_MB_UNIT           (1024*1024UL)

QString removeUrlEncode(const QString &urlStr);

#endif // PUBLICFUN_H

#include <stdio.h>
#include <getopt.h>
#include <string.h>

#define TEMP_FILE_NAME "__temp_file__.bom"
#define LOG(f, ...) do{printf(f,##__VA_ARGS__);printf("\n");fflush(stdout);}while(0)


int hasBom(FILE* fp)
{
    fseek(fp, 0, SEEK_SET);
    char buf[4];
    if (3 == fread(buf, 1, 3, fp)) {
        if ('\xEF' == buf[0] && '\xBB' == buf[1] && '\xBF' == buf[2]) {
            fseek(fp, 0, SEEK_SET);
            return 0;
        }
    }
    fseek(fp, 0, SEEK_SET);
    return 1;
}

int addBom(char* src, char* dst)
{
    FILE* infp = fopen(src, "r");
    if (NULL == infp) {
        LOG("open file faied: %s", strerror(errno));
        return -1;
    }

    // 判断是否是utf8有bom文件
    if (0 == hasBom(infp)) {
        fclose(infp);
        return 0;
    }

    FILE *outfp = NULL;
    outfp = fopen(TEMP_FILE_NAME, "w+");
    if (NULL == outfp) {
        LOG("open file faied: %s", strerror(errno));
        fclose(infp);
        return -1;
    }


    char buf[1024*4];
    buf[0] = '\xEF';
    buf[1] = '\xBB';
    buf[2] = '\xBF';
    fwrite(buf, 3, 1, outfp);

    while (1) {
        size_t len = fread(buf, 1, 1024*4, infp);
        if (len > 0) {
            fwrite(buf, len, 1, outfp);
        } else if (0 == len) {
            break;
        } else {
            fclose(infp);
            fclose(outfp);
            remove(TEMP_FILE_NAME);
            return -1;
        }
    }

    fclose(infp);
    fclose(outfp);

    remove(dst);
    if (0 != rename(TEMP_FILE_NAME, dst)) {
        LOG("rename file failed: %s", strerror(errno));
        return -1;
    }

    return 0;
}

/**
 * @brief rmBom
 * @param src
 * @param dst
 * @return 如果src与dst相同，表示覆盖源文件，就需要创建一个中间文件作为中转
 */
int rmBom(char* src, char* dst)
{
    FILE* infp = fopen(src, "r");
    if (NULL == infp) {
        LOG("open file faied: %s", strerror(errno));
        return -1;
    }

    // 判断是否是utf8有bom文件
    if (0 != hasBom(infp)) {
        fclose(infp);
        return 0;
    }

    FILE *outfp = NULL;
    outfp = fopen(TEMP_FILE_NAME, "w+");
    if (NULL == outfp) {
        LOG("open file faied: %s", strerror(errno));
        fclose(infp);
        return -1;
    }

    fseek(infp, 3, SEEK_SET);

    char buf[1024*4];
    while (1) {
        size_t len = fread(buf, 1, 1024*4, infp);
        if (len > 0) {
            fwrite(buf, len, 1, outfp);
        } else if (0 == len) {
            break;
        } else {
            fclose(infp);
            fclose(outfp);
            remove(TEMP_FILE_NAME);
            return -1;
        }
    }

    fclose(infp);
    fclose(outfp);

    remove(dst);
    if (0 != rename(TEMP_FILE_NAME, dst)) {
        LOG("rename file[%s] -> [%s] failed: %s", TEMP_FILE_NAME, dst, strerror(errno));
        return -1;
    }

    return 0;
}

void printUsage(const char* app)
{
    printf("Usage: %s [options] <inputfile>\n"
           "  o     output file name\n"
           "  a     action, add or remove(rm)\n"
           "  h     help information\n",
           app);
}


static char g_input[256] = {0};
static char g_output[256] = {0};
static int  g_action = 0;

/**
 * UTF8 BOM 工具
 *      删除BOM
 *      添加BOM
 */
int main(int argc, char** argv)
{
    char opt;
    while ((opt = getopt(argc, argv, "o:a:h")) != -1) {
        switch (opt) {
            case 'o':
                sprintf(g_output, "%s", optarg);
                break;

            case 'a':
                if (0 == strcmp(optarg, "add")) {
                    g_action = 0;
                } else if (0 == strcmp(optarg, "remove") || 0 == strcmp(optarg, "rm")) {
                    g_action = 1;
                } else {
                    printUsage(argv[0]);
                    return -1;
                }
                break;

            case 'h':
                printUsage(argv[0]);
                return 0;

            default:
                return -1;
        }
    }

    if (optind >= argc) {
        printUsage(argv[0]);
        return -1;
    }

    // 第一个未解析的参数作为输入文件
    sprintf(g_input, "%s", argv[optind]);

    if (g_output[0] == '\0') {
        sprintf(g_output, "%s", g_input);
    }

    if (0 == g_action) { // add
        return addBom(g_input, g_output);
    } else {
        return rmBom(g_input, g_output);
    }
}

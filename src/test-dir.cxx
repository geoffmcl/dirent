/*\
 * test-dir.c
 *
 * Copyright (c) 2014 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h> // for strdup(), ...
#include <time.h>
#include <errno.h>
#include <tchar.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "dirent.h"

typedef std::vector<std::string> vSTG;

static const char *module = "test-dir";
static const char *usr_input = 0;
static bool recursive = false;
static bool utc_option = false;

static uint64_t total_files = 0;
static uint64_t total_dirs  = 0;
static uint64_t total_bytes = 0;

static _TCHAR szFullPath[MAX_PATH];

// utils

// TODO: not yet used to present nice numbers
void nice_num( _TCHAR * dst, _TCHAR * src ) // get nice number, with commas
{
   size_t i;
   size_t len = strlen(src);
   size_t rem = len % 3;
   size_t cnt = 0;
   for( i = 0; i < len; i++ )
   {
      if( rem ) {
         *dst++ = src[i];
         rem--;
         if( ( rem == 0 ) && ( (i + 1) < len ) )
            *dst++ = ',';
      } else {
         *dst++ = src[i];
         cnt++;
         if( ( cnt == 3 ) && ( (i + 1) < len ) ) {
            *dst++ = ',';
            cnt = 0;
         }
      }
   }
   *dst = 0;
}


#ifdef _MSC_VER
#define M_IS_DIR _S_IFDIR
#define PATH_SEP "\\"
#else // !_MSC_VER
#define M_IS_DIR S_IFDIR
#define PATH_SEP "/"
#endif

#define DT_NONE 0
#define DT_FILE 1
#define DT_DIR  2

static struct stat buf;
int is_file_or_directory32 ( const char * path )
{
    if (!path)
        return DT_NONE;
    int res = stat(path,&buf);
	if (res == 0) {
		if (buf.st_mode & M_IS_DIR)
			return DT_DIR;
		else
			return DT_FILE;
	} else {
      switch (errno) {
         case ENOENT:
           printf("Path %s not found.\n", path);
           break;
         case EINVAL:
           printf("Invalid parameter to _stat.\n");
           break;
         default:
           /* Should never be reached. */
           printf("Unexpected error in _stat.\n");
      }

    }
	return DT_NONE;
}
uint64_t get_last_file_size() { return buf.st_size; }

// 19/10/2014  19:25    <DIR>          Win32
// 19/10/2014  19:34            22,579 ZERO_CHECK.vcxproj
void output_stat()
{
   struct tm * ptm;
   ptm = utc_option ? gmtime(&buf.st_mtime) : localtime(&buf.st_mtime);
   printf("%04ld/%02d/%02d %02d:%02d ",
       ptm->tm_year + 1900L, ptm->tm_mon + 1, ptm->tm_mday,
	   ptm->tm_hour, ptm->tm_min );
    if (buf.st_mode & M_IS_DIR) {
        printf("       <DIR> ");
    } else {
        printf( "% 12ld ", buf.st_size );
    }
}

void output_stat2()
{
    char timebuf[26];
    errno_t err = ctime_s(timebuf, 26, &buf.st_mtime);
    if (err) {
        printf("Invalid arguments to ctime_s!\n");
        exit(1);
    }
    size_t len = strlen(timebuf);
    while (len) {
        len--;
        if (timebuf[len] > ' ')
            break;
        timebuf[len] = 0;
    }
    printf("%s ",timebuf);
    if (buf.st_mode & M_IS_DIR) {
        printf("       <DIR> ");
    } else {
        printf( "% 12ld ", buf.st_size );
    }
}


void give_help( char *name )
{
    printf("%s: usage: [options] usr_input\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(2)\n");
    printf(" --recursive   (-r) = Recursively enumerate the given directory.\n");
    printf(" Given a valid directory input, will enumerate the items found.\n");

}

int parse_args( int argc, char **argv )
{
    int i,i2,c;
    char *arg, *sarg;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        i2 = i + 1;
        if (*arg == '-') {
            sarg = &arg[1];
            while (*sarg == '-')
                sarg++;
            c = *sarg;
            switch (c) {
            case 'h':
            case '?':
                give_help(argv[0]);
                return 2;
            case 'r':
                recursive = true;
                break;
            default:
                printf("%s: Unknown argument '%s'. Tyr -? for help...\n", module, arg);
                return 1;
            }
        } else {
            // bear argument
            if (usr_input) {
                printf("%s: Already have input '%s'! What is this '%s'\n", module, usr_input, arg );
                return 1;
            }
            _tfullpath (szFullPath, arg, MAX_PATH);
            usr_input = strdup(szFullPath);

            if (is_file_or_directory32(usr_input) != DT_DIR) {
                printf("%s: Error: Unable to 'stat' '%s'! Aborting...\n", module, usr_input);
                return 1;
            }
        }
    }
    if (!usr_input) {
        printf("%s: No user input found in command!\n", module);
        return 1;
    }
    return 0;
}

void fix_directory( std::string &dir )
{
    size_t pos = dir.rfind(PATH_SEP);
    if (pos != dir.size())
        dir += PATH_SEP;
}

int enumerate_dir(const char *dir, int dep)
{
    int count = 0;
    DIR *dp = opendir(dir);
    if (!dp) {
        printf("%s: Failed to opendir(%s)\n", module, dir);
        return 1;
    }
    std::string s(dir);
    std::string ff;
    struct dirent *pe;
    int typ;
    vSTG vDirs;
    fix_directory(s);
    uint64_t files = 0;
    uint64_t dirs  = 0;
    uint64_t bytes = 0;
    while ((pe = readdir(dp)) != 0) {
        if (strcmp(pe->d_name,".") == 0)
            continue;
        if (strcmp(pe->d_name,"..") == 0)
            continue;

        ff = s;
        ff += pe->d_name;
        typ = dep;
        while(typ--) {
            // printf(" ");
        }

        typ = is_file_or_directory32( ff.c_str() );
        if ((typ == DT_DIR)||(typ == DT_FILE)) {
            if (count == 0) {
                printf("\n");
                printf("  Directory of %s\n", dir);
            }
            count++;
            output_stat();
            printf("%s\n", pe->d_name);
            if (typ == DT_DIR) {
                if (recursive) {
                    vDirs.push_back(ff);
                }
                dirs++;
                total_dirs++;
            } else {
                files++;
                bytes += get_last_file_size();
                total_files++;
                total_bytes += get_last_file_size();
            }
        } 
    }
    closedir(dp);
    // show stats for this directory
    printf("      % 14I64u  files, % 14I64u bytes\n", files, bytes);
    printf("      % 14I64u  dirs\n", dirs);

    size_t ii,max = vDirs.size();
    for (ii = 0; ii < max; ii++) {
        ff = vDirs[ii];
        typ = enumerate_dir( ff.c_str(), (dep+1) );
        if (typ)
            return typ;
    }

    if ((dep == 0) && recursive) {
        printf("\n");
        printf("     Total Files Listed:\n");
        printf("      % 14I64u  files, % 14I64u bytes\n", total_files, total_bytes);
        printf("      % 14I64u  dirs ", total_dirs);
        if (dir[1] == ':') {
            char rp[3];
            DWORD SectorsPerCluster, BytesPerSector, NumberOfFreeClusters, TotalNumberOfClusters;
            rp[0] = dir[0];
            rp[1] = ':';
            rp[2] = 0;
            if (GetDiskFreeSpace( rp, &SectorsPerCluster, &BytesPerSector, &NumberOfFreeClusters, &TotalNumberOfClusters )) {
                uint64_t tfree = SectorsPerCluster * BytesPerSector * NumberOfFreeClusters;
                printf(", % 14I64u free bytes\n", tfree);
            }
        }
        printf("\n");
    }

    return 0;

}

// main() OS entry
int main( int argc, char **argv )
{
    int iret = 0;
    iret = parse_args(argc,argv);
    if (iret)
        return iret;
    iret = enumerate_dir(usr_input,0);
    return iret;
}

// eof = test-dir.c

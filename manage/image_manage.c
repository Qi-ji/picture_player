#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include <config.h>
#include "image_manage.h"

int picture_scan(const char *path)
{
	DIR *dir = NULL;
	//struct stat *file_statbuf;    //ÕâÀïÎÒ¶¨ÒåÎªÖ¸Õë¾Í»á·¢Éú¶Î´íÎóª
									//ÕâÊÇÒòÎªÊ¹ÓÃlstat(cur_pathname, &file_statbuf)Ê±£¬Ã»ÓÐ¸øåfile_statbufÖ¸¶¨Î»ÖÃ£¬ËùÒÔÔÚÊäÈëÄÚÈÝÊ±ºò¾Í»á·¢Éú¶Î´íÎó
	struct stat file_statbuf;
	struct dirent *ptr = NULL;
	char cur_pathname[PATHNAMW_MAX_NUM];

	/*1.open the dir*/	
	if((dir = opendir(path)) == NULL)
	{
		fprintf(stderr, "wrong pathname\n");
		return -1;		
	}

	/*2.read the dir,and judge if it is a file or still a dir*/
	 while ((ptr=readdir(dir)) != NULL)
	 {		
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)//current dir OR parrent dir	
		    continue;

		/*2.1 fill the cur_pathname with file readed from dir*/
		memset(cur_pathname, '\0', sizeof(cur_pathname));
		strcpy(cur_pathname, path);
		strcat(cur_pathname, "/");
		strcat(cur_pathname, ptr->d_name);
		lstat(cur_pathname, &file_statbuf);

		/*2.2 */
		if (S_ISREG(file_statbuf.st_mode))
		{
			debug("is a file \n");
		}
		
		if (S_ISDIR(file_statbuf.st_mode))
		{
			debug("is a dir \n");
			picture_scan(cur_pathname); 		//recursion the new dir
		}
		
	 }

	closedir(dir);
	return 0;
}












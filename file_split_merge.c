#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * NameSqlit(char *filename){
	char *name;
	int n = strlen(filename);
	int i = 0;
	name = (char *)malloc(n-4);
	while(*(filename+i) != '.'){
			*(name+i) = *(filename+i);
			i++;
	}
	*(name+i) = '\0';
	return name;
}

int FileCutting(char *file_name,long n){
	FILE *fpr; /*读文件指针*/
	FILE *fpw; /*写文件指针*/
	FILE *fplist; /**/
	//char file_name[20];
	int file_id; /*新文件名id，001,002*/


	long offset; /*文件指针位置*/
	long fsize;  /*文件大小*/

	long fblock = 0; /* 1 M */
	char data;
	unsigned int size_r;

	//printf("请输入文件名\n");
	//scanf("%s",file_name);
	//printf("请输入切割大小(输入50则拆分50K一份)\n");
	//scanf("%d",&n);
	//printf("%s",NameSqlit(file_name));
	if(NameSqlit(file_name) == 0 || n == 0){
		printf("输入有误，程序退出\n");
		return 0;
	}

	fblock = 1024 * n;

	fpr = fopen(file_name,"rb");

	if(!fpr)
	{
		printf("文件打开失败！\n");
		return 0;
	}

	offset = 0;
	fseek(fpr,0,SEEK_END);
	fsize = ftell(fpr);
	fseek(fpr,0,SEEK_SET); /*返回文件头*/

	size_r = fread(&data,sizeof(char),1,fpr);

	file_id = 1;
	sprintf(file_name,"%s.%03d",NameSqlit(file_name),file_id); /* demo.001 */
	
	fpw = fopen(file_name,"wb"); /*创建新文件*/
	fplist = fopen("./data/filelist.txt","w");
	while(size_r>0)
	{
		/*写入新的文件*/
		fwrite(&data,sizeof(char),1,fpw);
		offset = ftell(fpr);
		if(offset % fblock == 0)
		{
			/*结束当前文件，建立新文件*/
			fclose(fpw);
			fputs(file_name,fplist);
			fputs("\n",fplist);
			file_id++;
			sprintf(file_name,"%s.%03d",NameSqlit(file_name),file_id); /* demo.00x */	
			fpw = fopen(file_name,"wb"); /*创建新文件*/
		}
		size_r = fread(&data,sizeof(char),1,fpr);
	}
	if(fpw)
	{
		fputs(file_name,fplist);
		fputs("\n",fplist);
		fclose(fpw);
	}
	fclose(fplist);
	fclose(fpr);


	printf("文件切割完成\n");
	return 0;
}


int FileMerge(char *filename){

	FILE *fpr;
	FILE *fpw;
	FILE *fplist;

	char file_name[20];
	//char filename[20];
	int file_id,i;
	
	char data;
	unsigned int size_r;
	char name[20];
	//printf("请输入要合成的文件(格式ceshi.txt):\n");
	//scanf("%s",filename);
	/*创建要写人文件*/
	fpw = fopen(filename,"wb");
	if(!fpw)
	{
		printf("文件创建失败！\n");
		return 0;
	}
	fplist = fopen("./data/filelist.txt","r");
	//fgets(file_name,20,fplist);

	while(fgets(file_name,20,fplist) != NULL)
	{	
		file_name[strlen(file_name)-1]='\0';
		fpr = fopen(file_name,"rb");
		if(!fpr)
		{
			printf("打开文件%s失败\n",file_name);
			return 0;
		}
		
		size_r = fread(&data,sizeof(char),1,fpr);
		while(size_r>0)
		{
			fwrite(&data,sizeof(char),1,fpw);
			size_r = fread(&data,sizeof(char),1,fpr);
		}
		fclose(fpr);		
		//fgets(file_name,20,fplist);

	}
	fclose(fpw);
	printf("文件合并完成\n");
	return 0;

}

void DispalyMenu(){
	/*printf("==============请选择功能==============\n");
	printf("=               1.切割               =\n");
	printf("=               2.合并               =\n");
	printf("=               3.退出               =\n");
	printf("======================================\n");*/
	printf("请输入正确的命令格式如下：\n");
	printf("main.exe -t s 分割文件 单个文件大小\n");
	printf("main.exe -t m 合并后的文件名称\n");
	return;
}


int main(int argc,char* argv[])
{

	/*char option;
	DispalyMenu();
	scanf("%c",&option);
	while(1){
		switch(option){
			case '1': FileCutting();break;
			case '2': FileMerge();break;
			case '3': exit(0);
			default: exit(0);
		}
		DispalyMenu();
		getchar();
		scanf("%c",&option);
	}
	return 0;*/
	if(strcmp("s",argv[2]) == 0){
		if(argc < 5){
			DispalyMenu();
			return 0;
		}
	   printf("正在切割文件请稍后...\n");
	   FileCutting(argv[3],strtol(argv[4],NULL,10));
	}else if(strcmp("m",argv[2]) == 0){
		if(argc < 4){
			DispalyMenu();
			return 0;
		}
		printf("正在合并文件请稍后...\n");
		FileMerge(argv[3]);
	}else{
		DispalyMenu();
		return 0;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Student{
    int no;
    int age;
    char sex;
    char name[20];
};

bool Check_Student(int stu_no, char filename[20]) {
    struct Student *st2;
    FILE *fp;
    fp = fopen(filename,"rb+");
    while (fread(st2, sizeof(struct Student),1,fp) != 0){
        if(st2->no == stu_no){
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

void Display_Menu(){
    printf("=================================================\n");
    printf("=               1.新增学生                        =\n");
    printf("=              2.查看全部学生                      =\n");
    printf("=               3.查找学生                        =\n");
    printf("=              4.重新加载文件                      =\n");
    printf("=                 5.退出                         =\n");
    printf("=================================================\n");
}

int Load_File(char filename[20]){
    FILE *fp;
    fp = fopen(filename,"ab+");
    if(fp == NULL){
        printf("打开失败\n");
    }else{
        printf("加载成功\n");
    }
    fclose(fp);
    return 0;
}

int Add_Student(char *filename){
    struct Student st;
    FILE *fp = fopen(filename,"ab+");
    printf("==新增学生界面==\n");
    printf("请输入学生信息\n");
    printf("学号:");
    scanf("%d",&st.no);
    printf("姓名:");
    scanf("%s",st.name);
    getchar();
    printf("性别（F:女，M:男）:");
    scanf("%c",&st.sex);
    printf("年龄:");
    scanf("%d",&st.age);

    if(Check_Student(st.no,filename)){
        fwrite(&st,sizeof(struct Student),1,fp);
        printf("成功\n");
        fclose(fp);
        return 0;
    }else{
        printf("该学生已经存在\n");
        fclose(fp);
        return 0;
    }

}

int Student_Information(char *filename){
    struct Student *st;
    FILE *fp = fopen(filename,"rb+");
    rewind(fp);
    while (fread(st, sizeof(struct Student),1,fp) != 0){
        printf("%s-%d-%d-%s\n", st->name, st->no, st->age, st->sex == 'F' ? "女" : "男");
    }
    fclose(fp);
    return 0;
}

int Lookup_Student(char *filename){
    int szNo;
    struct Student *st;
    printf("请输入要查找的学生学号:");
    scanf("%d",&szNo);
    FILE *fp = fopen(filename,"rb+");
    while (fread(st, sizeof(struct Student),1,fp) != 0){
        if(st->no == szNo){
            printf("%s-%d-%d-%s\n", st->name, st->no, st->age, st->sex == 'F' ? "女" : "男");
        }
    }
    fclose(fp);
    return 0;
}

int main() {
    FILE *fp;
    char option;
    char szPath[20];
    printf("请输入需要加载的文件:");
    scanf("%s",&szPath);
    getchar();
    fp = fopen(szPath,"ab+");
    if(fp == NULL){
        printf("打开失败\n");
        exit(0);
    }
    Display_Menu();
    scanf("%c",&option);

    while(option != '5'){
        switch (option) {
            case '1':
                Add_Student(szPath);break;
            case '2':
                Student_Information(szPath);break;
            case '3':
                Lookup_Student(szPath);
                break;
            case '4':
                printf("请输入需要加载的文件:");
                scanf("%s",szPath);
                Load_File(szPath);
                break;
            case '5':
                fclose(fp);
                exit(0);
            default:
                exit(0);
        }
        Display_Menu();
        getchar();
        scanf("%c",&option);
    }
    fclose(fp);
    return 0;
}

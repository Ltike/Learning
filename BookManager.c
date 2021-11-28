#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct book
{
    char id[5];
    char name[128];
    char author[20];
    int quantity;
    int status;

    struct book *next;
};

struct student
{
    char number[10];
    char name[128];
    char id[5];
    char bookname[128];
    int status;

    struct student *next;
};

void input_book(struct book *head);
void output_book(struct book head);
int borrow_book(struct book *head, struct student *st);
void output_student(struct student st);
void return_book(struct book *head, struct student *st);
void del_book(struct book *head, struct student st);
void load_studentfile(char *filename,struct student *st);
void load_bookfile(char *filename,struct book *head);

int main(int argc,char* argv[])
{
    struct book head;
    struct student st;
    int ncmd;
    char ch;
    /*初始化头结点*/
    head.next = NULL;
    st.next = NULL;
	load_studentfile("student.bin",&st);
	load_bookfile("book.bin",&head);
    printf("----------------------------\n");
    printf("1. 录入图书\n");
    printf("2. 图书列表显示\n");
    printf("3. 借书\n");
    printf("4. 还书\n");
    printf("5. 借书列表显示\n");
    printf("6. 删除书籍\n");
    printf("7. 退出\n");
    printf("----------------------------\n");
    scanf("%d",&ncmd);
    while(ncmd != 7){
        if(ncmd == 1)
        {
            input_book(&head);
            printf("是否继续录入(Y/N)?");
            ch = getchar();
            while(ch == 'Y')
            {
                input_book(&head);
                printf("是否继续录入(Y/N)?");
                ch = getchar();
            }
        }else if(ncmd == 2){
            output_book(head);
        }else if(ncmd == 3){
            borrow_book(&head,&st);
            output_student(st);
        }else if(ncmd == 4){
            output_student(st);
            return_book(&head,&st);
            output_student(st);
        }else if(ncmd == 5){
            output_student(st);
        }else if(ncmd == 6){
            output_book(head);
            del_book(&head,st);
            printf("----------------------------\n");
            output_book(head);
        }

		printf("----------------------------\n");
		printf("----------------------------\n");
        printf("选择功能\n");
        scanf("%d",&ncmd);
    }
    output_book(head);

    return 0;
}

void load_studentfile(char *filename,struct student *st){
	FILE *fp = fopen(filename,"rb+");
	struct student *new_st;
    new_st = (struct student*)malloc(sizeof(struct student));
	if(fp == NULL) {
		printf("加载文件失败\n");
		return;
	}
	while (fread(new_st, sizeof(struct student),1,fp) != 0){
		new_st->next = st->next;
		st->next = new_st;
		new_st = (struct student*)malloc(sizeof(struct student));
	}
	fclose(fp);
	return;
}

void load_bookfile(char *filename,struct book *head){
	FILE *fp = fopen(filename,"rb+");
	struct book *bk;
    bk = (struct book*)malloc(sizeof(struct book));
	if(fp == NULL) {
		printf("加载文件失败\n");
		return;
	}
	while (fread(bk, sizeof(struct book),1,fp) != 0){
		bk->next = head->next;
		head->next = bk;
		bk = (struct book*)malloc(sizeof(struct book));
	}
	fclose(fp);
	return;
}

void input_book(struct book *head)
{
    struct book *new_book;


    new_book = (struct book*)malloc(sizeof(struct book));
    printf("输入图书编号：");
    scanf("%s",new_book->id);
    printf("输入图书名称:");
    scanf("%s",new_book->name);
    printf("输入作者：");
    scanf("%s",new_book->author);
    printf("输入库存：");
    scanf("%d",&new_book->quantity);
    new_book->status = new_book->quantity;
    flushall();
    new_book->next = head->next;
    head->next = new_book;

}

void output_book(struct book head)
{
    struct book *p;
    FILE *pf;
    pf = fopen("book.bin","w");
    p = head.next;
    while(p!=NULL)
    {
        printf("编号:%10s\t 书名:%10s\t 作者:%10s\t 存入数量:%5d\t 剩余库存:%5d\t 状态:%3s\n",
               p->id,
               p->name,
               p->author,
               p->quantity,
               p->status,
               p->status == 0 ? "不可借" : "可借");
        fwrite(p,sizeof(struct book),1,pf);
        p = p->next;
    }
    fclose(pf);
}

int borrow_book(struct book *head, struct student *st){
    char id[10],name[128];
    struct book *p;
    int quantity;
    struct student *new_st;
    new_st = (struct student*)malloc(sizeof(struct student));
    p = head->next;
    printf("输入书编号:");
    scanf("%s",id);
    while(p != NULL){
        if(strcmp(p->id,id) == 0){
            if(p->status > 0){
                printf("此书库存量:%d,可借!\n",p->quantity);
                printf("输入借书学生学号:");
                scanf("%s",new_st->number);
                printf("请输入学生姓名:");
                scanf("%s",new_st->name);
                //状态赋值,借出未还
                new_st->status = 1;
                quantity = p->status;
                quantity = quantity - 1;
                p->status = quantity;
                strcpy(new_st->bookname,p->name);
                //new_st->bookname = name;
                strcpy(new_st->id,p->id);
                //new_st->id = id;
                head = p;
                new_st->next = st->next;
                st->next = new_st;
                return 0;
            }else{
                printf("此书无库存不能借出!\n");
                return 0;
            }
        }
        p = p->next;
        head = p;
    }
    printf("没有此书\n");

    return 0;

}

void return_book(struct book *head, struct student *st){
    char student_id[10],book_id[5];
    int book_number;
    struct book *p;
    struct student *new_st;
    new_st = st->next;
    p = head->next;
    printf("请输入学号:");
    scanf("%s",student_id);
    printf("请输入书籍id:");
    scanf("%s",book_id);
    while (new_st != NULL){
        if(strcmp(new_st->number,student_id) == 0){
            if(strcmp(new_st->id,book_id) == 0){
                if(new_st->status == 1){
                    book_number = p->status;
                    //book_number += 1;
                    p->status = book_number + 1;
                    new_st->status = 0;
                    printf("归还成功\n");
                    return;
                } else{
                    printf("此学生该书以归还,无需重复操作\n");
                    return;
                }
            }else{
                printf("该学生未借此书,无需归还\n");
                return;
            }
        }
        new_st = new_st->next;
        st = new_st;
        p = p->next;
        head = p;
    }
    printf("该学生未借书\n");
    return;
}

void output_student(struct student st)
{
    struct student *p;
    FILE *pf;
    pf = fopen("student.bin","w");
    p = st.next;
    while(p!=NULL)
    {
        printf("学号:%10s\t 名字:%10s\t 书籍编号:%10s\t 书籍名称:%10s\t 状态:%10s\n",
               p->number,
               p->name,
               p->id,
               p->bookname,
               p->status == 1 ? "未还" : "已还");
        fwrite(p,sizeof(struct student),1,pf);
        p = p->next;
    }
    fclose(pf);
}

void del_book(struct book *head, struct student st){
    char book_id[5];
    struct book *p,*p1;
    struct student *new_st;
    new_st = st.next;
    p = head;
    p1 = head->next;
    printf("请输入需要删除的书籍id:");
    scanf("%s",book_id);
    while (p1 != NULL){
        if(strcmp(p1->id,book_id) == 0){
            while(new_st != NULL){
                if(strcmp(new_st->id,book_id)==0 && new_st->status==1){
                    printf("该书籍还有未还，暂不能删除\n");
                    return;
                }
                new_st = new_st->next;
            }
			if(p1->next == NULL){
				p->next = NULL;
				free(p1);
				printf("删除书籍%s成功\n",book_id);
				return;
			}else{
				p->next = p1->next;
				free(p1);
				printf("删除书籍%s成功\n",book_id);
				return;
			}
        }
        p1 = p1->next;
        p = p->next;
    }

	printf("没有找到该书籍\n");
	return;
}

#pragma warning (disable:4996)
#pragma warning (disable:6031)

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<graphics.h>
#include<time.h>
#include<easyx.h>
#include<conio.h>
#define FILENAME "dict.dic"
#define MAX_NAMELENTH 55
#define MAX_NUMBER 10
bool shouldExit = false;

//图片
IMAGE img;
IMAGE img1;
IMAGE img2;
IMAGE img3;
IMAGE img4;
IMAGE img5;
IMAGE img6;
struct Foood
{
	char name[MAX_NAMELENTH];
	int num;
	int t;
	int cap;
	int duration;
	struct Foood* NextPtr;
	int Foood_number;
};
struct Combo
{
	char name[MAX_NAMELENTH];
	char Foood[MAX_NUMBER][MAX_NAMELENTH];
	int num;
};
struct Order
{
	int time;
	char need_Foood[MAX_NAMELENTH];
	struct Order* NextPtr;
	struct Foood* NeedFoood;
	int order_number;
	int is_combo;
	struct Foood* Foood;
};
struct Orderlist
{
	struct Order* HeadPtr;
	struct Order* LastPtr;
};
struct Button
{
	int x;
	int y;
	int w;
	int h;
	COLORREF Color;
	char* text;
};
struct Button* creatButton(int x, int y, int w, int h, const char* str, COLORREF Color);
struct Foood* making_Fooodlist(int N, FILE* fp);
struct Combo* making_combo(int M, FILE* fp);
struct Foood* making_Foood_need(struct Combo combo, struct Foood* Fooodlist, int N);
struct Order making_orderlist(struct Combo* combo, struct Foood* Foood, int M, int N, int number, struct Button* b);
struct Orderlist* making_unfinished_order(struct Order order, struct Orderlist* list);
struct Orderlist* _selling_Foood(struct Orderlist* order, struct Foood* Foood, int* unfinished_num, int second, int* finished_time);
void drawButton(struct Button* b);
void printing_time(FILE* fp, int second);
bool isClickButton(struct Button* b, ExMessage m);
void show_state(int* time, int order_number,int page);
void show_food(struct Foood* foodlist, int N);
int main()
{
	int page_number = 0;
	int N = 0, M = 0, W1 = 0, W2 = 0, n = 0, second = 25199, unfinished_num = 0, order_number = 0;
	int state = 1;
	int hour = second / 3600;
	int min = (second - 3600 * hour) / 60;
	int sec = second % 60;
	char timebuffer[9] = { 0 };
	int click_number = 0;
	int run_speed = 1;
	int in_second = 0;
	int in_second1 = 0;
	int in_second2 = 0;
	FILE* fp = fopen(FILENAME, "r");
	fscanf(fp, "%d %d", &N, &M);
	struct Foood* Foood = making_Fooodlist(N, fp);
	fscanf(fp, "%d %d", &W1, &W2);
	struct Combo* combo = making_combo(M, fp);
	fclose(fp);
	fp = fopen("output.txt", "w");



	snprintf(timebuffer, 9 * sizeof(char), "%02d:%02d:%02d", hour, min, sec);

	//创建翻页按钮
	struct Button* prevButton = creatButton(1150, 550, 80, 30, "上一页", YELLOW);
	struct Button* nextButton = creatButton(1240, 550, 80, 30, "下一页", YELLOW);
	struct Button* closeButton = creatButton(0, 850, 80, 30, "关闭", RED);
	int i1 = 0;
	int k1 = 0;

	//创建点餐按钮
	struct Button* btngroup[200] = { 0 };
	while (i1 < N)
	{
		for (int j = 0; j < 6; j++, i1++)
			btngroup[i1] = creatButton(130 + j * 200, 10 + 40 * k1, 200, 30, Foood[i1].name, YELLOW);
		k1++;
	}
	int i2 = 0;
	int k2 = 0;
	while (i2 < M)
	{
		for (int j = 0; j < 4; j++, i2++)
			btngroup[i2 + N] = creatButton(130 + j * 300, 175 + 40 * k2, 200, 30, combo[i2].name, YELLOW);
		k2++;
	}
	struct Button* speed = creatButton(0, 750, 90, 30, "×1", YELLOW);
	clock_t interval, start, finish;


	//画界面
	initgraph(1500, 900, 1);setbkcolor(WHITE);cleardevice();settextcolor(BLACK);setbkcolor(RGB(232, 187, 33));settextstyle(30, 0, "楷体");outtextxy(0, 800, timebuffer);
	setfillcolor(RGB(232, 187, 33));solidrectangle(0, 0, 120, 900);	settextcolor(BLACK);settextstyle(40, 0, "楷体");	outtextxy(10, 50, "食物");settextcolor(BLACK);
	settextstyle(40, 0, "楷体");outtextxy(10, 200, "套餐");	outtextxy(10, 400, "仓库");outtextxy(10, 600, "订单");
	//加载图片
	loadimage(&img, "./0.jpg", 170,170);
	putimage(1330, 12, &img);
	loadimage(&img1, "./1.jpg", 350, 350);
	putimage(1100, 220, &img1);
	loadimage(&img2, "./2.jpg", 90, 90);
	putimage(10, 90, &img2);
	loadimage(&img3, "./3.jpg", 90, 90);
	putimage(10, 250, &img3);
	loadimage(&img4, "./4.jpg", 90, 90);
	putimage(10, 440, &img4);
	loadimage(&img5, "./5.jpg", 90, 90);
	putimage(10, 640, &img5);
	loadimage(&img6, "./6.jpg", 450, 250);
	putimage(1100, 630, &img6);
	//画出按钮
	for (int i = 0; i < M+N; i++)
		drawButton(btngroup[i]);
	drawButton(speed);
	drawButton(prevButton);
	drawButton(nextButton);
	drawButton(closeButton);
	int finished_time[54001] = { 0 };
	//显示时间
	settextcolor(BLACK);
	settextstyle(30, 0, "楷体");
	outtextxy(0, 800, timebuffer);

	struct Orderlist* unfinished_orderlist = (struct Orderlist*)malloc(sizeof(struct Orderlist));
	struct Order* unfinished_order = (struct Order*)malloc(sizeof(struct Order));
	unfinished_order->NextPtr = NULL;
	struct Order* Last_OrderPtr = (struct Order*)malloc(sizeof(struct Order));
	unfinished_orderlist->HeadPtr = unfinished_order;
	unfinished_orderlist->LastPtr = Last_OrderPtr;
	struct Order* order = (struct Order*)malloc(sizeof(struct Order) * 54001);



	//初始化
	for (int i = 0; i < N; i++)
	{
		Foood[i].duration = 0;
		Foood[i].num = 0;
	}
	show_food(Foood, N);


	//开始营业
	while (second <= 79200&&!shouldExit)
	{
		second++;
		start = clock();
		in_second = 0;
		in_second1 = 0;
		in_second2 = 0;
		hour = second / 3600;
		min = (second - 3600 * hour) / 60;
		sec = second % 60;
		snprintf(timebuffer, 9 * sizeof(char), "%02d:%02d:%02d", hour, min, sec);
		settextcolor(BLACK);
		settextstyle(30, 0, "楷体");
		outtextxy(0, 800, timebuffer);
		if (second != 25200)
		{

			//制作食物
			for (int i = 0; i < N; i++)
				if (Foood[i].num < Foood[i].cap)
				{
					Foood[i].duration++;
					if ((Foood[i].duration % Foood[i].t == 0) && (Foood[i].t != 0))
						Foood[i].num++;
				}
		}
		show_food(Foood, N);
		//一秒内获取鼠标信息
		while (1)
		{
			ExMessage msg;
			peekmessage(&msg, EM_MOUSE);
			flushmessage();
			if (isClickButton(closeButton, msg)) {
				if (fp != NULL) {
					for (int i = 0; i < order_number; i++) {
						if (finished_time[i] == 1) {
							fprintf(fp, "Fail\n");
						}
						else
						{
							printing_time(fp, finished_time[i]);
						}
					}
					fclose(fp);
				}
				shouldExit = true; 
				break; 
			}
			//翻页按钮
			if (in_second1 == 0)

			{
				if (isClickButton(prevButton, msg))
				{
					if (page_number > 0)
						page_number--;
					in_second1 = 1;
				}
				if (isClickButton(nextButton, msg))
				{
					if (page_number < order_number / 50)
						page_number++;
					in_second1 = 1;
				}
			}
			//倍速按钮
				if (isClickButton(speed, msg))
				{
					BeginBatchDraw();
					settextcolor(BLACK);
					setbkcolor(RGB(255, 201, 14));
					settextstyle(20, 0, "楷体");
					click_number++;
					if (click_number % 3 == 0)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "×1");
						run_speed = 1;
					}
					else if (click_number % 3 == 1)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "×10");
						run_speed = 10;
					}
					else if (click_number % 3 == 2)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "×100");
						run_speed = 100;
					}
					flushmessage();
					FlushBatchDraw();
					EndBatchDraw();
				}
			finish = clock();
			interval = (finish - start);
			if (interval >= (1000 / run_speed))
				break;

			//处理订单信息
			if (in_second == 0)
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					in_second = 1;
					if (state == 1)
					{
						for (int i = 0; i < M+N; i++)
						{
							if (isClickButton(btngroup[i], msg))
							{
								unfinished_num++;
								order[order_number] = making_orderlist(combo, Foood, M, N, order_number, btngroup[i]);
								unfinished_orderlist = making_unfinished_order(order[order_number], unfinished_orderlist);
								order_number++;
								show_state(finished_time, order_number,page_number);
								break;
							}
						}
					}
					if (state == 0)
					{
						for (int i = 0; i < M+N; i++)
						{
							if (isClickButton(btngroup[i], msg))
							{
								finished_time[order_number++] = 1;
								show_state(finished_time, order_number,page_number);
								break;
							}
						}
					}
				}
			}
			flushmessage();
			show_state(finished_time, order_number,page_number);
			unfinished_orderlist = _selling_Foood(unfinished_orderlist, Foood, &unfinished_num, second, finished_time);
			show_food(Foood, N);

			//判断系统状态
			show_state(finished_time, order_number,page_number);
			if ((unfinished_num > W1) && (state == 1))
				state = 0;
			else if ((unfinished_num < W2) && (state == 0))
				state = 1;
			finish = clock();
			interval = (finish - start);
			//1秒过后就退出
			if (interval >= (1000 / run_speed))
				break;
		}
	}


	//加班
	while (unfinished_num)
	{
		second++;
		hour = second / 3600;
		min = (second - 3600 * hour) / 60;
		sec = second % 60;
		snprintf(timebuffer, 9 * sizeof(char), "%02d:%02d:%02d", hour, min, sec);
		settextcolor(BLACK);
		settextstyle(30, 0, "楷体");
		outtextxy(0, 800, timebuffer);
		for (int i = 0; i < N; i++)
			if (Foood[i].num < Foood[i].cap)
			{
				Foood[i].duration++;
				if (Foood[i].duration % Foood[i].t == 0)
					Foood[i].num++;
			}
		show_food(Foood, N);
		unfinished_orderlist = _selling_Foood(unfinished_orderlist, Foood, &unfinished_num, second, finished_time);
		show_state(finished_time, order_number,page_number);
		Sleep(1000/run_speed);
	}


	//文件输出
	for (int i = 0; i < n; i++)
	{
		if (finished_time[i] == 1)
			fprintf(fp, "Fail\n");
		else
			printing_time(fp, finished_time[i]);
	}

	free(Last_OrderPtr);
	Last_OrderPtr = NULL;
	free(unfinished_order);
	unfinished_order = NULL;
	free(Foood);
	Foood = NULL;
	free(combo);
	combo = NULL;
	closegraph();
	return 0;
}

void printing_time(FILE* fp, int second)
{
	int hour = second / 3600;
	int min = (second - 3600 * hour) / 60;
	int sec = second % 60;

	fprintf(fp, "%02d:%02d:%02d\n", hour, min, sec);
}
/**
* @brief 制作食物栏
*
* @param 食物种类数
* @param 文件指针
*
* @return 指向这个食物栏的指针
*/
struct Foood* making_Fooodlist(int N, FILE* fp)
{
	char** name_list = (char**)malloc(sizeof(char*) * N);
	int* t = (int*)malloc(sizeof(int) * N);
	int* cap = (int*)malloc(sizeof(int) * N);
	struct Foood* FooodPtr = (struct Foood*)malloc(sizeof(struct Foood) * N);

	for (int i = 0; i < N; i++)
		fscanf(fp, "%s", FooodPtr[i].name);
	for (int i = 0; i < N; i++)
		fscanf(fp, "%d", &FooodPtr[i].t);
	for (int i = 0; i < N; i++)
		fscanf(fp, "%d", &FooodPtr[i].cap);
	for (int i = 0; i < N; i++)
		FooodPtr[i].num = 0;
	for (int i = 0; i < N; i++)
		FooodPtr[i].duration = 0;

	return FooodPtr;
}
/**
*@brief 制作套餐栏
*
* @param 套餐种类数
* @param 文件指针
*
* @return 指向这个套餐栏的指针
*/
struct Combo* making_combo(int M, FILE* fp)
{
	struct Combo* comboPtr = NULL;
	comboPtr = (struct Combo*)malloc(sizeof(struct Combo) * M);
	for (int i = 0; i < M; i++)
	{
		fscanf(fp, "%s", comboPtr[i].name);
		char s = 0;
		int j = 0;
		for (j = 0; (s != '\n') && (s != EOF); j++)
		{
			fscanf(fp, "%s", comboPtr[i].Foood[j]);
			s = fgetc(fp);
		}
		comboPtr[i].num = j;
	}
	return comboPtr;
}
/**
*@brief 将时间转化为秒
*
* @param combo:套餐表
* @param Foood:食物表
* @param M:套餐数目
* @param N:食物数目
* @param number:订单数
* @param 输入时间表处理后的版本:
*所有时间是按xx:xx:xx的格式存在表格里的
* @return 指向这个时间数组的指针
*/
struct Order making_orderlist(struct Combo* combo, struct Foood* Foood, int M, int N, int number, struct Button* b)
{
	char Fooodkind[55] = { 0 };
	int is_Foood = 0;
	struct Order order;
	strcpy(Fooodkind, b->text);
	for (int i = 0; i < N; i++)
	{
		if (strcmp(Foood[i].name, Fooodkind) == 0)
		{
			order.is_combo = 0;
			order.Foood = &Foood[i];
			is_Foood = 1;
			break;
		}
	}
	order.order_number = number;//导入订单序号
	if (is_Foood)
		return order;
	for (int j = 0; j < M; j++)
	{
		if (strcmp(combo[j].name, Fooodkind) == 0)
		{
			order.is_combo = 1;
			order.NeedFoood = making_Foood_need(combo[j], Foood, N);//为点套餐的人制作食物链表
			break;
		}
	}
	return order;
}
/**
*@brief 制作订单表格:
* 将时间转化成秒数存入,订单转化为字符型数组存入
*
* @param combo:订单数
* @param Fooodlist:食物列表
* @param N:食物数目
* @return 指向订单表格的指针
*/
struct Foood* making_Foood_need(struct Combo combo, struct Foood* Fooodlist, int N)
{
	struct Foood* currentPtr = NULL, * HeadPtr = NULL, * LastPtr = NULL;
	HeadPtr = (struct Foood*)malloc(sizeof(struct Foood));
	LastPtr = (struct Foood*)malloc(sizeof(struct Foood));
	HeadPtr->NextPtr = NULL;
	int i = 0;
	while (i < combo.num)
	{
		currentPtr = (struct Foood*)malloc(sizeof(struct Foood));
		for (int j = 0; j < N; j++)
		{
			if (strcmp(combo.Foood[i], Fooodlist[j].name) == 0)
			{
				strcpy(currentPtr->name, Fooodlist[j].name);
				currentPtr->Foood_number = j;
				break;
			}
		}
		if (HeadPtr->NextPtr == NULL)
		{
			HeadPtr->NextPtr = currentPtr;
			LastPtr = currentPtr;
		}
		else
		{
			LastPtr->NextPtr = currentPtr;
			LastPtr = currentPtr;
		}
		LastPtr->NextPtr = NULL;
		i++;
	}
	return HeadPtr;
}
/**
*@brief 制作未完成订单表格:
* 将未完成的订单存入
*
* @param order:新的订单,默认为未完成
* @param list:未完成订单表
* @return 指向订单表格的链表头指针
*/
struct Orderlist* making_unfinished_order(struct Order order, struct Orderlist* list)
{
	struct Order* currentPtr = (struct Order*)malloc(sizeof(struct Order));
	currentPtr->NeedFoood = order.NeedFoood;
	currentPtr->order_number = order.order_number;
	currentPtr->Foood = order.Foood;
	currentPtr->is_combo = order.is_combo;
	strcpy(currentPtr->need_Foood, order.need_Foood);
	if (list->HeadPtr->NextPtr == NULL)
	{
		list->HeadPtr->NextPtr = currentPtr;
		list->LastPtr = currentPtr;
	}
	else
	{
		list->LastPtr->NextPtr = currentPtr;
		list->LastPtr = currentPtr;
	}
	list->LastPtr->NextPtr = NULL;
	return list;
}
/**
*@brief 出售食物:
* 将完成的订单从未完成的表格中清除
*
* @param order:未完成订单指针
* @param Foood:食物链表
* @param num:未完成数目指针
* @param second:当下时间
* @param finished_time:完成时间数组
* @return 出售完之后的未完成的订单表格的指针
*/
struct Orderlist* _selling_Foood(struct Orderlist* order, struct Foood* Foood, int* num, int second, int* finished_time)
{
	struct Order* currentPtr = order->HeadPtr->NextPtr, * Previous_Ptr = order->HeadPtr;
	while (currentPtr != NULL)
	{
		int number = currentPtr->order_number;
		int find1 = 0;
		if (currentPtr->is_combo == 0)
		{
			if (currentPtr->Foood->num)
			{
				Previous_Ptr->NextPtr = currentPtr->NextPtr;
				(currentPtr->Foood->num)--;
				(*num)--;
				find1 = 1;
				finished_time[number] = second;
			}
		}
		else
		{
			struct Foood* Foood_currentPtr = currentPtr->NeedFoood->NextPtr, * Foood_Previous_Ptr = currentPtr->NeedFoood;
			while (Foood_currentPtr)//对套餐中的食物链表进行售卖
			{
				int flag = 0;
				if (Foood[Foood_currentPtr->Foood_number].num)
				{
					flag = 1;
					Foood[Foood_currentPtr->Foood_number].num--;
					Foood_Previous_Ptr->NextPtr = Foood_currentPtr->NextPtr;
				}
				if (flag == 0)
					Foood_Previous_Ptr = Foood_Previous_Ptr->NextPtr;
				Foood_currentPtr = Foood_currentPtr->NextPtr;
			}
			if (currentPtr->NeedFoood->NextPtr == NULL)
			{
				Previous_Ptr->NextPtr = currentPtr->NextPtr;
				find1 = 1;
				(*num)--;
				finished_time[number] = second;
			}
		}
		if (find1 && (currentPtr->NextPtr == NULL))
		{
			order->LastPtr = Previous_Ptr;
			order->LastPtr->NextPtr = NULL;
		}
		//改变尾指针位置
		else if (!find1)
			Previous_Ptr = Previous_Ptr->NextPtr;
		currentPtr = currentPtr->NextPtr;
	}
	return order;
}
/**
*@brief 创建按钮:
* 制作按钮
*
* @param x:h横坐标
* @param y:纵坐标
* @param w:宽度
* @param h:高度
* @param  字符
* @param 颜色
* @return 
*/
struct Button* creatButton(int x, int y, int w, int h, const char* str, COLORREF Color)
{
	struct Button* b = (struct Button*)malloc(sizeof(struct Button));
	int textlen = strlen(str) + 1;
	b->x = x;
	b->y = y;
	b->w = w;
	b->h = h;
	b->Color = b->Color;
	b->text = (char*)malloc(sizeof(char) * textlen);
	strcpy(b->text, str);
	return b;
}
/**
*@brief 画按钮:
* 制作按钮
*
* @param 按钮
*/
void drawButton(struct Button* b)
{
	setfillcolor(RGB(255, 201, 14));
	fillroundrect(b->x, b->y, b->x + b->w, b->y + b->h, 5, b->h);
	settextcolor(BLACK);
	setbkcolor(RGB(255, 201, 14));
	settextstyle(20, 0, "楷体");
	int textw = textwidth(b->text);
	int texth = textheight(b->text);
	int xx = b->x + (b->w - textw) / 2;
	int yy = b->y + (b->h - texth) / 2;
	outtextxy(xx, yy, b->text);
}
/**
*@brief:判断是否点击
* 
* @param 按钮
* @param 互动信息
*/
bool isClickButton(struct Button* b, ExMessage m)
{
	if ((m.x > b->x) && (m.x < (b->x + b->w))
		&& (m.y > b->y) && (m.y < (b->y + b->h))
		&& m.message == WM_LBUTTONDOWN)
		return true;
	return false;
	if (strcmp(b->text, "关闭") == 0 && (m.x > b->x) && (m.x < (b->x + b->w))
		&& (m.y > b->y) && (m.y < (b->y + b->h)) && m.message == WM_LBUTTONDOWN)
	{
		shouldExit = true;
		return true;
	}
	return false;
}
/**
*@brief:展示订单状况
*
* @param 时间数组
* @param 订单数目
*  @param 页面数
*/
void show_state(int* time, int order_number,int page)
{
	char** state;
	state = (char**)malloc(sizeof(char*) * order_number);
	for (int i = 0; i < order_number; i++)
		state[i] = (char*)malloc(sizeof(char) * 100);
	for (int i = 0; i < order_number; i++)
	{
		snprintf(state[i], 5 * sizeof(char), "%d ", i + 1);
		if (time[i] == 0)
			strcat(state[i], "制作中...");
		else if (time[i] == 1)
			strcat(state[i], "Fail");
		else
		{
			int hour = time[i] / 3600;
			int min = (time[i] - 3600 * hour) / 60;
			int sec = time[i] % 60;
			char* ctime = (char*)malloc(sizeof(char) * 100);
			snprintf(ctime, 9 * sizeof(char), "%02d:%02d:%02d  ", hour, min, sec);
			strcat(state[i], ctime);
			free(ctime);
		}
	}
	BeginBatchDraw();
	settextcolor(BLACK);
	setbkcolor(WHITE);
	settextstyle(30, 0, "楷体");
	int count = 0;
	int i = page * 50;
	int j = 0;
	int k = 0;
	clearrectangle(200, 550, 1150, 880);
	while (i < order_number&&count<50)
	{
		for (int j = 0; i<order_number; j++, i++,count++)
		{
			outtextxy(200 * (k + 1), 550 + j * 30, state[i]);
			if (j == 9)
			{
				i++;
				break;
			}
		}
		if (count >= 50)
			break;
		if (k < 4)
			k++;
		else
		{
			j = 0;
			k = 0;
			break;
		}
	}
	FlushBatchDraw();
	EndBatchDraw();
}
/**
*@brief:展示食物状况
*
* @param 食物库
* @param 食物数目
*/
void show_food(struct Foood* foodlist, int N)
{
	int j = 0;
	for (int i = 0; i < N; i++)
	{
		char number[60] = { 0 };
		snprintf(number, sizeof(char) * 10, "%d/%d ", foodlist[i].num, foodlist[i].cap);
		strcat(number, foodlist[i].name);
		setbkcolor(WHITE);
		settextstyle(20, 0, "楷体");
		int texth = textheight(number);
		if (300 + i * texth < 500)
			outtextxy(200, 300 + i * texth, number);
		else
		{
			outtextxy(700, 300+ j * texth, number);
			j++;
		}
	}
}
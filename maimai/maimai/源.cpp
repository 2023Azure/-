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

//ͼƬ
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

	//������ҳ��ť
	struct Button* prevButton = creatButton(1150, 550, 80, 30, "��һҳ", YELLOW);
	struct Button* nextButton = creatButton(1240, 550, 80, 30, "��һҳ", YELLOW);
	struct Button* closeButton = creatButton(0, 850, 80, 30, "�ر�", RED);
	int i1 = 0;
	int k1 = 0;

	//������Ͱ�ť
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
	struct Button* speed = creatButton(0, 750, 90, 30, "��1", YELLOW);
	clock_t interval, start, finish;


	//������
	initgraph(1500, 900, 1);setbkcolor(WHITE);cleardevice();settextcolor(BLACK);setbkcolor(RGB(232, 187, 33));settextstyle(30, 0, "����");outtextxy(0, 800, timebuffer);
	setfillcolor(RGB(232, 187, 33));solidrectangle(0, 0, 120, 900);	settextcolor(BLACK);settextstyle(40, 0, "����");	outtextxy(10, 50, "ʳ��");settextcolor(BLACK);
	settextstyle(40, 0, "����");outtextxy(10, 200, "�ײ�");	outtextxy(10, 400, "�ֿ�");outtextxy(10, 600, "����");
	//����ͼƬ
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
	//������ť
	for (int i = 0; i < M+N; i++)
		drawButton(btngroup[i]);
	drawButton(speed);
	drawButton(prevButton);
	drawButton(nextButton);
	drawButton(closeButton);
	int finished_time[54001] = { 0 };
	//��ʾʱ��
	settextcolor(BLACK);
	settextstyle(30, 0, "����");
	outtextxy(0, 800, timebuffer);

	struct Orderlist* unfinished_orderlist = (struct Orderlist*)malloc(sizeof(struct Orderlist));
	struct Order* unfinished_order = (struct Order*)malloc(sizeof(struct Order));
	unfinished_order->NextPtr = NULL;
	struct Order* Last_OrderPtr = (struct Order*)malloc(sizeof(struct Order));
	unfinished_orderlist->HeadPtr = unfinished_order;
	unfinished_orderlist->LastPtr = Last_OrderPtr;
	struct Order* order = (struct Order*)malloc(sizeof(struct Order) * 54001);



	//��ʼ��
	for (int i = 0; i < N; i++)
	{
		Foood[i].duration = 0;
		Foood[i].num = 0;
	}
	show_food(Foood, N);


	//��ʼӪҵ
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
		settextstyle(30, 0, "����");
		outtextxy(0, 800, timebuffer);
		if (second != 25200)
		{

			//����ʳ��
			for (int i = 0; i < N; i++)
				if (Foood[i].num < Foood[i].cap)
				{
					Foood[i].duration++;
					if ((Foood[i].duration % Foood[i].t == 0) && (Foood[i].t != 0))
						Foood[i].num++;
				}
		}
		show_food(Foood, N);
		//һ���ڻ�ȡ�����Ϣ
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
			//��ҳ��ť
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
			//���ٰ�ť
				if (isClickButton(speed, msg))
				{
					BeginBatchDraw();
					settextcolor(BLACK);
					setbkcolor(RGB(255, 201, 14));
					settextstyle(20, 0, "����");
					click_number++;
					if (click_number % 3 == 0)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "��1");
						run_speed = 1;
					}
					else if (click_number % 3 == 1)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "��10");
						run_speed = 10;
					}
					else if (click_number % 3 == 2)
					{
						clearrectangle(0, 750, 90, 780);
						outtextxy(30, 755, "��100");
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

			//��������Ϣ
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

			//�ж�ϵͳ״̬
			show_state(finished_time, order_number,page_number);
			if ((unfinished_num > W1) && (state == 1))
				state = 0;
			else if ((unfinished_num < W2) && (state == 0))
				state = 1;
			finish = clock();
			interval = (finish - start);
			//1�������˳�
			if (interval >= (1000 / run_speed))
				break;
		}
	}


	//�Ӱ�
	while (unfinished_num)
	{
		second++;
		hour = second / 3600;
		min = (second - 3600 * hour) / 60;
		sec = second % 60;
		snprintf(timebuffer, 9 * sizeof(char), "%02d:%02d:%02d", hour, min, sec);
		settextcolor(BLACK);
		settextstyle(30, 0, "����");
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


	//�ļ����
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
* @brief ����ʳ����
*
* @param ʳ��������
* @param �ļ�ָ��
*
* @return ָ�����ʳ������ָ��
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
*@brief �����ײ���
*
* @param �ײ�������
* @param �ļ�ָ��
*
* @return ָ������ײ�����ָ��
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
*@brief ��ʱ��ת��Ϊ��
*
* @param combo:�ײͱ�
* @param Foood:ʳ���
* @param M:�ײ���Ŀ
* @param N:ʳ����Ŀ
* @param number:������
* @param ����ʱ������İ汾:
*����ʱ���ǰ�xx:xx:xx�ĸ�ʽ���ڱ�����
* @return ָ�����ʱ�������ָ��
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
	order.order_number = number;//���붩�����
	if (is_Foood)
		return order;
	for (int j = 0; j < M; j++)
	{
		if (strcmp(combo[j].name, Fooodkind) == 0)
		{
			order.is_combo = 1;
			order.NeedFoood = making_Foood_need(combo[j], Foood, N);//Ϊ���ײ͵�������ʳ������
			break;
		}
	}
	return order;
}
/**
*@brief �����������:
* ��ʱ��ת������������,����ת��Ϊ�ַ����������
*
* @param combo:������
* @param Fooodlist:ʳ���б�
* @param N:ʳ����Ŀ
* @return ָ�򶩵�����ָ��
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
*@brief ����δ��ɶ������:
* ��δ��ɵĶ�������
*
* @param order:�µĶ���,Ĭ��Ϊδ���
* @param list:δ��ɶ�����
* @return ָ�򶩵���������ͷָ��
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
*@brief ����ʳ��:
* ����ɵĶ�����δ��ɵı�������
*
* @param order:δ��ɶ���ָ��
* @param Foood:ʳ������
* @param num:δ�����Ŀָ��
* @param second:����ʱ��
* @param finished_time:���ʱ������
* @return ������֮���δ��ɵĶ�������ָ��
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
			while (Foood_currentPtr)//���ײ��е�ʳ�������������
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
		//�ı�βָ��λ��
		else if (!find1)
			Previous_Ptr = Previous_Ptr->NextPtr;
		currentPtr = currentPtr->NextPtr;
	}
	return order;
}
/**
*@brief ������ť:
* ������ť
*
* @param x:h������
* @param y:������
* @param w:���
* @param h:�߶�
* @param  �ַ�
* @param ��ɫ
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
*@brief ����ť:
* ������ť
*
* @param ��ť
*/
void drawButton(struct Button* b)
{
	setfillcolor(RGB(255, 201, 14));
	fillroundrect(b->x, b->y, b->x + b->w, b->y + b->h, 5, b->h);
	settextcolor(BLACK);
	setbkcolor(RGB(255, 201, 14));
	settextstyle(20, 0, "����");
	int textw = textwidth(b->text);
	int texth = textheight(b->text);
	int xx = b->x + (b->w - textw) / 2;
	int yy = b->y + (b->h - texth) / 2;
	outtextxy(xx, yy, b->text);
}
/**
*@brief:�ж��Ƿ���
* 
* @param ��ť
* @param ������Ϣ
*/
bool isClickButton(struct Button* b, ExMessage m)
{
	if ((m.x > b->x) && (m.x < (b->x + b->w))
		&& (m.y > b->y) && (m.y < (b->y + b->h))
		&& m.message == WM_LBUTTONDOWN)
		return true;
	return false;
	if (strcmp(b->text, "�ر�") == 0 && (m.x > b->x) && (m.x < (b->x + b->w))
		&& (m.y > b->y) && (m.y < (b->y + b->h)) && m.message == WM_LBUTTONDOWN)
	{
		shouldExit = true;
		return true;
	}
	return false;
}
/**
*@brief:չʾ����״��
*
* @param ʱ������
* @param ������Ŀ
*  @param ҳ����
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
			strcat(state[i], "������...");
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
	settextstyle(30, 0, "����");
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
*@brief:չʾʳ��״��
*
* @param ʳ���
* @param ʳ����Ŀ
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
		settextstyle(20, 0, "����");
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
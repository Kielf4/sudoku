#pragma warning(disable:4996)
#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<cstdlib>
#include<fstream>
#include<algorithm>
using namespace std;

int counts;
int p = 0;

int a[15] = { '1','2','3','4','5','6','7','9' };//生成所用 
char move13[10][5] = { "036", "063" };
char move36[10][5] = { "258", "285", "528", "582", "825", "852" };
char move69[10][5] = { "147", "174", "417", "471", "714", "741" };


char sudokuu[200000000];//求解所用 
int vis[3][10][10];
int res[9][9];
int suc;

class Sudo
{


public:
	void create(int num);
	void solve(char* file);
	void setvis(int m, int n, int num);
	void resetvis(int m, int n, int num);
	bool checkvis(int m, int n, int num);
	void TraceBack(int n);
};

void Sudo::create(int num)
{
	FILE* fout = fopen("sudoku.txt", "w");
	int p = 0;					//记录终局个数
	int flag = 1;
	int tt;
	tt = num / 72 + 1;
	for (int t = 0;t < tt;t++)
	{
		a[8] = '8';//学号为1120172770 
		next_permutation(a + 1, a + 8);
		for (int i = 0;i < 2 && flag;++i)
		{
			for (int j = 0;j < 6 && flag;++j)
			{
				for (int k = 0;k < 6 && flag;++k)
				{
					int move[10];
					int rows = 0;
					for (int ii = 0;ii < 3;ii++)
					{
						move[rows] = move13[i][ii] - '0';
						rows++;
					}
					for (int jj = 0;jj < 3;jj++)
					{
						move[rows] = move36[j][jj] - '0';
						rows++;
					}
					for (int kk = 0;kk < 3;kk++)
					{
						move[rows] = move69[k][kk] - '0';
						rows++;
					}
					for (int l = 0;l < 9;l++)
					{
						int m = move[l];
						fputc(a[(8 + m) % 9], fout);
						for (int n = 1;n < 17;n++)
						{
							fputc(' ', fout);
							fputc(a[((16 - n) / 2 + m) % 9], fout);
							++n;
						}
						fputc('\n', fout);
					}
					fputc('\n', fout);
					++p;
					if (p >= num)
					{
						flag = 0;
						break;
					}
				}
			}
		}
	}		//使用STL全排列函数
	fputc('\0', fout);
	fclose(fout);
}


void Sudo::setvis(int m, int n, int num)  //[m,n]有num，则vis对应为1 
{
	vis[0][m][num] = 1;
	vis[1][n][num] = 1;
	int nn;
	nn = m / 3 * 3 + n / 3;
	vis[2][nn][num] = 1;
}
void Sudo::resetvis(int m, int n, int num) //[m,n]无num,则vis对应为0 
{
	vis[0][m][num] = 0;
	vis[1][n][num] = 0;
	int nn;
	nn = m / 3 * 3 + n / 3;
	vis[2][nn][num] = 0;
}
bool Sudo::checkvis(int m, int n, int num)  //检查[m,n]中是否可放num 
{
	int nn;
	nn = m / 3 * 3 + n / 3;
	if (vis[0][m][num] == 0 && vis[1][n][num] == 0 && vis[2][nn][num] == 0)
		return true;
	else
		return false;
}

void Sudo::TraceBack(int n)
{
	if (suc == 1)
		return;
	if (n > 80)  //代表解完当前数独
	{
		int i, j;
		for (i = 0;i < 9;i++)
		{
			for (j = 0;j < 9;j++)
			{
				if (j != 8)
				{
					sudokuu[p++] = res[i][j] + '0';
					sudokuu[p++] = ' ';
				}
				else
				{
					sudokuu[p++] = res[i][j] + '0';
					if (i != 8)
						sudokuu[p++] = '\n';
				}
			}
		}
		suc = 1;
		return;
	}
	if (res[n / 9][n % 9] != 0)   //当前格子有数字，跳到下一格
	{
		TraceBack(n + 1);
	}
	else
	{
		for (int m = 1;m <= 9;m++)
		{
			int f = checkvis(n / 9, n % 9, m);
			if (f == 0)
				continue;
			else
			{
				res[n / 9][n % 9] = m;
				setvis(n / 9, n % 9, res[n / 9][n % 9]);
				TraceBack(n + 1);
				resetvis(n / 9, n % 9, res[n / 9][n % 9]);
				res[n / 9][n % 9] = 0;
			}
		}
	}
}

void Sudo::solve(char* file)
{
	FILE* fp1 = fopen(file, "r");
	FILE* fp2 = fopen("sudoku.txt", "w");
	int t = 0;
	int count = 0;
	while (fscanf(fp1, "%d", &res[t / 9][t % 9]) != EOF)
	{
		if (count != 0)//代表数独数量 
		{
			sudokuu[p++] = '\n';
		}
		for (int t = 1;t < 81;t++)     //读入一个需要求解的数独
			fscanf(fp1, "%d", &res[t / 9][t % 9]);
		suc = 0;
		memset(vis, 0, sizeof(vis));
		for (int t = 0;t < 81;t++)
			if (res[t / 9][t % 9] != 0)   //当前格子有数字，跳到下一格
			{
				setvis(t / 9, t % 9, res[t / 9][t % 9]);
			}
		TraceBack(0);   //回溯求解
		count++;
	}
	sudokuu[p++] = '\0';
	fwrite(sudokuu, 1, p, fp2);  //将结果写入文件
	fclose(fp1);
	fclose(fp2);
}

int main(int argc, char* argv[])
{
	Sudo sudo;
	if (argc == 3)
	{
		if (argv[1][1] == 'c')
		{
			int flag = 1;
			int i = 0;
			int j = strlen(argv[2]);
			for (int i = 0;i < j;i++)
			{
				if (argv[2][i]<'0' || argv[2][i]>'9')
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)
			{
				counts = atoi(argv[2]);
				if (counts >= 1 && counts <= 1000000)
				{
					sudo.create(counts);
				}
				else printf("输入数字范围出现错误\n");
			}
			else printf("输入的数字出现错误\n");
		}
		else
		{
			if (argv[1][1] == 's')
			{
				sudo.solve(argv[2]);
			}
			else printf("输入指令错误\n");

		}
	}
	else
	{
		printf("输入出现错误\n");
	}
	return 0;
}

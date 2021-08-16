/*
扫雷算法
1. 命令行输入要生成的数组大小以及雷的数量
2.随机产生一个位置用来放置一个雷
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MINE 'X'
#define EMPT '0'
#define OPEN 'K'
#define BLOCK '*'

void print_array(int sz, char (*a)[sz], char (*b)[sz], const char * title) {
    puts(title);
    for (size_t i = 0; i < sz; i++)
    {
        for (size_t j = 0; j < sz; j++)
        {
            if (b) {
                char ch = b[i][j];
                if (ch == OPEN) {
                    printf("%3c",a[i][j]);
                } else {
                    printf("%3c",'*');
                }
            } else {
                char ch = a[i][j];
                if (ch == MINE) {
                    printf("  \033[41m%c\033[0m",ch);
                } else {
                    printf("%3c",ch);
                }
            }
        }
        printf("\n");
    }
}

void open_block(int sz, char (*a)[sz], char (*b)[sz], int posi, int posj) {
    // 递归查找的方向有4个,上,下,左,右
    // 递归结束的条件为，当且位置处的数据为表示雷的数量时
    if ( posi < 0 || posi >= sz || posj < 0 || posj >= sz) 
        return;
    b[posi][posj] = OPEN;
    char ch = a[posi][posj];
    if (ch != MINE && ch != EMPT) {
        return;
    }
    a[posi][posj] = OPEN;
    // 上
    open_block(sz, a, b, posi - 1, posj);
    // 下
    open_block(sz, a, b, posi + 1, posj);
    // 左
    open_block(sz, a, b, posi, posj - 1);
    // 右
    open_block(sz, a, b, posi, posj + 1);
    a[posi][posj] = ch;
}

int main(int argc, char * argv[]) {
    if (argc < 3) {
        printf("参数个数为3\n");
        exit(EXIT_FAILURE);
    }
    int sz = atoi(argv[1]);
    if (sz < 0) {
        printf("输入的size必须大于0\n");
        exit(EXIT_FAILURE);
    }
    int mines = atoi(argv[2]);
    if (mines < 0) {
        printf("输入的mines必须大于0\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    char array[sz][sz];
    char info[sz][sz];
    for (size_t i = 0; i < sz; i++)
    {
        for (size_t j = 0; j < sz; j++)
        {
            array[i][j] = EMPT;
            info[i][j] = EMPT;
        }
    }
    // print_array(sz, array, "初始的数组为:");

    //1. 随机产生一个数据
    for (size_t cnt = 0; cnt < mines; cnt++)
    {
        int i = rand() % sz;
        int j = rand() % sz;
        array[i][j] = MINE;
    }
    // print_array(sz, array, "有雷的数组为:");

    //2. 在有雷的位置附近生成位置信息
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
        {
            char ch = array[i][j];
            // 对每一个非雷的数据,都遍历其周围的8个格子,判断雷的数量
            if (ch != MINE) {
                char num = '0';
                int row_start, row_end;
                row_start = (i - 1) >= 0 ? (i - 1) : 0;
                row_end = (i + 1) < sz ? (i + 1) : (sz - 1);
                int col_start, col_end;
                col_start = (j - 1) >= 0 ? (j - 1) : 0;
                col_end = (j + 1) < sz ? (j + 1) : (sz - 1);
                for (size_t row = row_start; row <= row_end; row++)
                {
                    for (size_t col = col_start; col <= col_end; col++)
                    {
                        if (array[row][col] == MINE) {
                            num += 1;
                        }
                    }
                }
                // printf("pos [%d,%d] row_start = %d, row_end = %d, col_start = %d, col_end = %d, have mines %d.\n", i, j, row_start, row_end, col_start, col_end, num);
                array[i][j] = num;
            }
        }
    }
    printf("------------------------------------\n");
    printf("--   Welcome to SWEEP program.    --\n");
    printf("------------------------------------\n");
    for (size_t i = 0; i < sz; i++)
    {
        for (size_t j = 0; j < sz; j++)
        {
            printf("%3c",BLOCK);
        }
        printf("\n");
    }
    printf("\n");
    // print_array(sz, array, NULL, "雷阵数据为:");
    int pos_i, pos_j;
    while (true)
    {
        printf("enter click pos [0,%d),[0,%d): ",sz, sz);
        scanf("%d %d", &pos_i, &pos_j);
        if (pos_i < 0 || pos_i >= sz || pos_j < 0 || pos_j >= sz) {
            printf("enter range is invalid.\n");
            continue;
        }
        char ch = array[pos_i][pos_j];
        printf("get the pos is (%d,%d). here's state is %c\n", pos_i, pos_j, ch);
        if (ch == MINE) {
            printf("GAME OVER! YOU LOSE~~~.\n");
            print_array(sz, array, NULL, "");
            printf("\n");
            break;
        } else {
            if (info[pos_i][pos_j] == OPEN) {
                puts("该位置已被翻开，请重新输入");
                continue;
            }
            // 从指定位置递归查找所有非0的块,并标记为打开
            if (ch == EMPT) {
                open_block(sz, array, info, pos_i, pos_j);
            } else {
                info[pos_i][pos_j] = OPEN;
            }
            print_array(sz, array, info, "");
            printf("\nWOW! YOU ARE SO LUCKY! GO ON...\n");
        }
    }
    
    return 0;
}